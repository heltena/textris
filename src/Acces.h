/**** EC 96/97. Lab. E/S 1: */

/* ACCES.H: rutines d'accés a E/S i Memòria definides a ACCES.ASM */

      /* Per impedir múltiples "inclusions" d'aquest fitxer capçalera: */
#ifndef RACCES_H
#define RACCES_H

#include "tipus.h"

      /* Accés a memòria */

extern BYTE Peek (PTR Adr);
      /* Llegeix el byte contingut a "Adr" */
extern void Poke (PTR Adr, BYTE Valor);
      /* Escriu a "Adr" el byte "Valor" */

      /* Accés a ports d'Entrada/Sortida */

extern BYTE InPort (WORD Port);
      /* Llegeix un byte del port indicat */
extern void OutPort (WORD Port, BYTE Valor);
      /* Escriu a "Port" el byte "Valor" */
#endif
