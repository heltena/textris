#!/bin/sh
# Builds site/textris.wasm from the recovered sources in ../src.
#
# Needs only clang with the wasm32 target (Apple's clang has it) and python3.
# No linker is required: tools/wasmlink.py turns clang's object file into a module.
#
# If binaryen's wasm-opt is found (tools/binaryen/bin, $WASM_OPT or the PATH) the
# module is also "asyncified", which lets it pause in every browser. Without it
# the build still works, but the page then needs WebAssembly JSPI (Chrome, Edge).
set -eu
cd "$(dirname "$0")"

CLANG="${CLANG:-clang}"
PYTHON="${PYTHON:-python3}"

mkdir -p build site
"$PYTHON" tools/prep.py ../src port build

# -O0 on purpose: the game busy-waits on variables changed by "interrupts"
# (while (cinc_segons < 3) ;), so nothing may be cached in registers, and the
# coverage hook has to stay in every loop. Speed is irrelevant here.
"$CLANG" --target=wasm32 -mcpu=mvp -O0 -std=gnu89 -fsigned-char -ffreestanding -fno-builtin \
    -fsanitize-coverage=trace-pc -w -Ibuild \
    -c build/unity.c -o build/textris.o

"$PYTHON" tools/wasmlink.py build/textris.o build/textris.linked.wasm --base 4096 \
    --export port_run --export port_key --export port_vram --export port_set_speed \
    > build/link.json
"$PYTHON" tools/checklink.py build/link.json

WASM_OPT="${WASM_OPT:-tools/binaryen/bin/wasm-opt}"
if [ ! -x "$WASM_OPT" ]; then
    WASM_OPT="$(command -v wasm-opt || true)"
fi
if [ -n "$WASM_OPT" ]; then
    "$WASM_OPT" --asyncify --pass-arg=asyncify-imports@env.host_sleep -O1 \
        build/textris.linked.wasm -o site/textris.wasm
    echo "build: site/textris.wasm ok (asyncify: any browser)"
else
    cp build/textris.linked.wasm site/textris.wasm
    echo "build: site/textris.wasm ok (no wasm-opt found: the page will need WebAssembly JSPI)"
fi
ls -l site/textris.wasm
