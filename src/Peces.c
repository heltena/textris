#include "peces.h"


/* Vector de peces del tetris normal (configuracions diferents de figures
   de 4 blocs 4-connectades) */

Tvector_peces vector_peces = {
/* peça  0 */ { 1,  1,  1, { {-1, 2}, {0, 2}, {1, 2}, {2, 2} } },
/* peça  1 */ { 1,  0,  0, { {0, 0}, {0, 1}, {0, 2}, {0, 3} } },

/* peça  2 */ { 2,  3,  3, { {0, 1}, {1, 0}, {1, 1}, {2, 0} } },
/* peça  3 */ { 2,  2,  2, { {0, 0}, {0, 1}, {1, 1}, {1, 2} } },

/* peça  4 */ { 3,  5,  7, { {0, 0}, {0, 1}, {1, 1}, {2, 1} } },
/* peça  5 */ { 3,  6,  4, { {1, 0}, {1, 1}, {1, 2}, {0, 2} } },
/* peça  6 */ { 3,  7,  5, { {0, 0}, {1, 0}, {2, 0}, {2, 1} } },
/* peça  7 */ { 3,  4,  6, { {0, 0}, {1, 0}, {0, 1}, {0, 2} } },

/* peça  8 */ { 4, 11,  9, { {1, 0}, {0, 1}, {1, 1}, {2, 1} } },
/* peça  9 */ { 4,  8, 10, { {0, 0}, {0, 1}, {1, 1}, {0, 2} } },
/* peça 10 */ { 4,  9, 11, { {0, 0}, {1, 0}, {2, 0}, {1, 1} } },
/* peça 11 */ { 4, 10,  8, { {1, 0}, {0, 1}, {1, 1}, {1, 2} } },

/* peça 12 */ { 5, 13, 15, { {2, 0}, {0, 1}, {1, 1}, {2, 1} } },
/* peça 13 */ { 5, 14, 12, { {0, 0}, {1, 0}, {1, 1}, {1, 2} } },
/* peça 14 */ { 5, 15, 13, { {0, 0}, {1, 0}, {2, 0}, {0, 1} } },
/* peça 15 */ { 5, 12, 14, { {0, 0}, {0, 1}, {0, 2}, {1, 2} } },

/* peça 16 */ { 6, 17, 17, { {0, 0}, {1, 0}, {1, 1}, {2, 1} } },
/* peça 17 */ { 6, 16, 16, { {1, 0}, {0, 1}, {1, 1}, {0, 2} } },

/* peça 18 */ { 7, 18, 18, { {0, 0}, {1, 0}, {1, 1}, {0, 1} } } };


void dibujar_peca (BYTE num_peca, BYTE x, BYTE y, Tmatriu matriu)
{
  struct Tpeca peca = vector_peces[num_peca];
  struct Tcolor color=vector_joc[peca.color]; /* Ja tinc el color i l'atribut */
  BYTE i;

  if (matriu==NUL) { /* Dibujar a pantalla la peca */
    for (i=0; i<MAX_CUADRES; i++) { /* Dibujar cada cuadre */
      EscriureCar (y+peca.cuadre[i].desp_y, (x+peca.cuadre[i].desp_x)*2,
                   color.caracter_esq, color.atribut); /* a pantalla Bloc 1 */
      EscriureCar (y+peca.cuadre[i].desp_y, (x+peca.cuadre[i].desp_x)*2+1,
                   color.caracter_dre, color.atribut); /* a pantalla Bloc 2 */
    } /* for */
  } else {
    for (i=0; i<MAX_CUADRES; i++)
      matriu[y+peca.cuadre[i].desp_y][x+peca.cuadre[i].desp_x]=color;
  }
}


void esborrar_peca (BYTE num_peca, BYTE x, BYTE y, BYTE color_fons)
{
  struct Tpeca peca = vector_peces[num_peca];
  BYTE i;

  for (i=0; i<MAX_CUADRES; i++) { /* Esborrar cada cuadre */
    EscriureCar (y+peca.cuadre[i].desp_y, (x+peca.cuadre[i].desp_x)*2,
                 ' ', color_fons); /* a pantalla Bloc 1 */
    EscriureCar (y+peca.cuadre[i].desp_y, (x+peca.cuadre[i].desp_x)*2+1,
                 ' ', color_fons); /* a pantalla Bloc 2 */
  }
}


BYTE esborrar_linies (Tmatriu origen, Tmatriu desti,
                      BYTE num_peca, BYTE x, BYTE y, BYTE pasar) {
  struct Tpeca peca = vector_peces[num_peca];
  Tmatriu auxiliar;
  BYTE i, j, k, l, max_desp_y;
  BYTE linia, num_de_linies=0;

  copiar_matriu (auxiliar, origen);
  dibujar_peca (num_peca, x, y, auxiliar); /* a 'auxiliar' tenim les linies */

  max_desp_y=0;
  for (i=0; i<MAX_CUADRES; i++)
    if (max_desp_y<peca.cuadre[i].desp_y) max_desp_y=peca.cuadre[i].desp_y;
     /* agafem el desplaçament més gran */

  if (y+max_desp_y>MATRIU_LONG_Y) max_desp_y=MATRIU_LONG_Y-y; /* retallem */

  k=1;
  for (j=0; j<max_desp_y+1; j++) { /* màximes línies que es poden fer */
    linia=CERT; /* anem a mirar si la línia y+j està completa */
    for (i=0; i<MATRIU_LONG_X; i++)
       /* La posició y+j , i està en blanc ? */
      if (auxiliar[y+j][i].atribut==color_nul.atribut) linia=FALS;

    if (linia) { /* està completa */
      if (pasar) introduir_linia (desti, origen[y+j]); /* posem línia */

      for (l=y+j; l>0; l--)             /* treiem la línia */
        for (i=0; i<MATRIU_LONG_X; i++)
          if (l-k>=0) origen[l][i]=auxiliar[l-k][i];
          else origen[l][i]=color_nul; /* les línies d'amunt son blancs */

      k++;
      num_de_linies++; /* incrementar el número de linies */
    } else { /* Em de posar el que queda de la peça a la matriu del jugador */
      for (i=0; i<MATRIU_LONG_X; i++)
        origen[y+j][i]=auxiliar[y+j][i];
    }
  }

  return (num_de_linies);
}


BYTE peca_legal (BYTE num_peca, BYTE x, BYTE y, Tmatriu matriu)
{
  struct Tpeca peca = vector_peces[num_peca];
  BYTE i, res=CERT;
  struct Tcolor col_mat;

  for (i=0; i<MAX_CUADRES;i++)
    res&=( ( (x+peca.cuadre[i].desp_x>=0) &&
             (x+peca.cuadre[i].desp_x<MATRIU_LONG_X) )&&
           ( (y+peca.cuadre[i].desp_y>=0) &&
             (y+peca.cuadre[i].desp_y<MATRIU_LONG_Y) ));
  if (!res) return (FALS); /* si ja es ilegal surt, sino, continua ... */

  for (i=0; i<MAX_CUADRES; i++){
    col_mat=matriu[y+peca.cuadre[i].desp_y][x+peca.cuadre[i].desp_x];
    res&=(col_mat.atribut==color_nul.atribut);
  }
  return (res);
}
