/* Definició del tipus Tjugador. */

#ifndef JUGADOR_H
#define JUGADOR_H

#include "matriu.h"
#include "peces.h"
#include "tecles.h"

struct Tjugador {
 Tmatriu matriu_joc;
 ENTER8 x, y; /* posició a la matriu */
 BYTE peca_anterior, peca_actual, peca_seguent;
 BYTE peca_ant_x, peca_ant_y, /* posició relativa a la matriu */
     peca_act_x, peca_act_y, /* posició relativa a la matriu */
     peca_seg_x, peca_seg_y; /* posició relativa a LA PANTALLA */

 BYTE mov_rotar_esq, mov_rotar_dre, /* Variables que donen l'estat actual */
     mov_esquerra, mov_dreta, mov_baixar,
     mov_abortar, sortir, canvi_peca, pasar, mort, espera;

 BYTE nivell;
 WORD punts, /* punts que has fet */
     linies; /* línies que porta. Una fantasmada WORD, pero hi ha gent */
                    /* que fa més de 255 i ... com està tant bé fet, doncs... */
 BYTE linies_queden; /* línies que queden per pasar de nivell */

 Ttecles tecles;
};

extern struct Tjugador jugador_A, jugador_B;

#endif
