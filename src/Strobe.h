/****  EC 96/97. Lab. E/S 3: */

/* STROBE.H: rutina per fer un strobe genèric */

        /* Per impedir múltiples "inclusions" d'aquest fitxer capçalera: */
#ifndef STROBE_H
#define STROBE_H

#include "tipus.h"

extern void Strobe (WORD AdrPort, BYTE Bit0_7);

        /* Fa el strobe (posar successivament a 1 i a 0) el bit especificat
           pel paràmetre Bit0_7 del port d'E/S indicat. */

#endif
