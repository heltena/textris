/**** EC 96/97. Lab. E/S 2: */

/* ALTAVEU.H: rutines de control d'altaveu */

      /* Per impedir múltiples "inclusions" d'aquest fitxer capçalera: */
#ifndef ALTAVEU_H
#define ALTAVEU_H

#include "tipus.h"

extern void So (WORD frequencia);

      /* Programa el timer que genera la freqüència per l'altaveu
         amb el valor que se li passa com a paràmetre i retorna.
         D'aquesta forma l'altaveu queda sonant després de retornar.
         En el cas de que el paràmetre sigui 0 ha d'aturar l'altaveu. */

#endif
