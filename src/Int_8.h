/* RSI de la interrupció del rellotge. */

#ifndef INT_8_H
#define INT_8_H

#include "gral.h"
#include "interrup.h"
#include "musica.h"
#include "tipus.h"


/* Variables que porta els tics del nivell */
extern BYTE tics_nivell, tics_joc, tics_rellotge, tics_cinc_segons;

extern BYTE nivell_RSI_8; /* nivell del joc */

/* Variables del rellotge que surt per pantalla */
extern BYTE hi_ha_rellotge;
extern BYTE minuts, segons;

extern BYTE cinc_segons;

/* Variable que varia de CERT a FALS cada mig segon */
extern BYTE panpalluga;

/* Variable de aleatori */
extern WORD valor_aleatori_RSI_8;

/* Variables booleanes que indica si la interrupció de rellotge a baixat */
/* la peça i si permet actualitzar les peces dels jugadors */
extern BYTE RSI_8_fa_baixar, RSI_8_permet_canviar;


static void RSI_8 (void);
/* el codi de la interrupció. */


static void (*antic_punter_int_8) (void);
/* antic punter a la interrupció. Cal deixar-lo com estava */


void iniciar_RSI_8 (BYTE);
/* inicialitza les cançons que utilitza i penja la interrupció al vector. */
/* Pel paràmetre passem el nivell inicial. */


void acabar_RSI_8 (void);
/* desmunta la interrupció del vector posant-hi la direcció antiga. */


void canviar_nivell (BYTE);
/* Canviar el nivell de la RSI. ALERTA!!! hem de canviar aixó cada cop que */
/* canviem el nivell del módul pral. */

#endif
