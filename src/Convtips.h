/**** EC 96/97. Lab. E/S 1: */

/* CONVTIPS.H: rutines de conversió de tipus elementals */

      /* Per impedir múltiples "inclusions" d'aquest fitxer capçalera: */
#ifndef CONVTIPS_H
#define CONVTIPS_H

#include "tipus.h"

      /* Punters, Segments i Desplaçaments */

extern PTR Ptr (WORD Segment, WORD Desplacament);
      /* Converteix el segment i desplacament en un punter */
extern WORD Segment (PTR Adreca);
      /* Retorna el segment del punter donat */
extern WORD Desplacament (PTR Adreca);
      /* Retorna el desplacament del punter donat */

      /* Words, Bytes alts i Bytes baixos */

extern WORD Word (BYTE msb, BYTE lsb);
      /* Retorna un WORD amb part baixa i alta els donats */
extern BYTE MSB (WORD Valor);
      /* Retorna el byte alt del word donat */
extern BYTE LSB (WORD Valor);
      /* Retorna el byte baix del word donat */

#endif
