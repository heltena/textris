/* platform.c - the PC that Textris expects, rebuilt for WebAssembly.
 *
 * The 1997 game is plain C on top of a handful of assembly modules
 * (ACCES, ALTAVEU, CONVTIPS, INTERRUP, PANTALLA, STROBE, TECLAT). This file
 * provides those same routines, with the same C interfaces, on top of four
 * functions supplied by the web page:
 *
 *      host_now()        milliseconds since the page started
 *      host_sleep(ms)    give control back to the browser for a while
 *      host_sound(div)   PC speaker: 8253 timer divisor, 0 = silence
 *      (the page reads the 80x25 text screen straight from port_vram())
 *
 * The game was written for an interrupt-driven machine: it sits in loops such
 * as "while (cinc_segons < 3) ;" waiting for the timer interrupt to change a
 * variable. To keep that working without touching the game, the build asks
 * clang to call __sanitizer_cov_trace_pc() at every basic block. That hook is
 * the "CPU clock" of this little machine: every so many blocks it lets real
 * time catch up (host_sleep) and delivers the pending interrupts by calling
 * the handlers the game installed with CanviarVectInt().
 *
 * This file is #included at the top of the generated unity.c, before compat.h.
 */

/* ---- basic types (replaces Tipus.h: Turbo C's int was 16 bits) ---------- */
#define TIPUS_H
typedef unsigned short  WORD;   /* Natural de 16 bits */
typedef unsigned char   BYTE;   /* Natural de  8 bits */
typedef void *          PTR;    /* Punter genèric (far) */

#define PORT_IMPORT(name) __attribute__((import_module("env"), import_name(#name)))
#define PORT_NOHOOK __attribute__((no_sanitize("coverage")))

PORT_IMPORT(host_now)   double host_now (void);
PORT_IMPORT(host_sleep) void   host_sleep (double ms);
PORT_IMPORT(host_sound) void   host_sound (int divisor);

BYTE textris_main (void);            /* "main" of Textris.c, see compat.h */

/* ---- machine state ------------------------------------------------------- */
#define PORT_COLS 80
#define PORT_ROWS 25
#define PORT_CELLS (PORT_COLS * PORT_ROWS)

static BYTE port_video[PORT_CELLS * 2];   /* B800:0000 - character, attribute */

static void (*port_vector[256]) ();       /* interrupt vector table */
static BYTE port_if = 1;                  /* CPU interrupt flag */
static BYTE port_imr = 0;                 /* 8259 interrupt mask register */
static BYTE port_port60 = 0;              /* keyboard data port */

#define PORT_KEYS 64
static BYTE port_keys[PORT_KEYS];         /* scan codes waiting to be read */
static unsigned port_key_head = 0, port_key_tail = 0;

/* CPU speed: basic blocks executed per millisecond of emulated time. */
static unsigned port_steps_per_ms = 1500;
#define PORT_SLICE_MS 4                   /* interrupts are checked this often */
#define PORT_TICK_US 54925UL              /* 18.2 Hz timer interrupt */

static unsigned port_steps = 0;
static BYTE port_in_hook = 0;
static BYTE port_started = 0;
static double port_t0, port_last_yield;
static double port_vtime_ms = 0;          /* emulated time */
static unsigned long long port_vtime_us = 0, port_next_tick_us = PORT_TICK_US;

/* ---- interrupt delivery --------------------------------------------------- */
PORT_NOHOOK static void port_call_isr (void (*isr) ()) {
  port_if = 0;                            /* an interrupt clears IF ... */
  ((void (*) (void)) isr) ();
  port_if = 1;                            /* ... and IRET restores it */
}

PORT_NOHOOK static void port_deliver (void) {
  if (!port_if) return;

  if (port_vtime_us >= port_next_tick_us) {         /* IRQ 0: timer */
    port_next_tick_us += PORT_TICK_US;
    if (!(port_imr & 1) && port_vector[8]) port_call_isr (port_vector[8]);
  }

  if (port_key_head != port_key_tail && !(port_imr & 2)) {   /* IRQ 1: keyboard */
    port_port60 = port_keys[port_key_head];
    port_key_head = (port_key_head + 1) % PORT_KEYS;
    if (port_vector[9]) port_call_isr (port_vector[9]);
  }
}

/* One slice of emulated time has been executed: wait for the real clock to
 * catch up, then deliver interrupts. */
PORT_NOHOOK static void port_slice (void) {
  double now, ahead;

  if (!port_started) {
    port_started = 1;
    port_t0 = port_last_yield = host_now ();
  }
  port_vtime_ms += PORT_SLICE_MS;
  port_vtime_us += PORT_SLICE_MS * 1000UL;

  now = host_now ();
  ahead = port_vtime_ms - (now - port_t0);
  if (ahead < -250.0) {                   /* the tab was asleep: do not race */
    port_t0 -= ahead;
    ahead = 0.0;
  }
  if (ahead >= 1.0) {
    host_sleep (ahead);
    port_last_yield = host_now ();
  } else if (now - port_last_yield > 10.0) {
    host_sleep (0.0);                     /* keep the page responsive */
    port_last_yield = host_now ();
  }

  port_deliver ();
}

/* Called by the compiler-inserted instrumentation at every basic block. */
PORT_NOHOOK void __sanitizer_cov_trace_pc (void) {
  if (port_in_hook) return;
  if (++port_steps < port_steps_per_ms * PORT_SLICE_MS) return;
  port_steps = 0;
  port_in_hook = 1;
  port_slice ();
  port_in_hook = 0;
}

/* Used by the routines below that have to wait for something. */
PORT_NOHOOK static void port_idle (void) {
  BYTE was = port_in_hook;
  port_steps = 0;
  port_in_hook = 1;
  port_slice ();
  port_in_hook = was;
}

/* ---- functions the page calls --------------------------------------------- */
PORT_NOHOOK BYTE *port_vram (void) { return port_video; }

PORT_NOHOOK void port_key (unsigned scancode) {
  unsigned next = (port_key_tail + 1) % PORT_KEYS;
  if (next == port_key_head) return;      /* buffer full: the key is lost */
  port_keys[port_key_tail] = (BYTE) scancode;
  port_key_tail = next;
}

PORT_NOHOOK void port_set_speed (unsigned steps_per_ms) {
  if (steps_per_ms < 50) steps_per_ms = 50;
  port_steps_per_ms = steps_per_ms;
}

PORT_NOHOOK unsigned port_run (void) {
  unsigned code = textris_main ();
  host_sound (0);
  return code;
}

/* ---- the C library bits clang may ask for ---------------------------------- */
PORT_NOHOOK void *memcpy (void *d, const void *s, unsigned long n) {
  BYTE *dd = d; const BYTE *ss = s;
  while (n--) *dd++ = *ss++;
  return d;
}
PORT_NOHOOK void *memmove (void *d, const void *s, unsigned long n) {
  BYTE *dd = d; const BYTE *ss = s;
  if (dd < ss) while (n--) *dd++ = *ss++;
  else { dd += n; ss += n; while (n--) *--dd = *--ss; }
  return d;
}
PORT_NOHOOK void *memset (void *d, int c, unsigned long n) {
  BYTE *dd = d;
  while (n--) *dd++ = (BYTE) c;
  return d;
}

/* ==== ACCES.ASM ============================================================== */
/* Far pointers are kept as segment:offset packed in the 32 bits of a PTR. */
PORT_NOHOOK static BYTE *port_far (PTR Adr) {
  unsigned long v = (unsigned long) Adr;
  unsigned long lin = ((v >> 16) << 4) + (v & 0xFFFF);
  if (lin >= 0xB8000UL && lin < 0xB8000UL + PORT_CELLS * 2) return port_video + (lin - 0xB8000UL);
  return 0;
}
PORT_NOHOOK BYTE Peek (PTR Adr) { BYTE *p = port_far (Adr); return p ? *p : 0; }
PORT_NOHOOK void Poke (PTR Adr, BYTE Valor) { BYTE *p = port_far (Adr); if (p) *p = Valor; }
PORT_NOHOOK BYTE InPort (WORD Port) {
  if (Port == 0x60) return port_port60;
  if (Port == 0x21) return port_imr;
  return 0;
}
PORT_NOHOOK void OutPort (WORD Port, BYTE Valor) {
  if (Port == 0x21) port_imr = Valor;
}

/* ==== ALTAVEU.ASM ============================================================ */
/* The original programs timer 2 with this value: it is the 8253 divisor
 * (1193182 / Hz), not a frequency. 0 switches the speaker off. */
PORT_NOHOOK void So (WORD frequencia) { host_sound (frequencia); }

/* ==== CONVTIPS.ASM =========================================================== */
PORT_NOHOOK PTR Ptr (WORD Segment, WORD Desplacament) {
  return (PTR) (((unsigned long) Segment << 16) | Desplacament);
}
PORT_NOHOOK WORD Segment (PTR Adreca) { return (WORD) ((unsigned long) Adreca >> 16); }
PORT_NOHOOK WORD Desplacament (PTR Adreca) { return (WORD) ((unsigned long) Adreca & 0xFFFF); }
PORT_NOHOOK WORD Word (BYTE msb, BYTE lsb) { return (WORD) ((msb << 8) | lsb); }
PORT_NOHOOK BYTE MSB (WORD Valor) { return (BYTE) (Valor >> 8); }
PORT_NOHOOK BYTE LSB (WORD Valor) { return (BYTE) (Valor & 0xFF); }

/* ==== INTERRUP.ASM =========================================================== */
PORT_NOHOOK WORD Inhibir (void) {
  WORD flags = port_if ? 0x0202 : 0x0002;   /* bit 9 is IF */
  port_if = 0;
  return flags;
}
PORT_NOHOOK void Desinhibir (WORD Flags) { port_if = (Flags & 0x0200) != 0; }
PORT_NOHOOK void EOI (void) { }
/* Salvar and Restaurar wrapped a C function into an interrupt handler (saving
 * registers, loading DS, returning with IRET). Here handlers are ordinary C
 * functions called by port_deliver(), so there is nothing to save. */
PORT_NOHOOK void Salvar (void) { }
PORT_NOHOOK void Restaurar (void) { }
PORT_NOHOOK void (*CanviarVectInt (BYTE NumInt, void (*NovaRSI) ())) () {
  void (*antiga) () = port_vector[NumInt];
  port_vector[NumInt] = NovaRSI;
  return antiga;
}
PORT_NOHOOK void InhibirIRQ (BYTE irq) { port_imr |= (BYTE) (1 << (irq & 7)); }
PORT_NOHOOK void DesinhibirIRQ (BYTE irq) { port_imr &= (BYTE) ~(1 << (irq & 7)); }

/* ==== PANTALLA.ASM =========================================================== */
PORT_NOHOOK PTR BufferPantalla (void) { return Ptr (0xB800, 0x0000); }   /* colour text mode */

PORT_NOHOOK void EscriureCar (BYTE fila, BYTE columna, char caracter, BYTE atribut) {
  unsigned pos = fila * PORT_COLS + columna;
  if (pos >= PORT_CELLS) return;          /* the original would write past the screen */
  port_video[pos * 2] = (BYTE) caracter;
  port_video[pos * 2 + 1] = atribut;
}

PORT_NOHOOK void EscriureString (BYTE fila, BYTE columna, char *string, BYTE atribut) {
  unsigned pos = fila * PORT_COLS + columna;
  for (; *string && pos < PORT_CELLS; string++, pos++) {
    port_video[pos * 2] = (BYTE) *string;
    port_video[pos * 2 + 1] = atribut;
  }
}

PORT_NOHOOK void EsborrarPantalla (BYTE atribut) {
  unsigned pos;
  for (pos = 0; pos < PORT_CELLS; pos++) {
    port_video[pos * 2] = ' ';
    port_video[pos * 2 + 1] = atribut;
  }
}

PORT_NOHOOK WORD QuinCarAtr (BYTE fila, BYTE columna) {
  unsigned pos = fila * PORT_COLS + columna;
  if (pos >= PORT_CELLS) return 0;
  return (WORD) (port_video[pos * 2] | (port_video[pos * 2 + 1] << 8));
}

/* ==== STROBE.ASM ============================================================= */
/* The game strobes bit 7 of port 61h to acknowledge the keyboard. Nothing to do. */
PORT_NOHOOK void Strobe (WORD AdrPort, BYTE Bit0_7) { (void) AdrPort; (void) Bit0_7; }

/* ==== TECLAT.ASM ============================================================= */
PORT_NOHOOK void IniTeclatENQ (void) { port_imr |= 2; }
PORT_NOHOOK void FiTeclatENQ (void) { port_imr &= (BYTE) ~2; }

PORT_NOHOOK BYTE LlegirTeclatENQ (void) {
  BYTE code;
  for (;;) {
    while (port_key_head == port_key_tail) port_idle ();
    code = port_keys[port_key_head];
    port_key_head = (port_key_head + 1) % PORT_KEYS;
    port_port60 = code;
    if (!(code & 0x80)) return code;      /* only "make" codes */
  }
}

/* Same table as the reconstructed src/Teclat.asm. */
PORT_NOHOOK char ASCII (BYTE ScanCode) {
  static const char taula[] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 8, 9,
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', 13, 0,
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', 39, '`', 0, 92,
    'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', 0, '*', 0, ' ', 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    '7', '8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.' };
  if (ScanCode >= sizeof taula) return 0;
  return taula[ScanCode];
}
