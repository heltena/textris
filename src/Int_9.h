/* RSI de la interrupció del teclat. */

#ifndef INT_9_H
#define INT_9_H

#include "acces.h"
#include "estats.h"
#include "gral.h"
#include "interrup.h"
#include "jugador.h"
#include "pantalla.h"
#include "strobe.h"
#include "tecles.h"
#include "tipus.h"

/* Tecles que necesito saber per poder moure les peces */
extern Ttecles tecles_jugador_A, tecles_jugador_B;


/* Estat del joc (MENU_PRAL, REDEFINIR, JOC) */
extern BYTE estat_del_joc;


/* Variable booleana que indica si la interrupció de teclat a canviat */
/* l'estat de joc o a modificat alguna cosa de la pantalla o ... */
extern BYTE RSI_9_ha_canviat;


static void RSI_9 (void);
/* el codi de la interrupció. */


static void (*antic_punter_int_9) (void);
/* antic punter a la interrupció. Cal deixar-lo com estava */


void iniciar_RSI_9 (void);
/* Penja la interrupció al vector. */


void actualitzar_tecles (Ttecles, Ttecles);
/* Tecles dels jugadors (A, B) */


void acabar_RSI_9 (void);
/* desmunta la interrupció del vector posant-hi la direcció antiga. */

#endif
