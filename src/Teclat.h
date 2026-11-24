/****  EC 96/97. Lab. E/S 3: */

/* TECLAT.H: rutines de control del teclat per ENQUESTA */

        /* Per impedir múltiples "inclusions" d'aquest fitxer capçalera: */
#ifndef TECLAT_H
#define TECLAT_H

#include "tipus.h"


extern BYTE LlegirTeclatENQ (void);

        /* Llegeix el teclat per enquesta, esperant que es premi una
           tecla. Només farà cas quan la tecla baixi ("make") i no quan
           pugi ("break"). El codi retornat serà el codi de rastreig
           (scan code). */


extern char ASCII (BYTE ScanCode);

        /* Tradueix a ASCII el codi de rastreig (scan code) que se li
           passa. En cas de no tenir traducció, retornarà el valor 0. */


extern void IniTeclatENQ (void);

        /* Inhibeix les peticions d'interrupció del teclat, per poder
           treballar per enquesta. Cal cridar aquesta rutina una vegada,
           abans de qualsevol crida a LlegirTeclat(). */


extern void FiTeclatENQ (void);

        /* Desinhibeix les peticions d'interrupció del teclat, quan s'ha
           acabat de treballar per enquesta. Cal cridar aquesta rutina
           abans d'acabar el programa, doncs altrament el sistema operatiu
           no podrà llegir el teclat (ho fa per interrupcions: "penjat"). */

#endif
