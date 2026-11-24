#include "matriu.h"


Tmatriu matriu_nivell[MAX_NIVELL];


void iniciar_matriu (Tmatriu matriu) {
 BYTE i, j;

 for (j=0; j<MATRIU_LONG_Y; j++)
  for (i=0; i<MATRIU_LONG_X; i++)
   matriu[j][i]=color_nul;
}


void iniciar_matriu_nivells (void) {
 BYTE nivell, i, j;

 for (nivell=0; nivell<MAX_NIVELL; nivell++)
  iniciar_matriu (matriu_nivell[nivell]); /* totes a blancs */

/* nivell ú: tots blancs */

/* nivell dos: punts */
 matriu_nivell[1][MATRIU_LONG_Y-2][3]=vector_joc[1];
 matriu_nivell[1][MATRIU_LONG_Y-2][7]=vector_joc[2];
 matriu_nivell[1][MATRIU_LONG_Y-3][5]=vector_joc[3];
 matriu_nivell[1][MATRIU_LONG_Y-3][4]=vector_joc[3];
 matriu_nivell[1][MATRIU_LONG_Y-4][2]=vector_joc[4];
 matriu_nivell[1][MATRIU_LONG_Y-4][8]=vector_joc[5];

/* nivell tres: piràmide */
 /* primera línia */
 matriu_nivell[2][MATRIU_LONG_Y-1][0]=vector_joc[1];
 matriu_nivell[2][MATRIU_LONG_Y-1][1]=vector_joc[2];
 matriu_nivell[2][MATRIU_LONG_Y-1][2]=vector_joc[3];
 matriu_nivell[2][MATRIU_LONG_Y-1][4]=vector_joc[4];
 matriu_nivell[2][MATRIU_LONG_Y-1][5]=vector_joc[4];
 matriu_nivell[2][MATRIU_LONG_Y-1][7]=vector_joc[3];
 matriu_nivell[2][MATRIU_LONG_Y-1][8]=vector_joc[2];
 matriu_nivell[2][MATRIU_LONG_Y-1][9]=vector_joc[1];

 /* segona línia */
 matriu_nivell[2][MATRIU_LONG_Y-2][1]=vector_joc[5];
 matriu_nivell[2][MATRIU_LONG_Y-2][2]=vector_joc[4];
 matriu_nivell[2][MATRIU_LONG_Y-2][3]=vector_joc[3];

 matriu_nivell[2][MATRIU_LONG_Y-2][6]=vector_joc[3];
 matriu_nivell[2][MATRIU_LONG_Y-2][7]=vector_joc[4];
 matriu_nivell[2][MATRIU_LONG_Y-2][8]=vector_joc[5];

 /* tercera línia */
 matriu_nivell[2][MATRIU_LONG_Y-3][2]=vector_joc[6];
 matriu_nivell[2][MATRIU_LONG_Y-3][3]=vector_joc[4];
 matriu_nivell[2][MATRIU_LONG_Y-3][4]=vector_joc[3];
 matriu_nivell[2][MATRIU_LONG_Y-3][5]=vector_joc[3];
 matriu_nivell[2][MATRIU_LONG_Y-3][6]=vector_joc[4];
 matriu_nivell[2][MATRIU_LONG_Y-3][7]=vector_joc[6];

 /* quarta línia */
 matriu_nivell[2][MATRIU_LONG_Y-4][3]=vector_joc[1];
 matriu_nivell[2][MATRIU_LONG_Y-4][4]=vector_joc[3];
 matriu_nivell[2][MATRIU_LONG_Y-4][5]=vector_joc[3];
 matriu_nivell[2][MATRIU_LONG_Y-4][6]=vector_joc[1];

 /* quinta línia */
 matriu_nivell[2][MATRIU_LONG_Y-5][4]=vector_joc[2];
 matriu_nivell[2][MATRIU_LONG_Y-5][5]=vector_joc[2];

/* nivell quatre: E C */
 matriu_nivell[3][MATRIU_LONG_Y-5][1]=vector_joc[5];
 matriu_nivell[3][MATRIU_LONG_Y-5][2]=vector_joc[5];
 matriu_nivell[3][MATRIU_LONG_Y-5][7]=vector_joc[5];
 matriu_nivell[3][MATRIU_LONG_Y-5][8]=vector_joc[5];

 matriu_nivell[3][MATRIU_LONG_Y-4][0]=vector_joc[4];
 matriu_nivell[3][MATRIU_LONG_Y-4][3]=vector_joc[4];
 matriu_nivell[3][MATRIU_LONG_Y-4][6]=vector_joc[4];
 matriu_nivell[3][MATRIU_LONG_Y-4][9]=vector_joc[4];

 matriu_nivell[3][MATRIU_LONG_Y-3][1]=vector_joc[3];
 matriu_nivell[3][MATRIU_LONG_Y-3][6]=vector_joc[3];

 matriu_nivell[3][MATRIU_LONG_Y-2][0]=vector_joc[2];
 matriu_nivell[3][MATRIU_LONG_Y-2][3]=vector_joc[2];
 matriu_nivell[3][MATRIU_LONG_Y-2][6]=vector_joc[2];
 matriu_nivell[3][MATRIU_LONG_Y-2][9]=vector_joc[2];

 matriu_nivell[3][MATRIU_LONG_Y-1][1]=vector_joc[1];
 matriu_nivell[3][MATRIU_LONG_Y-1][2]=vector_joc[1];
 matriu_nivell[3][MATRIU_LONG_Y-1][7]=vector_joc[1];
 matriu_nivell[3][MATRIU_LONG_Y-1][8]=vector_joc[1];

/* nivell cinquè: plaques als costats */
 matriu_nivell[4][MATRIU_LONG_Y-6][0]=vector_joc[6];
 matriu_nivell[4][MATRIU_LONG_Y-6][9]=vector_joc[6];

 matriu_nivell[4][MATRIU_LONG_Y-5][0]=vector_joc[5];
 matriu_nivell[4][MATRIU_LONG_Y-5][9]=vector_joc[5];

 matriu_nivell[4][MATRIU_LONG_Y-4][0]=vector_joc[4];
 matriu_nivell[4][MATRIU_LONG_Y-4][9]=vector_joc[4];

 matriu_nivell[4][MATRIU_LONG_Y-3][0]=vector_joc[3];
 matriu_nivell[4][MATRIU_LONG_Y-3][9]=vector_joc[3];

 matriu_nivell[4][MATRIU_LONG_Y-2][0]=vector_joc[2];
 matriu_nivell[4][MATRIU_LONG_Y-2][9]=vector_joc[2];

 matriu_nivell[4][MATRIU_LONG_Y-1][0]=vector_joc[1];
 matriu_nivell[4][MATRIU_LONG_Y-1][9]=vector_joc[1];

/* nivell sisè: */
 matriu_nivell[5][MATRIU_LONG_Y-6][5]=vector_joc[1];
 matriu_nivell[5][MATRIU_LONG_Y-6][6]=vector_joc[2];
 matriu_nivell[5][MATRIU_LONG_Y-6][7]=vector_joc[3];
 matriu_nivell[5][MATRIU_LONG_Y-6][8]=vector_joc[4];
 matriu_nivell[5][MATRIU_LONG_Y-6][9]=vector_joc[5];

 matriu_nivell[5][MATRIU_LONG_Y-5][5]=vector_joc[2];
 matriu_nivell[5][MATRIU_LONG_Y-4][5]=vector_joc[3];
 matriu_nivell[5][MATRIU_LONG_Y-3][5]=vector_joc[4];
 matriu_nivell[5][MATRIU_LONG_Y-2][5]=vector_joc[5];
 matriu_nivell[5][MATRIU_LONG_Y-1][5]=vector_joc[6];

/* nivell setè: línia orfa */
 matriu_nivell[6][MATRIU_LONG_Y-7][1]=vector_joc[1];
 matriu_nivell[6][MATRIU_LONG_Y-7][2]=vector_joc[2];
 matriu_nivell[6][MATRIU_LONG_Y-7][3]=vector_joc[3];
 matriu_nivell[6][MATRIU_LONG_Y-7][4]=vector_joc[4];
 matriu_nivell[6][MATRIU_LONG_Y-7][5]=vector_joc[4];
 matriu_nivell[6][MATRIU_LONG_Y-7][6]=vector_joc[3];
 matriu_nivell[6][MATRIU_LONG_Y-7][7]=vector_joc[2];
 matriu_nivell[6][MATRIU_LONG_Y-7][8]=vector_joc[1];

/* nivell vuitè: ska */
 matriu_nivell[7][MATRIU_LONG_Y-4][1]=vector_joc[1];
 matriu_nivell[7][MATRIU_LONG_Y-4][3]=vector_joc[2];
 matriu_nivell[7][MATRIU_LONG_Y-4][5]=vector_joc[3];
 matriu_nivell[7][MATRIU_LONG_Y-4][7]=vector_joc[4];
 matriu_nivell[7][MATRIU_LONG_Y-4][9]=vector_joc[5];

 matriu_nivell[7][MATRIU_LONG_Y-5][0]=vector_joc[5];
 matriu_nivell[7][MATRIU_LONG_Y-5][2]=vector_joc[4];
 matriu_nivell[7][MATRIU_LONG_Y-5][4]=vector_joc[3];
 matriu_nivell[7][MATRIU_LONG_Y-5][6]=vector_joc[2];
 matriu_nivell[7][MATRIU_LONG_Y-5][8]=vector_joc[1];

 matriu_nivell[7][MATRIU_LONG_Y-6][1]=vector_joc[1];
 matriu_nivell[7][MATRIU_LONG_Y-6][3]=vector_joc[2];
 matriu_nivell[7][MATRIU_LONG_Y-6][5]=vector_joc[3];
 matriu_nivell[7][MATRIU_LONG_Y-6][7]=vector_joc[4];
 matriu_nivell[7][MATRIU_LONG_Y-6][9]=vector_joc[5];

/* nivell novè: ratxola de xocolata */
 matriu_nivell[8][MATRIU_LONG_Y-2][0]=vector_joc[3];
 matriu_nivell[8][MATRIU_LONG_Y-2][2]=vector_joc[5];
 matriu_nivell[8][MATRIU_LONG_Y-2][4]=vector_joc[3];
 matriu_nivell[8][MATRIU_LONG_Y-2][6]=vector_joc[5];
 matriu_nivell[8][MATRIU_LONG_Y-2][8]=vector_joc[3];

 matriu_nivell[8][MATRIU_LONG_Y-4][0]=vector_joc[2];
 matriu_nivell[8][MATRIU_LONG_Y-4][2]=vector_joc[4];
 matriu_nivell[8][MATRIU_LONG_Y-4][4]=vector_joc[2];
 matriu_nivell[8][MATRIU_LONG_Y-4][6]=vector_joc[4];
 matriu_nivell[8][MATRIU_LONG_Y-4][8]=vector_joc[2];

 matriu_nivell[8][MATRIU_LONG_Y-6][0]=vector_joc[1];
 matriu_nivell[8][MATRIU_LONG_Y-6][2]=vector_joc[3];
 matriu_nivell[8][MATRIU_LONG_Y-6][4]=vector_joc[1];
 matriu_nivell[8][MATRIU_LONG_Y-6][6]=vector_joc[3];
 matriu_nivell[8][MATRIU_LONG_Y-6][8]=vector_joc[1];

/* nivell decè: X */
 matriu_nivell[9][MATRIU_LONG_Y-9][1]=vector_joc[1];
 matriu_nivell[9][MATRIU_LONG_Y-9][9]=vector_joc[1];

 matriu_nivell[9][MATRIU_LONG_Y-8][2]=vector_joc[2];
 matriu_nivell[9][MATRIU_LONG_Y-8][8]=vector_joc[2];

 matriu_nivell[9][MATRIU_LONG_Y-7][3]=vector_joc[3];
 matriu_nivell[9][MATRIU_LONG_Y-7][7]=vector_joc[3];

 matriu_nivell[9][MATRIU_LONG_Y-6][4]=vector_joc[4];
 matriu_nivell[9][MATRIU_LONG_Y-6][6]=vector_joc[4];

 matriu_nivell[9][MATRIU_LONG_Y-5][5]=vector_joc[5];

 matriu_nivell[9][MATRIU_LONG_Y-4][4]=vector_joc[4];
 matriu_nivell[9][MATRIU_LONG_Y-4][6]=vector_joc[4];

 matriu_nivell[9][MATRIU_LONG_Y-3][3]=vector_joc[3];
 matriu_nivell[9][MATRIU_LONG_Y-3][7]=vector_joc[3];

 matriu_nivell[9][MATRIU_LONG_Y-2][2]=vector_joc[2];
 matriu_nivell[9][MATRIU_LONG_Y-2][8]=vector_joc[2];

 matriu_nivell[9][MATRIU_LONG_Y-1][1]=vector_joc[1];
 matriu_nivell[9][MATRIU_LONG_Y-1][9]=vector_joc[1];
}


void dibujar_matriu (Tmatriu matriu, BYTE x, BYTE y) {
 BYTE i, j;

 for (j=0; j<MATRIU_LONG_Y; j++)
  for (i=0; i<MATRIU_LONG_X; i++) {
   EscriureCar (y+j, (x+i)*2, matriu[j][i].caracter_esq,
         matriu[j][i].atribut);
   EscriureCar (y+j, (x+i)*2+1, matriu[j][i].caracter_dre,
         matriu[j][i].atribut);
  }
}


void copiar_matriu (Tmatriu desti, Tmatriu origen) {
 BYTE i, j;

 for (j=0; j<MATRIU_LONG_Y; j++)
  for (i=0; i<MATRIU_LONG_X; i++)
   desti[j][i]=origen[j][i];
}


void introduir_linia (Tmatriu matriu, struct Tcolor linia[MATRIU_LONG_X]) {
 BYTE i, j;

 for (j=1; j<MATRIU_LONG_Y; j++)
  for (i=0; i<MATRIU_LONG_X; i++)
   matriu[j-1][i]=matriu[j][i];
 for (i=0; i<MATRIU_LONG_X; i++) matriu[MATRIU_LONG_Y-1][i]=linia[i];
}
