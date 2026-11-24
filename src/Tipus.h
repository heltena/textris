/**** EC 96/97. Lab. E/S 1: */

/* TIPUS.H:     definició de tipus equivalents */

    /* Per impedir múltiples "inclusions" d'aquest fitxer capçalera: */
#ifndef TIPUS_H
#define TIPUS_H
    /* Comprovació de que es fa servir el model de memòria correcte: */
#ifndef __LARGE__
#error Aquest mòdul només és utilitzable amb el model LARGE !!!
#endif

typedef unsigned int    WORD;   /* Natural de 16 bits */
typedef unsigned char   BYTE;   /* Natural de  8 bits */
typedef void *          PTR;    /* Punter genèric (far) */

#endif
