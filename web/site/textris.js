// textris.js - the browser side: text screen, keyboard, PC speaker.
import { createMachine, CP437, SCANCODES, EXTENDED } from "./machine.js";

const COLS = 80, ROWS = 25;
const CW = 18, CH = 32;            // one text cell on the canvas (a 9x16 VGA cell, doubled)
const PALETTE = ["#000000", "#0000aa", "#00aa00", "#00aaaa", "#aa0000", "#aa00aa", "#aa5500", "#aaaaaa",
                 "#555555", "#5555ff", "#55ff55", "#55ffff", "#ff5555", "#ff55ff", "#ffff55", "#ffffff"];
const FONT = '28px "Cascadia Mono", "DejaVu Sans Mono", Menlo, Consolas, "Liberation Mono", monospace';

const canvas = document.getElementById("screen");
const ctx = canvas.getContext("2d", { alpha: false });
const overlay = document.getElementById("overlay");
const message = document.getElementById("message");
const startButton = document.getElementById("start");
const muteButton = document.getElementById("mute");
const statusLine = document.getElementById("status");
const params = new URLSearchParams(location.search);

// ---- text screen ---------------------------------------------------------------
const shadow = new Uint8Array(COLS * ROWS * 2).fill(255);   // what is on the canvas now

function drawCell(col, row, code, attr) {
  const x = col * CW, y = row * CH;
  const fg = PALETTE[attr & 15], bg = PALETTE[(attr >> 4) & 7];
  ctx.fillStyle = bg;
  ctx.fillRect(x, y, CW, CH);
  ctx.fillStyle = fg;
  switch (code) {
    case 0x00: case 0x20: case 0xff: return;
    // Block characters are drawn as rectangles so that they tile exactly.
    case 0xdb: ctx.fillRect(x, y, CW, CH); return;                    // █
    case 0xdc: ctx.fillRect(x, y + CH / 2, CW, CH / 2); return;        // ▄
    case 0xdf: ctx.fillRect(x, y, CW, CH / 2); return;                 // ▀
    case 0xdd: ctx.fillRect(x, y, CW / 2, CH); return;                 // ▌
    case 0xde: ctx.fillRect(x + CW / 2, y, CW / 2, CH); return;        // ▐
    case 0xfe: ctx.fillRect(x + 4, y + 10, CW - 8, CH - 20); return;   // ■
    case 0xc4: ctx.fillRect(x, y + CH / 2 - 1, CW, 2); return;         // ─
    case 0xb3: ctx.fillRect(x + CW / 2 - 1, y, 2, CH); return;         // │
    case 0xcd: ctx.fillRect(x, y + CH / 2 - 4, CW, 2); ctx.fillRect(x, y + CH / 2 + 2, CW, 2); return;  // ═
    case 0xba: ctx.fillRect(x + CW / 2 - 4, y, 2, CH); ctx.fillRect(x + CW / 2 + 2, y, 2, CH); return;  // ║
    case 0xb0: case 0xb1: case 0xb2:                                    // ░ ▒ ▓ as VGA-style dither
      for (let py = 0; py < 16; py++) {
        for (let px = 0; px < 9; px++) {
          const light = (px + 2 * (py & 1)) % 4 === 0;                  // 25 % of the pixels
          const on = code === 0xb0 ? light : code === 0xb1 ? ((px + py) & 1) === 0 : !light;
          if (on) ctx.fillRect(x + px * 2, y + py * 2, 2, 2);
        }
      }
      return;
  }
  ctx.fillText(CP437[code], x + CW / 2, y + CH / 2 + 1, CW);
}

function paint(video) {
  ctx.font = FONT;
  ctx.textAlign = "center";
  ctx.textBaseline = "middle";
  for (let i = 0; i < COLS * ROWS; i++) {
    const code = video[i * 2], attr = video[i * 2 + 1];
    if (shadow[i * 2] === code && shadow[i * 2 + 1] === attr) continue;
    shadow[i * 2] = code;
    shadow[i * 2 + 1] = attr;
    drawCell(i % COLS, (i / COLS) | 0, code, attr);
  }
}

// ---- PC speaker -----------------------------------------------------------------
let audio = null, oscillator = null, gain = null, muted = false, divisor = 0;

function applySound() {
  if (!audio) return;
  const on = divisor > 0 && !muted;
  if (on) oscillator.frequency.setValueAtTime(Math.min(1193182 / divisor, 12000), audio.currentTime);
  gain.gain.setValueAtTime(on ? 0.05 : 0, audio.currentTime);
}

function startAudio() {
  if (audio) { audio.resume(); return; }
  const AudioContext = window.AudioContext || window.webkitAudioContext;
  if (!AudioContext) return;
  audio = new AudioContext();
  oscillator = audio.createOscillator();
  oscillator.type = "square";
  gain = audio.createGain();
  gain.gain.value = 0;
  oscillator.connect(gain).connect(audio.destination);
  oscillator.start();
  applySound();
}

muteButton.addEventListener("click", () => {
  muted = !muted;
  muteButton.setAttribute("aria-pressed", String(muted));
  muteButton.textContent = "So del navegador: " + (muted ? "silenciat" : "activat");
  applySound();
});

// ---- keyboard ---------------------------------------------------------------------
let machine = null;
const down = new Set();

function sendKey(code, release) {
  if (!machine) return false;
  if (code in SCANCODES) {
    machine.key(SCANCODES[code] | (release ? 0x80 : 0));
  } else if (code in EXTENDED) {
    machine.key(0xe0);
    machine.key(EXTENDED[code] | (release ? 0x80 : 0));
  } else {
    return false;
  }
  return true;
}

window.addEventListener("keydown", (e) => {
  if (e.metaKey || e.ctrlKey || e.altKey || !overlay.hidden) return;
  if (sendKey(e.code, false)) {
    down.add(e.code);
    e.preventDefault();
  }
});
window.addEventListener("keyup", (e) => {
  if (!down.delete(e.code)) return;
  sendKey(e.code, true);
  e.preventDefault();
});
window.addEventListener("blur", () => {           // no stuck keys when the window loses focus
  for (const code of down) sendKey(code, true);
  down.clear();
});

// ---- running the game ---------------------------------------------------------------
const canSuspend = typeof WebAssembly.Suspending === "function" && typeof WebAssembly.promising === "function";
let wasmBytes = null;

function show(text, buttonLabel) {
  message.textContent = text;
  startButton.hidden = !buttonLabel;
  if (buttonLabel) startButton.textContent = buttonLabel;
  overlay.hidden = false;
  if (buttonLabel) startButton.focus();
}

async function play() {
  overlay.hidden = true;
  shadow.fill(255);
  const host = {
    suspending: true,
    now: () => performance.now(),
    sleep: (ms) => new Promise((resolve) => setTimeout(resolve, ms)),
    sound: (d) => { divisor = d; applySound(); },
  };
  const m = await createMachine(wasmBytes, host);
  if (params.has("speed")) m.setSpeed(Number(params.get("speed")));
  machine = m;
  // Test aid: ?autostart&keys=5000:45,6000:30 presses scan codes at the given milliseconds.
  for (const item of (params.get("keys") || "").split(",").filter(Boolean)) {
    const [ms, code] = item.split(":").map(Number);
    setTimeout(() => machine === m && m.key(code), ms);
    setTimeout(() => machine === m && m.key(code | 0x80), ms + 80);
  }
  if (params.has("debug")) {   // Test aid: mirrors the text screen into the page as plain text.
    const pre = document.body.appendChild(document.createElement("pre"));
    pre.id = "debug";
    let frames = 0;
    requestAnimationFrame(function count() { frames++; requestAnimationFrame(count); });
    setInterval(() => {
      const rows = [];
      for (let r = 0; r < ROWS; r++) {
        let s = "";
        for (let c = 0; c < COLS; c++) s += CP437[m.video[(r * COLS + c) * 2]];
        rows.push(s);
      }
      pre.textContent = `t=${Math.round(performance.now())} frames=${frames}\n` + rows.join("\n");
    }, 250);
  }
  statusLine.textContent = "En marxa";
  let frame = requestAnimationFrame(function loop() {
    paint(m.video);
    frame = requestAnimationFrame(loop);
  });
  let text;
  try {
    const code = await m.run();
    text = code === 0 ? "El programa ha acabat." : `El programa ha acabat (codi ${code}).`;
  } catch (err) {
    console.error(err);
    text = "El programa s'ha aturat per un error: " + err.message;
  }
  machine = null;
  down.clear();
  divisor = 0;
  applySound();
  paint(m.video);
  cancelAnimationFrame(frame);
  statusLine.textContent = "Aturat";
  show(text, "Tornar a començar");
}

startButton.addEventListener("click", () => {
  startAudio();
  play();
});

(async function boot() {
  if (!canSuspend) {
    show("Aquest navegador no té WebAssembly JSPI (JavaScript Promise Integration), que el joc necessita " +
         "per esperar el rellotge i el teclat. Prova-ho amb una versió recent de Chrome o Edge.");
    return;
  }
  try {
    const response = await fetch("textris.wasm");
    if (!response.ok) throw new Error(`textris.wasm: HTTP ${response.status}`);
    wasmBytes = await response.arrayBuffer();
  } catch (err) {
    show("No s'ha pogut carregar textris.wasm (" + err.message + "). Cal servir aquesta carpeta per HTTP; " +
         "obrir index.html directament des del disc no funciona.");
    return;
  }
  if (params.has("autostart")) play();
  else show("Dos jugadors, un teclat. Prem per començar (el so necessita un clic).", "Jugar");
})();
