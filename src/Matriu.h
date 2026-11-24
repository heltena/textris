/* Definició dels tipus de dades que defineixen la matriu de joc. */

#ifndef MATRIU_H
#define MATRIU_H

#include "gral.h" /* COLOR_NUL */
#include "pantalla.h"
#include "tipus.h"
#include "video.h" /* struct Tcolor */


typedef struct Tcolor Tmatriu [MATRIU_LONG_Y][MATRIU_LONG_X];
 /* A cada posició el número de color */


extern Tmatriu matriu_nivell[MAX_NIVELL]; /* matriu inicial de cada nivell */

/* Prototips de les funcions que maneguen matrius */

void iniciar_matriu (Tmatriu);
/* Inicialitza la matriu amb COLOR_NUL (gral.h) */


void iniciar_matriu_nivells (void);
/* Inicialitza totes les matrius que formen les pantalles del joc (nivell) */


void dibujar_matriu (Tmatriu, BYTE, BYTE);
/* Dibuixa la matriu a la posició (x, y) de la pantalla */
/* No mira la correctessa de la posició */


void copiar_matriu (Tmatriu, Tmatriu);
/* Copia la matriu origen a destí */


void introduir_linia (Tmatriu, struct Tcolor [MATRIU_LONG_X]);
/* introdueix a la última fila de la matriu la línia pasada com a paràmetre*/

#endif
