# Textris

Textris és un Tetris per a dos jugadors en mode text, escrit el 1997 per a MS-DOS
com a pràctica de l'assignatura Estructura de Computadors (E.T.I.S.) per
Joan Carol Serrat i Helio Tejedor Navarro. Està fet en Turbo C amb algunes rutines
en assemblador x86.

**Juga-hi al navegador: <https://www.heltena.com/textris/>**

El codi font original es va perdre; l'únic que en quedava era la memòria impresa de la
pràctica, que incloïa tots els llistats. Aquest projecte recupera el joc a partir
d'aquell paper: es va escanejar la memòria, es va transcriure el codi i es va adaptar
perquè es pugui jugar en un navegador.

## Què hi ha aquí

- `src/`: el codi recuperat (C i assemblador), tal com estava imprès.
- `web/`: la versió per a navegador. Compila el C original a WebAssembly i substitueix
  l'assemblador per una petita capa que emula la pantalla de text, el teclat, el
  rellotge i l'altaveu del PC. Les instruccions són a `web/README.md`.

## A tenir en compte

El codi ve d'una transcripció, de manera que pot contenir algun error de lectura.
`Strobe.asm` i `Teclat.asm` no eren a la memòria (els proporcionava el professor) i
s'han reescrit a partir de la seva descripció.
