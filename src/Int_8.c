#include "int_8.h"


/* Variables que porten els tics */
BYTE tics_nivell, tics_joc, tics_rellotge, tics_cinc_segons, tics_panpalluga;

/* Nivell intern de la interrupció de rellotge */
BYTE nivell_RSI_8;

/* S'ha de posar el rellotge a pantalla? */
BYTE hi_ha_rellotge;

/* Comptadors de l'hora actual */
BYTE minuts, segons;

/* Comptador de cinc segons (per començar la partida) */
BYTE cinc_segons;

/* Un clock de mig segon per fer panpallugues. Variable booleana. */
BYTE panpalluga;

/* Perque funcioni l'aleatori */
WORD valor_aleatori_RSI_8;

/* Variables booleanes que indica si la interrupció de rellotge a baixat */
/* la peça i si permet actualitzar les peces dels jugadors */
BYTE RSI_8_fa_baixar, RSI_8_permet_canviar;


void iniciar_RSI_8 (BYTE nou_nivell) {
 iniciar_canco (&musica_tetris, CERT);
 tics_nivell=0; tics_joc=0; tics_rellotge=0;
 tics_cinc_segons=0; tics_panpalluga=0;
 nivell_RSI_8=nou_nivell;
 RSI_8_fa_baixar=FALS;
 RSI_8_permet_canviar=FALS;
 hi_ha_rellotge=FALS;
 panpalluga=FALS;
 antic_punter_int_8=CanviarVectInt (8, RSI_8);
}


void RSI_8 (void) {
 Salvar ();
 if (hi_ha_so)
  actualitzar_canco (&musica_tetris);
 else So (0);

 if (hi_ha_EFESP) {
  actualitzar_canco (&musica_moure_peca); /* si està en off no pasa res */
  actualitzar_canco (&musica_esborrar_una); /* ordre mínim */
  actualitzar_canco (&musica_esborrar_dos);
  actualitzar_canco (&musica_esborrar_tres);
  actualitzar_canco (&musica_esborrar_textris); /* ordre màxim */
 }

 if (!hi_ha_so && !hi_ha_EFESP) So (0);

 if ( ++tics_nivell > MAX_NIVELL-nivell_RSI_8+2 ) { /* una fórmula simple */
  RSI_8_fa_baixar=CERT; /* Baixa la peça */
  tics_nivell=0;
 }

 if ( ++tics_joc > MAX_TICS_JOC ) { /* Permetrà que el jugador mogui? */
  RSI_8_permet_canviar=CERT;
  tics_joc=0;
 }

 if ( ++tics_cinc_segons > 18 ) { /* A pasat un segon? */
  tics_cinc_segons=0;
  cinc_segons++; /* A pasat un segon */
 }

 if ( hi_ha_rellotge ) { /* Posem el rellotge a pantalla? */
  if ( ++tics_rellotge > 18 ) { /* A pasat un segon? */
   tics_rellotge=0;
   if ( ++segons >= 59 ) { /* I un minut? */
        segons=0;
        if ( ++minuts >= 59 ) /* col?!%&lons una hora de joc? */
         minuts=0;
   }
  }
  /* Posem el rellotge */
  EscriureCar (FILA_REL, COLUMNA_REL,   '0'+minuts/10, ATR_REL);
  EscriureCar (FILA_REL, COLUMNA_REL+1, '0'+minuts%10, ATR_REL);
  EscriureCar (FILA_REL, COLUMNA_REL+2, ':', ATR_REL);
  EscriureCar (FILA_REL, COLUMNA_REL+3, '0'+segons/10, ATR_REL);
  EscriureCar (FILA_REL, COLUMNA_REL+4, '0'+segons%10, ATR_REL);
 }

 if ( ++tics_panpalluga > 9 ) { /* fa panpalluga? */
  tics_panpalluga=0;
  panpalluga=!panpalluga;
 }

 valor_aleatori_RSI_8++; /* quan arriba a 65535+1 torna a zero */

 EOI ();
 Restaurar ();
}


void acabar_RSI_8 () {
 CanviarVectInt (8, antic_punter_int_8);
 So (0); /* per treure el so */
}


void canviar_nivell (BYTE nou_nivell) {
 nivell_RSI_8=nou_nivell;
}
