/****  EC 96/97. Lab. E/S 2: */

/* PANTALLA.H: rutines de control de pantalla */

        /* Per impedir múltiples "inclusions" d'aquest fitxer capçalera: */
#ifndef PANTALLA_H
#define PANTALLA_H
#include "tipus.h"


extern PTR BufferPantalla (void);

        /* Retorna l'adreça on està mapejada la pantalla. Emmagatzema
           internament aquesta informació per ser usada per les rutines
           posteriors. En cas que el mode de pantalla no sigui reconegut
           retorna un valor que indica que s'ha produït un error. */

extern void EscriureCar (BYTE fila, BYTE columna, char caracter, BYTE atribut);

        /* Escriu a pantalla el caràcter que se li passa, amb l'atribut
           indicat, a les coordenades especificades. La informació d'on
           es troba mapejada la pantalla l'obté de la variable que ha
           inicialitzat la rutina BufferPantalla(). */

extern void EscriureString (BYTE fila, BYTE columna, char * string, BYTE atribut);

        /* Escriu a pantalla la cadena de caràcters que se li passa, a
           partir de la posició donada, i amb el mateix atribut per tots
           els caràcters. La seva implementació utilitza instruccions de
           tractament de strings de l'i8086 (LODSB i STOSW). La informació
           d'on es troba mapejada la pantalla l'obté de la variable que ha
           inicialitzat la rutina BufferPantalla(). */

extern void EsborrarPantalla (BYTE atribut);

        /* Omple tota la pantalla de caràcters blancs (espai, codi ASCII
           32) amb l'atribut que se li indica. També fa servir instruccions
           de tractament de strings (concretament, REP STOSW). La informació
           d'on es troba mapejada la pantalla l'obté de la variable que ha
           inicialitzat la rutina BufferPantalla().*/

extern WORD QuinCarAtr (BYTE fila, BYTE columna);

        /* Accedeix a la posició de pantalla indicada i retorna la parella
           (caràcter, atribut) que es troba en aquella posició, el caràcter
           a la part baixa (LSB) del resultat i l'atribut a la part alta
           (MSB). La informació d'on es troba mapejada la pantalla l'obté
           de la variable que ha inicialitzat la rutina BufferPantalla(). */

#endif
