# Textris al navegador

El codi C original de `../src` compilat a WebAssembly, sense modificar-lo. Les rutines
d'assemblador (pantalla, teclat, interrupcions, altaveu) les substitueix `port/platform.c`.

## Jugar

```sh
cd site
python3 -m http.server 8765
```

i obrir <http://127.0.0.1:8765/>. Cal servir la carpeta per HTTP; obrir `index.html` des del
disc no funciona. La carpeta `site/` és tota la web: es pot copiar tal qual a qualsevol
allotjament estàtic.

Per publicar-la a GitHub Pages n'hi ha prou amb pujar el contingut de `site/`
(`index.html`, `textris.js`, `machine.js`, `textris.wasm`); no cal cap capçalera ni
configuració especial.

Provat amb Chrome 155. El mòdul es compila amb Asyncify (binaryen), que no demana res
especial al navegador.

## Compilar

```sh
./build.sh
```

Necessita `clang` amb el target `wasm32` (el d'Apple el porta) i `python3`. Si troba
`wasm-opt` de [binaryen](https://github.com/WebAssembly/binaryen/releases) a
`tools/binaryen/bin`, a `$WASM_OPT` o al PATH, hi aplica Asyncify i el joc funciona a
qualsevol navegador. Sense `wasm-opt` també compila, però llavors la pàgina necessita
WebAssembly JSPI (Chrome o Edge).

| Fitxer | Què fa |
|---|---|
| `tools/prep.py` | Copia `../src` a `build/gen` (noms en minúscules, caràcters DOS com a bytes CP437) i genera `build/unity.c` |
| `port/compat.h` | Tres `#define` perquè el C de Turbo C compili amb clang (`int` de 16 bits, `const`, `main`) |
| `port/platform.c` | El "PC": memòria de vídeo, vectors d'interrupció, rellotge de 18,2 Hz, teclat, altaveu |
| `tools/wasmlink.py` | Converteix l'objecte de clang en un mòdul WebAssembly (fa de `wasm-ld` per a un sol objecte) |
| `tools/checklink.py` | Comprova que el mòdul només demana el que la pàgina li dona |
| `site/machine.js` | Carrega el mòdul, li dona memòria, taula i les funcions `host_*`, i el pausa quan el joc espera (Asyncify o JSPI) |
| `site/textris.js` | Pantalla de text al `<canvas>`, teclat i so |

## Provar sense navegador

```sh
node test.mjs 1500:dump 5000:45 9000:dump    # temps_ms:dump | temps_ms:scancode
node test.mjs --async 5000:45 9000:dump      # pausant el mòdul de debò a cada espera
```

Executa el joc amb un rellotge virtual, prem tecles i imprimeix la pantalla de 80x25.
A la web, `?autostart&keys=5000:45&debug` fa el mateix dins la pàgina, i `?speed=N`
canvia la velocitat de la CPU emulada (blocs bàsics per mil·lisegon, 1500 per defecte).
