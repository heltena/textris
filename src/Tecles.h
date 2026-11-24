/* Definició de les tecles del joc */

#ifndef TECLES_H
#define TECLES_H

#include "present.h"
#include "tipus.h"

/* Definició de tecles */
#define NUMERO_TECLES 8
#define TECLA_ROTAR_ESQ 0
#define TECLA_ROTAR_DRE 1
#define TECLA_BAIXAR 2
#define TECLA_ABORTAR 3
#define TECLA_ESQUERRA 4
#define TECLA_DRETA 5
#define TECLA_ON_OFF_SO 6
#define TECLA_ON_OFF_EFESP 7 /* Es llegeix EFectes ESPecials com l'anglés FX */

typedef BYTE Ttecles[NUMERO_TECLES];
typedef char *Tnom_tecla[NUMERO_TECLES];

extern Ttecles tecles_def_jugador_A;
extern Ttecles tecles_def_jugador_B;
extern Tnom_tecla nom_tecla;

void copiar_tecles (Ttecles *, Ttecles);
/* Copia les tecles d'origen al destí (destí, origen) */

#endif
