// machine.js - loads textris.wasm and gives it the "PC" it expects.
// No DOM in here: the browser page (textris.js) and the Node test both use it.

// Unicode equivalents of the 256 characters of the DOS code page 437.
export const CP437 =
  " \u263a\u263b\u2665\u2666\u2663\u2660\u2022\u25d8\u25cb\u25d9\u2642\u2640\u266a\u266b\u263c\u25ba\u25c4\u2195\u203c\u00b6\u00a7\u25ac\u21a8\u2191\u2193\u2192\u2190\u221f\u2194\u25b2\u25bc" +
  " !\"#$%&'()*+,-./0123456789:;<=>?" +
  "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_" +
  "`abcdefghijklmnopqrstuvwxyz{|}~\u2302" +
  "\u00c7\u00fc\u00e9\u00e2\u00e4\u00e0\u00e5\u00e7\u00ea\u00eb\u00e8\u00ef\u00ee\u00ec\u00c4\u00c5\u00c9\u00e6\u00c6\u00f4\u00f6\u00f2\u00fb\u00f9\u00ff\u00d6\u00dc\u00a2\u00a3\u00a5\u20a7\u0192" +
  "\u00e1\u00ed\u00f3\u00fa\u00f1\u00d1\u00aa\u00ba\u00bf\u2310\u00ac\u00bd\u00bc\u00a1\u00ab\u00bb\u2591\u2592\u2593\u2502\u2524\u2561\u2562\u2556\u2555\u2563\u2551\u2557\u255d\u255c\u255b\u2510" +
  "\u2514\u2534\u252c\u251c\u2500\u253c\u255e\u255f\u255a\u2554\u2569\u2566\u2560\u2550\u256c\u2567\u2568\u2564\u2565\u2559\u2558\u2552\u2553\u256b\u256a\u2518\u250c\u2588\u2584\u258c\u2590\u2580" +
  "\u03b1\u00df\u0393\u03c0\u03a3\u03c3\u00b5\u03c4\u03a6\u0398\u03a9\u03b4\u221e\u03c6\u03b5\u2229\u2261\u00b1\u2265\u2264\u2320\u2321\u00f7\u2248\u00b0\u2219\u00b7\u221a\u207f\u00b2\u25a0\u00a0";

// IBM PC keyboard scan codes (set 1) for KeyboardEvent.code values.
// Keys in EXTENDED send an E0 prefix first, like a real 101-key keyboard;
// their base code is the same as the numeric keypad key they duplicate.
export const SCANCODES = {
  Escape: 0x01, Digit1: 0x02, Digit2: 0x03, Digit3: 0x04, Digit4: 0x05, Digit5: 0x06,
  Digit6: 0x07, Digit7: 0x08, Digit8: 0x09, Digit9: 0x0a, Digit0: 0x0b, Minus: 0x0c,
  Equal: 0x0d, Backspace: 0x0e, Tab: 0x0f, KeyQ: 0x10, KeyW: 0x11, KeyE: 0x12, KeyR: 0x13,
  KeyT: 0x14, KeyY: 0x15, KeyU: 0x16, KeyI: 0x17, KeyO: 0x18, KeyP: 0x19, BracketLeft: 0x1a,
  BracketRight: 0x1b, Enter: 0x1c, ControlLeft: 0x1d, KeyA: 0x1e, KeyS: 0x1f, KeyD: 0x20,
  KeyF: 0x21, KeyG: 0x22, KeyH: 0x23, KeyJ: 0x24, KeyK: 0x25, KeyL: 0x26, Semicolon: 0x27,
  Quote: 0x28, Backquote: 0x29, ShiftLeft: 0x2a, Backslash: 0x2b, KeyZ: 0x2c, KeyX: 0x2d,
  KeyC: 0x2e, KeyV: 0x2f, KeyB: 0x30, KeyN: 0x31, KeyM: 0x32, Comma: 0x33, Period: 0x34,
  Slash: 0x35, ShiftRight: 0x36, NumpadMultiply: 0x37, AltLeft: 0x38, Space: 0x39,
  CapsLock: 0x3a, Numpad7: 0x47, Numpad8: 0x48, Numpad9: 0x49, NumpadSubtract: 0x4a,
  Numpad4: 0x4b, Numpad5: 0x4c, Numpad6: 0x4d, NumpadAdd: 0x4e, Numpad1: 0x4f,
  Numpad2: 0x50, Numpad3: 0x51, Numpad0: 0x52, NumpadDecimal: 0x53,
};
export const EXTENDED = {
  Home: 0x47, ArrowUp: 0x48, PageUp: 0x49, ArrowLeft: 0x4b, ArrowRight: 0x4d, End: 0x4f,
  ArrowDown: 0x50, PageDown: 0x51, Insert: 0x52, Delete: 0x53, NumpadEnter: 0x1c,
  ControlRight: 0x1d, AltRight: 0x38, NumpadDivide: 0x35,
};

const PAGE = 65536;
const MEMORY_PAGES = 16;      // 1 MiB: data from 4096 up, stack from the top down
const TABLE_ENTRIES = 256;    // build.sh checks the module fits these limits

// Fixed place in the module's memory where Asyncify saves the call stack while
// the game sleeps (above the data, well below the stack; see MEMORY_PAGES).
const ASYNC_DATA = 0x90000;
const ASYNC_SIZE = 0x10000;

// host = { now(): ms, sleep(ms), sound(divisor), suspending: bool }
//
// The game blocks in loops waiting for "interrupts", so host_sleep() has to pause
// the WebAssembly code and give control back to the browser. Three ways:
//   asyncify  the module was post-processed by binaryen (build.sh does it when
//             wasm-opt is available): works in every browser. run() -> Promise.
//   jspi      plain module + WebAssembly JS Promise Integration. run() -> Promise.
//   sync      host.suspending = false: sleep() returns at once (Node test with a
//             virtual clock). run() returns the exit code directly.
export async function createMachine(wasmBytes, host) {
  const module = await WebAssembly.compile(wasmBytes);
  const asyncified = WebAssembly.Module.exports(module).some((e) => e.name === "asyncify_start_unwind");
  const jspi = typeof WebAssembly.Suspending === "function" && typeof WebAssembly.promising === "function";
  const mode = !host.suspending ? "sync" : asyncified ? "asyncify" : jspi ? "jspi" : null;
  if (!mode) throw new Error("this build needs WebAssembly JSPI, which this browser does not have");

  const memory = new WebAssembly.Memory({ initial: MEMORY_PAGES });
  const table = new WebAssembly.Table({ initial: TABLE_ENTRIES, element: "anyfunc" });
  const stackPointer = new WebAssembly.Global({ value: "i32", mutable: true }, MEMORY_PAGES * PAGE);
  const words = new Int32Array(memory.buffer);
  let x = null;                 // the module's exports
  let state = "running";        // asyncify: running | unwinding | sleeping | rewinding
  let wake = null;              // asyncify: promise that ends the current sleep

  let sleep = host.sleep;
  if (mode === "jspi") sleep = new WebAssembly.Suspending(host.sleep);
  if (mode === "asyncify") {
    sleep = (ms) => {
      if (state === "rewinding") {          // back where we left off: carry on
        x.asyncify_stop_rewind();
        state = "running";
        return;
      }
      wake = host.sleep(ms);
      words[ASYNC_DATA >> 2] = ASYNC_DATA + 8;
      words[(ASYNC_DATA >> 2) + 1] = ASYNC_DATA + ASYNC_SIZE;
      x.asyncify_start_unwind(ASYNC_DATA);  // every C function now returns to run()
      state = "unwinding";
    };
  }

  const instance = await WebAssembly.instantiate(module, {
    env: {
      __linear_memory: memory,
      __indirect_function_table: table,
      __stack_pointer: stackPointer,
      host_now: host.now,
      host_sleep: sleep,
      host_sound: host.sound,
    },
  });
  x = instance.exports;

  let run = x.port_run;
  if (mode === "jspi") run = WebAssembly.promising(x.port_run);
  if (mode === "asyncify") {
    run = async () => {
      let code = x.port_run();
      while (state === "unwinding") {
        x.asyncify_stop_unwind();
        state = "sleeping";
        await wake;
        state = "rewinding";
        x.asyncify_start_rewind(ASYNC_DATA);
        code = x.port_run();                // replays the call stack up to host_sleep
      }
      return code;
    };
  }

  return {
    mode,
    video: new Uint8Array(memory.buffer, x.port_vram(), 80 * 25 * 2),   // character, attribute pairs
    run,
    key: (scancode) => x.port_key(scancode),
    setSpeed: (stepsPerMs) => x.port_set_speed(stepsPerMs),
  };
}

export function screenText(video) {
  const lines = [];
  for (let row = 0; row < 25; row++) {
    let s = "";
    for (let col = 0; col < 80; col++) s += CP437[video[(row * 80 + col) * 2]];
    lines.push(s);
  }
  return lines;
}
