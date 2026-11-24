#!/usr/bin/env python3
"""Minimal single-object WebAssembly "linker".

clang can emit a WebAssembly *object* file without any extra tool, but turning
it into a module normally needs wasm-ld. A single object file is already almost
a valid module: it imports its memory, stack pointer and function table from
`env`, lays its data out from address 0 and carries the function-table entries.
This script does the little that is missing, for ONE object file:

  * moves the data from address 0 up to --base (so that no variable lives at
    the NULL address), fixing every address the relocation tables point at;
  * adds an export section for the functions named with --export;
  * drops the linker-only custom sections.

The page still has to provide env.__linear_memory, env.__stack_pointer and
env.__indirect_function_table when it instantiates the module.

usage: wasmlink.py in.o out.wasm --base 4096 --export name [--export name ...]
Prints a JSON summary (data end address, table size needed, imports) to stdout.
"""
import argparse
import json
import sys

R_FUNCTION_INDEX_LEB, R_TABLE_INDEX_SLEB, R_TABLE_INDEX_I32 = 0, 1, 2
R_MEMORY_ADDR_LEB, R_MEMORY_ADDR_SLEB, R_MEMORY_ADDR_I32 = 3, 4, 5
HAS_ADDEND = {3, 4, 5, 8, 9, 11, 14, 15, 16, 17, 21, 22, 23, 25}
SUPPORTED = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 13, 20}


class Reader:
    def __init__(self, data, pos=0):
        self.b, self.p = data, pos

    def byte(self):
        v = self.b[self.p]
        self.p += 1
        return v

    def uleb(self):
        r = s = 0
        while True:
            x = self.byte()
            r |= (x & 0x7F) << s
            s += 7
            if not x & 0x80:
                return r

    def sleb(self):
        r = s = 0
        while True:
            x = self.byte()
            r |= (x & 0x7F) << s
            s += 7
            if not x & 0x80:
                if x & 0x40:
                    r -= 1 << s
                return r

    def take(self, n):
        v = self.b[self.p:self.p + n]
        self.p += n
        return v

    def name(self):
        return self.take(self.uleb()).decode("utf-8")

    def eof(self):
        return self.p >= len(self.b)


def uleb(v):
    out = bytearray()
    while True:
        x = v & 0x7F
        v >>= 7
        if v:
            out.append(x | 0x80)
        else:
            out.append(x)
            return bytes(out)


def sleb(v):
    out = bytearray()
    while True:
        x = v & 0x7F
        v >>= 7
        if (v == 0 and not x & 0x40) or (v == -1 and x & 0x40):
            out.append(x)
            return bytes(out)
        out.append(x | 0x80)


def padded_uleb5(v):
    v &= 0xFFFFFFFF
    return bytes([(v & 0x7F) | 0x80, ((v >> 7) & 0x7F) | 0x80, ((v >> 14) & 0x7F) | 0x80,
                  ((v >> 21) & 0x7F) | 0x80, (v >> 28) & 0x0F])


def padded_sleb5(v):
    out = bytearray()
    for i in range(5):
        x = v & 0x7F
        v >>= 7
        out.append(x | (0x80 if i < 4 else 0))
    return bytes(out)


def vec(items):
    return uleb(len(items)) + b"".join(items)


def wname(s):
    e = s.encode("utf-8")
    return uleb(len(e)) + e


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("inp")
    ap.add_argument("out")
    ap.add_argument("--base", type=int, default=4096)
    ap.add_argument("--export", action="append", default=[])
    args = ap.parse_args()

    data = open(args.inp, "rb").read()
    if data[:8] != b"\0asm\x01\0\0\0":
        sys.exit("not a WebAssembly binary")

    # ---- split into sections -------------------------------------------------
    r = Reader(data, 8)
    sections = []  # [id, payload(bytearray), custom name]
    while not r.eof():
        sid = r.byte()
        size = r.uleb()
        payload = bytearray(r.take(size))
        cname = None
        if sid == 0:
            cr = Reader(payload)
            cname = cr.name()
            payload = payload[cr.p:]
        sections.append([sid, payload, cname])

    def find(sid):
        for i, s in enumerate(sections):
            if s[0] == sid:
                return i
        return None

    # ---- imports (for the summary and to count imported functions) -----------
    imports = []
    r = Reader(sections[find(2)][1])
    for _ in range(r.uleb()):
        mod, fld, kind = r.name(), r.name(), r.byte()
        if kind == 0:
            r.uleb()
        elif kind == 1:
            r.byte()
            flags = r.uleb()
            r.uleb()
            if flags & 1:
                r.uleb()
        elif kind == 2:
            flags = r.uleb()
            r.uleb()
            if flags & 1:
                r.uleb()
        elif kind == 3:
            r.byte()
            r.byte()
        else:
            sys.exit(f"unsupported import kind {kind}")
        imports.append((mod, fld, "func table memory global".split()[kind]))

    # ---- symbol table --------------------------------------------------------
    functions = {}
    linking = next(s for s in sections if s[2] == "linking")
    r = Reader(linking[1])
    if r.uleb() != 2:
        sys.exit("unsupported linking section version")
    while not r.eof():
        sub, size = r.byte(), r.uleb()
        end = r.p + size
        if sub == 8:  # WASM_SYMBOL_TABLE
            for _ in range(r.uleb()):
                kind, flags = r.byte(), r.uleb()
                undefined, explicit = flags & 0x10, flags & 0x40
                if kind in (0, 2, 4, 5):  # function, global, tag, table
                    index = r.uleb()
                    name = r.name() if (not undefined or explicit) else None
                    if kind == 0 and not undefined:
                        functions[name] = index
                elif kind == 1:  # data
                    r.name()
                    if not undefined:
                        r.uleb(), r.uleb(), r.uleb()
                elif kind == 3:  # section
                    r.uleb()
                else:
                    sys.exit(f"unsupported symbol kind {kind}")
        r.p = end

    # ---- apply the data base to every memory-address relocation --------------
    for s in sections:
        if not (s[2] or "").startswith("reloc."):
            continue
        r = Reader(s[1])
        target = sections[r.uleb()][1]
        for _ in range(r.uleb()):
            rtype, off = r.uleb(), r.uleb()
            r.uleb()  # symbol index
            if rtype in HAS_ADDEND:
                r.sleb()
            if rtype not in SUPPORTED:
                sys.exit(f"unsupported relocation type {rtype} in {s[2]}")
            if rtype == R_MEMORY_ADDR_LEB:
                v = Reader(target, off).uleb()
                target[off:off + 5] = padded_uleb5(v + args.base)
            elif rtype == R_MEMORY_ADDR_SLEB:
                v = Reader(target, off).sleb()
                target[off:off + 5] = padded_sleb5(v + args.base)
            elif rtype == R_MEMORY_ADDR_I32:
                v = int.from_bytes(target[off:off + 4], "little")
                target[off:off + 4] = ((v + args.base) & 0xFFFFFFFF).to_bytes(4, "little")

    # ---- rebuild the data section at the new base -----------------------------
    data_end = args.base
    di = find(11)
    if di is not None:
        r = Reader(sections[di][1])
        segs = []
        for _ in range(r.uleb()):
            if r.uleb() != 0 or r.byte() != 0x41:
                sys.exit("unsupported data segment form")
            off = r.sleb()
            if r.byte() != 0x0B:
                sys.exit("unsupported data segment offset expression")
            content = r.take(r.uleb())
            addr = off + args.base
            data_end = max(data_end, addr + len(content))
            segs.append(uleb(0) + b"\x41" + sleb(addr) + b"\x0b" + uleb(len(content)) + bytes(content))
        sections[di][1] = bytearray(vec(segs))

    # ---- function table size needed --------------------------------------------
    table_size = 1
    ei = find(9)
    if ei is not None:
        r = Reader(sections[ei][1])
        for _ in range(r.uleb()):
            if r.uleb() != 0 or r.byte() != 0x41:
                sys.exit("unsupported element segment form")
            off = r.sleb()
            r.byte()
            n = r.uleb()
            for _ in range(n):
                r.uleb()
            table_size = max(table_size, off + n)

    # ---- export section ---------------------------------------------------------
    missing = [n for n in args.export if n not in functions]
    if missing:
        sys.exit("cannot export, not defined in the object: " + ", ".join(missing))
    exports = vec([wname(n) + b"\x00" + uleb(functions[n]) for n in args.export])

    out = bytearray(b"\0asm\x01\0\0\0")
    done = False
    for sid, payload, cname in sections:
        if sid == 0:
            continue  # drop linking / reloc.* / producers / target_features
        if sid == 7:
            sys.exit("object already has an export section")
        if not done and sid > 7:
            out += b"\x07" + uleb(len(exports)) + exports
            done = True
        out += bytes([sid]) + uleb(len(payload)) + payload
    if not done:
        out += b"\x07" + uleb(len(exports)) + exports
    open(args.out, "wb").write(out)

    print(json.dumps({
        "data_base": args.base,
        "data_end": data_end,
        "table_size": table_size,
        "imports": [f"{m}.{f} ({k})" for m, f, k in imports],
        "exports": args.export,
        "bytes": len(out),
    }, indent=1))


if __name__ == "__main__":
    main()
