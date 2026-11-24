/**** EC 96/97. Lab. E/S 4: */

/* INTERRUP.H:
      rutines vàries per utilitzar interrupcions des de programes en "C" */

      /* Per impedir múltiples "inclusions" d'aquest fitxer capçalera: */
#ifndef INTERRUP_H
#define INTERRUP_H

#include "tipus.h"


extern WORD Inhibir (void);

      /* Inhibeix les interrupcions (posa IF=0), retornant el valor de la
         paraula d'estat (flags) abans d'inhibir. Per llegir els flags és
         útil la instrucció PUSHF. */


extern void Desinhibir (WORD Flags);

      /* Carrega el valor passat com a paràmetre com a nou valor de la
         paraula d'estat (flags). Per escriure els flags podem s'usarà
         la instrucció POPF. */


extern void EOI (void);

      /* Envia al controlador d'interrupcions (i8259) una comanda de final
         d'interrupció no específica (EOI). */


extern void Salvar (void);

      /* Aquesta rutina és la primera que es cridarà dintre d'una rutina
         de servei d'interrupció (RSI) escrita en "C", i s'ha d'encarregar
         de guardar a la pila el valor de tots els registres del processa-
         dor, per tal de poder recuperar després els valors originals,
         donat que el codi en "C" de la RSI els haurà modificat. També
         inicialitzarà el registre DS per que apunti al segment de dades
         correcte. */


extern void Restaurar (void);

      /* Aquesta rutina és la complementària de l'anterior, i és l' última
         que es cridarà dintre de qualsevol RSI escrita en "C". La seva
         funció és treure de la pila els registres guardats per la rutina
         Salvar, eliminar les possibles variables locals i retornar de la
         RSI (amb IRET). */


extern void (*CanviarVectInt (BYTE NumInt, void (*NovaRSI)() ))();

      /* Assigna una nova rutina de servei d'interrupció a una de les posi-
         cions del vector d'interrupcions (V.I.), retornant l'adreça de
         l'anterior rutina de servei, per tal de restaurar-la més endavant.
         Quan s'estigui accedint al Vector d'Interrupcions les interrupcions
         han d'estar inhibides, deixant-les després en el mateix estat (inhi-
         bides o no) que quan es va cridar aquesta rutina. */


extern void InhibirIRQ (BYTE irq);

      /* Accedeix al registre de màscara d'interrupcions del 8259 per inhi-
         bir la interrupció corresponent a la línia IRQ que se li indica al
         paràmetre (valor esperat entre 0 i 7). */


extern void DesinhibirIRQ (BYTE irq);

      /* Accedeix al registre de màscara d'interrupcions del 8259 per desin-
         hibir la interrupció corresponent a la línia IRQ que se li indica al
         paràmetre (valor esperat entre 0 i 7). */

#endif
