// Headless smoke test: runs the game in Node on a virtual clock, presses keys
// at scripted times and prints the 80x25 screen.
//   node test.mjs                         presentation + menu
//   node test.mjs 3500:dump 4000:45 9000:dump      time_ms:scancode | time_ms:dump
//   node test.mjs --async ...             same, but really pausing the module at
//                                         every sleep (the path browsers use)
import fs from "node:fs";
import { createMachine, screenText } from "./site/machine.js";

const args = process.argv.slice(2);
const asyncMode = args[0] === "--async";
if (asyncMode) args.shift();
const script = (args.length ? args : ["1500:dump", "5000:dump"])
  .map((s) => { const [t, what] = s.split(":"); return { t: Number(t), what }; })
  .sort((a, b) => a.t - b.t);
const end = script[script.length - 1].t + 1;

let clock = 0, sounds = 0, lastSound = 0, sleeps = 0, machine;
const STOP = new Error("stop");

function dump(label) {
  console.log(`---- ${label} ` + "-".repeat(Math.max(0, 70 - label.length)));
  for (const line of screenText(machine.video)) console.log("|" + line + "|");
}

const host = {
  suspending: asyncMode,
  now: () => clock,
  sound: (d) => { sounds++; lastSound = d; },
  sleep: (ms) => {
    clock += Math.max(ms, 0.01);
    sleeps++;
    while (script.length && script[0].t <= clock) {
      const ev = script.shift();
      if (ev.what === "dump") dump(`t=${ev.t} ms  (sound calls: ${sounds}, last divisor: ${lastSound})`);
      else {
        const code = Number(ev.what);
        machine.key(code);
        if (!(code & 0x80)) {             // release the key 80 ms later
          script.push({ t: ev.t + 80, what: String(code | 0x80) });
          script.sort((a, b) => a.t - b.t);
        }
      }
    }
    if (clock > end) {
      if (!asyncMode) throw STOP;
      console.log(`(mode ${machine.mode}, ${sleeps} sleeps)`);
      process.exit(0);
    }
    if (asyncMode) return Promise.resolve();
  },
};

machine = await createMachine(fs.readFileSync(new URL("./site/textris.wasm", import.meta.url)), host);
try {
  const code = await machine.run();
  dump(`program ended with code ${code} at t=${Math.round(clock)} ms`);
} catch (e) {
  if (e !== STOP) { dump(`CRASH at t=${Math.round(clock)} ms: ${e.message}`); throw e; }
}
