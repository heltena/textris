#include "tecles.h"

/* Donar nom a les tecles, més que res per poder demanar-les */
const Tnom_tecla nom_tecla = {
  "    Rotar esquerra: ",
  "       Rotar dreta: ",
  "            Baixar: ",
  "           Abortar: ",
  "          Esquerra: ",
  "             Dreta: ",
  "     Act/Desact so: ",
  "  Act/Desact EFESP: " };

const Ttecles tecles_def_jugador_A = {
  /* TECLA_ROTAR_ESQ */    16,  /* Q */
  /* TECLA_ROTAR_DRE */    18,  /* E */
  /* TECLA_BAIXAR */       45,  /* X */
  /* TECLA_ABORTAR */       1,  /* ESC */
  /* TECLA_ESQUERRA */     30,  /* A */
  /* TECLA_DRETA */        32,  /* D */
  /* TECLA_ON_OFF_SO */    24,  /* O */
  /* TECLA_ON_OFF_EFESP */ 23}; /* I */


const Ttecles tecles_def_jugador_B = {
  /* TECLA_ROTAR_ESQ */    71,  /* 7 */
  /* TECLA_ROTAR_DRE */    73,  /* 9 */
  /* TECLA_BAIXAR */       80,  /* 2 */
  /* TECLA_ABORTAR */       1,  /* ESC */
  /* TECLA_ESQUERRA */     75,  /* 4 */
  /* TECLA_DRETA */        77,  /* 6 */
  /* TECLA_ON_OFF_SO */    24,  /* O */
  /* TECLA_ON_OFF_EFESP */ 23}; /* I */


void copiar_tecles (Ttecles *desti, Ttecles origen) {
  BYTE i;

  for (i=0; i<NUMERO_TECLES; i++) (*desti)[i]=origen[i];
}


void demanar_tecles (Ttecles *tecles_a, Ttecles *tecles_b) {
  BYTE i, j, valida, tecla;

  InhibirIRQ (1); /* No permet llegir per teclat mitjançant la seva int. */
  IniTeclatENQ (); /* Llegim per enquesta */
  dibujar_demanar_tecles ();

  /* Posem el que sortirà (el primer el dibuja el següent bucle */
  for (i=1; i<NUMERO_TECLES; i++)
    EscriureString (5+i*2, 7, nom_tecla[i], 0x30);

  /* ara anem preguntant ... */
  for (i=0; i<NUMERO_TECLES; i++) {
    EscriureString (5+i*2, 7, nom_tecla[i], 0x3F); /* pregunta: */
    do {
      tecla=LlegirTeclatENQ (); /* respon */

      valida=CERT;
      /* a veure si es vàlida */
      if ( (i!=TECLA_ABORTAR) && (i!=TECLA_ON_OFF_SO) &&
           (i!=TECLA_ON_OFF_EFESP) ) {
        /* que sigui diferent a les ja definides */
        for (j=0; j<i; j++) valida&=((*tecles_a)[j]!=tecla);

        /* que sigui diferent a les de l'altre jugador */
        for (j=0; j<NUMERO_TECLES; j++) valida&=((*tecles_b)[j]!=tecla);
      }
    } while (!valida);
    /* es vàlida */
    EscriureCar (5+i*2, 30, ASCII(tecla), 0x3E);
    (*tecles_a)[i]=tecla; /* guardem-la */
  }
  /* Ja em llegit totes */

  /* anem per l'altre jugador */
  /* Posem el que sortirà (el primer el dibuja el següent bucle */
  for (i=1; i<NUMERO_TECLES; i++)
    EscriureString (5+i*2, 48, nom_tecla[i], 0x30);

  /* ara anem preguntant ... */
  for (i=0; i<NUMERO_TECLES; i++) {
    EscriureString (5+i*2, 48, nom_tecla[i], 0x3F); /* pregunta: */
    do {
      tecla=LlegirTeclatENQ (); /* respon */

      /* a veure si es vàlida */
      valida=CERT;
      if ( (i!=TECLA_ABORTAR) && (i!=TECLA_ON_OFF_SO) &&
           (i!=TECLA_ON_OFF_EFESP) ) {
        /* que sigui diferent a les ja definides */
        for (j=0; j<i; j++) valida&=((*tecles_b)[j]!=tecla);

        /* que sigui diferent a les de l'altre jugador */
        for (j=0; j<NUMERO_TECLES; j++) valida&=((*tecles_a)[j]!=tecla);
      }
    } while (!valida);
    /* es vàlida */
    EscriureCar (5+i*2, 71, ASCII(tecla), 0x3E);
    (*tecles_b)[i]=tecla; /* guardem-la */
  }
  /* Ja em llegit totes */

  FiTeclatENQ ();
  DesinhibirIRQ (1); /* Permet que interrompi el teclat */
}
