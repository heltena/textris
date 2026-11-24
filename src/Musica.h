/* Definició dels tipus i funcions que formen la part "musical" del textris */

#ifndef MUSICA_H
#define MUSICA_H

#include "altaveu.h"
#include "gral.h"
#include "interrup.h"

#define MAX_NOTES 60      /* notes que pot tenir una cançó */
#define TO -200
#define SEMI_TO -100
#define OCTAVA_NORMAL 2000
#define DO -10*TO + OCTAVA_NORMAL
#define RE  -9*TO + OCTAVA_NORMAL
#define MI  -8*TO + OCTAVA_NORMAL
#define FA  -7*TO + OCTAVA_NORMAL
#define SOL -6*TO + OCTAVA_NORMAL
#define LA  -5*TO + OCTAVA_NORMAL
#define SI  -4*TO + OCTAVA_NORMAL
#define OCTAVA -1400

struct Tmusica {
 BYTE repetir,          /* FALS no repeteix, CERT realitza un bucle inf. */
     pausa,
     notes,          /* notes que te la cançó */
     nota_actual,       /* índex del vector de cançó que indica la nota */
     duracio_so_actual,  /* nombre de tics que porta el so actual */
     duracio_so_parcial, /* nombre de tics totals que ha de fer la nota actual */
     duracio_so_total;   /* nombre de tics totals que ha de fer la nota actual amb pausa inclosa */
 struct Tnota {
  BYTE duracio, parcial;
  WORD nota;    /* duració i nota d'una de les notes de la cançó */
 } canco [MAX_NOTES];    /* la cançó */
};

/* la popular cançó del tirorirorito */
extern struct Tmusica musica_tetris;

/* la música que fa el moure una peca */
extern struct Tmusica musica_moure_peca;

/* la música que sona quan es fan 'n' línies */
extern struct Tmusica musica_esborrar_una, musica_esborrar_dos,
                      musica_esborrar_tres, musica_esborrar_textris;

void iniciar_canco (struct Tmusica *, BYTE);
/* només inicialitza les variables que necesita la ISR. */
/* el segon paràmetre indica si ha de sonar (CERT) o no ha de sonar (FALS) */


void canviar_pausa (struct Tmusica *);
/* Si havia pausa la treu, en cas contrari la posa */


void posar_pausa (struct Tmusica *);
/* posa la pausa a la cançó */


void treure_pausa (struct Tmusica *);
/* treu la pausa a la cançó */


BYTE actualitzar_canco (struct Tmusica *);
/* es crida cada pols de rellotge. actualitza la cançó pasada. */
/* retorna CERT si la cançó està sonant, FALS si ja no sona. */


#endif
