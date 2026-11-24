#!/usr/bin/env python3
"""Checks the summary printed by wasmlink.py against what site/machine.js provides."""
import json
import sys

d = json.load(open(sys.argv[1]))
provided = ["env.__linear_memory (memory)", "env.__stack_pointer (global)",
            "env.__indirect_function_table (table)", "env.host_now (func)",
            "env.host_sleep (func)", "env.host_sound (func)"]
extra = [i for i in d["imports"] if i not in provided]
if extra:
    sys.exit("build: the module needs functions the page does not provide: " + ", ".join(extra))
# Limits assumed by site/machine.js: 1 MiB of memory with the Asyncify buffer at
# 0x90000, and 256 function-table entries.
if d["data_end"] > 512 * 1024 or d["table_size"] > 256:
    sys.exit("build: module too large for the limits in site/machine.js")
print(f"link: data {d['data_base']}..{d['data_end']}, table {d['table_size']}, {d['bytes']} bytes")
