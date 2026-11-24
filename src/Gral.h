/* Definició dels tipus generals */

#ifndef GRAL_H
#define GRAL_H

#include "tipus.h"


/* Generals */

typedef char ENTER8; /* -128 .. 127 */
typedef int ENTER16; /* -32768 .. 32767 */

#define MATRIU_LONG_X 10  /* Amplada de la matriu de joc */
#define MATRIU_LONG_Y 20  /* Alçada de la matriu de joc */

#define COLOR_NUL 0      /* índex del color nul a la matriu de video */

#define NUL 0            /* valor nul */

#define FALS 0           /* Valor de Fals */
#define CERT !FALS       /* Valor de Cert */

#define NO_PECA 255      /* Valor de no peça (no té peça) */

/* Menú */

#define PECA_INICIAL 5   /* Peça inicial del menú */

#define PECA_JUGAR 5     /* Quina peça indica Començar a jugar */
#define PECA_NIVELL_MES 4 /* Quina peça indica Pujar nivell */
#define PECA_SORTIR 7    /* Quina peça indica Sortir */
#define PECA_NIVELL_MENYS 6 /* Quina peça indica Baixar nivell */
#define MAX_NIVELL 10    /* Nombre màxim de nivells */


#define X_PECA_ESQ 10    /* Posició de la peça al cercle esquerra del menú */
#define X_PECA_DRE 65    /* Posició de la peça al cercle dreta del menú */
#define Y_PECA 9         /* Fila on posem la peça del menú */


/* joc */

#define X_PECA_SEG_A 16  /* Columna on posem la següent peça del jugador A */
#define Y_PECA_SEG_A  7  /* Fila on posem la següent peça del jugador A */

#define X_PECA_SEG_B 20  /* Columna on posem la següent peça del jugador B */
#define Y_PECA_SEG_B 14  /* Fila on posem la següent peça del jugador B */

#define X_GEIM_OVER_A 10 /* Posició on sortirà el cuadre de Geim Over */
#define Y_GEIM_OVER_A 8

#define X_GEIM_OVER_B 58 /* Posició on sortirà el cuadre de Geim Over */
#define Y_GEIM_OVER_B 8

#define X_INICIAL MATRIU_LONG_X/2-2 /* Columna inicial de la nova peça */
#define Y_INICIAL 0      /* Fila inicial de la nova peça */

/* Rellotge */
#define MAX_TICS_JOC 1 /* Permet al jugador canviar la seva peca cada tic */

#define FILA_REL 24    /* Sota de tot */
#define COLUMNA_REL 38 /* En mig */
#define ATR_REL 0x5F   /* Blanc sobre magenta */


#define NUM_LINIES_PASAR 5 /* Número de línies que has de fer per pasar les línies */


/* Variables */

extern char *STRING_TEXTRIS_1; /* Logotip */
extern char *STRING_TEXTRIS_2;
extern char *STRING_TEXTRIS_3;
extern char *STRING_TEXTRIS_4;

extern BYTE hi_ha_so;     /* Els jugador soporten la música? */
extern BYTE hi_ha_EFESP;  /* Els jugador soporten els "efectes especials"? */

extern BYTE num_linies_nou_nivell[MAX_NIVELL]; /* número de línies que s'han */
                                               /* de fer per pasar de nivell */

void Escriure_WORD (BYTE, BYTE, WORD, BYTE);
/* Dibuixa el WORD pasat com a paràmetre a la posició fila x columna */
/* l'últim paràmetre és l'atribut */

#endif
