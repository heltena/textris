/* Definició dels tipus de dades que defineixen el vector de peces. */

#ifndef PECES_H
#define PECES_H

#include "gral.h"
#include "matriu.h"
#include "tipus.h"

#define MAX_CUADRES 4  /* Número de cuadres que composen una peça */
#define MAX_PECES 19   /* Peçes que hi han. Per ficar noves peces augmentar */


struct Tcuadre {
  ENTER8 desp_x, desp_y; /* Desplaçament respecte la posició de la peça */
};


typedef struct Tpeca {
          BYTE color; /* Color de la peca (veure Video.h) */
          BYTE seg_peca_rot_esq, /* On a d'anar si vol girar a l'esquerra */
               seg_peca_rot_dre; /* On a d'anar si vol girar a la dreta */
          struct Tcuadre cuadre[MAX_CUADRES];
         } Tvector_peces[MAX_PECES];


extern Tvector_peces vector_peces;


/* Prototips de les funcions que maneguen peces */

void dibujar_peca (BYTE, BYTE, BYTE, Tmatriu);
/* Dibuja la peça pasada com a paràmetre a la posició dita (x, y) */
/* No mira si pot o no pot dibujar-la */
/* La estructura Tmatriu es el panell de joc. Si es NUL es dibuixarà a */
/* pantalla (útil si es vol dibujar la peça següent) */


void esborrar_peca (BYTE, BYTE, BYTE, BYTE);
/* Esborra una peça de la pantalla (a la matriu no te sentit) segons la */
/* posició (x, y). Posa el caracter blanc amb el color pasat com a paràmetre */


BYTE esborrar_linies (Tmatriu, Tmatriu, BYTE, BYTE, BYTE, BYTE);
/* Esborra les línies de la matriu origen amb la peça pasada com a */
/* paràmetre i pasa les línies a l'altre matriu si es pot */


BYTE peca_legal (BYTE, BYTE, BYTE, Tmatriu);
/* Funció booleana que retorna CERT si la peca pasada pot dibujar-se a la */
/* posició dita (x, y) */
/* La estructura Tmatriu ha de ser el panel de joc */

#endif
