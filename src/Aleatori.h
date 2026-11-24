/* Definició de la funció aleatori. Potser no es tan bona com la del C, */
/* però funciona i estem orgullossos d'ella, la nostra filla. */
/* tarararanaaaaa taranarana ranaranraraaaaa */

#ifndef ALEATORI_H
#define ALEATORI_H

#include "int_8.h" /* per portar el temps, Mem[0x40:0x6C] no funciona */
#include "tipus.h"


WORD aleatori (WORD);
/* Retorna un número comprés entre 0 i 'rang'-1 sent aquest rang el número */
/* pasat com a paràmetre */

#endif
