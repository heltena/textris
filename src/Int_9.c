#include "int_9.h"

/* Vector de tecles dels dos jugadors */
Ttecles tecles_jugador_A, tecles_jugador_B;

/* Tecla que s'ha polsat */
BYTE tecla;

/* Estat actual del joc (MENU_PRAL, REDIFINICIO, JOC, SOBRE_TEXTRIS) */
/* Hi ha una controvèrsia sobre on posar aquesta variable aquí o al */
/* módul principal. La meva resposta es aquí (lògic?) doncs es el punt */
/* crític de la interrupció. */
BYTE estat_del_joc;

/* Variable booleana que indica si la interrupció de teclat ha canviat */
/* l'estat de joc o ha modificat alguna cosa de la pantalla o ... */
BYTE RSI_9_ha_canviat;


void iniciar_RSI_9 () {
 RSI_9_ha_canviat=FALS;
 jugador_A.mov_rotar_esq=FALS; jugador_A.mov_rotar_dre=FALS;
 jugador_A.mov_esquerra=FALS; jugador_A.mov_dreta=FALS;
 jugador_A.mov_baixar=FALS;

 jugador_B.mov_rotar_esq=FALS; jugador_B.mov_rotar_dre=FALS;
 jugador_B.mov_esquerra=FALS; jugador_B.mov_dreta=FALS;
 jugador_B.mov_baixar=FALS;

 antic_punter_int_9=CanviarVectInt (9, RSI_9);
}


void RSI_9 (void) {
 Salvar ();
 tecla=InPort (0x60);
 Strobe (0x61, 7);

 if ( !(tecla & 0x80) ) RSI_9_ha_canviat=CERT;

 if ( (tecla==tecles_jugador_A[TECLA_ON_OFF_SO]) || /* So */
     (tecla==tecles_jugador_B[TECLA_ON_OFF_SO]) )
  hi_ha_so=!hi_ha_so;

 if ( (tecla==tecles_jugador_A[TECLA_ON_OFF_EFESP]) || /* Efectes especials */
     (tecla==tecles_jugador_B[TECLA_ON_OFF_EFESP]) )
  hi_ha_EFESP=!hi_ha_EFESP;

 if (estat_del_joc != ESTAT_SOBRE_TEXTRIS)
  if ( !(tecla & 0x80) ) { /* Make? */
   jugador_A.mov_rotar_esq|=(tecla==tecles_jugador_A[TECLA_ROTAR_ESQ]);
   jugador_A.mov_rotar_dre|=(tecla==tecles_jugador_A[TECLA_ROTAR_DRE]);
   jugador_A.mov_esquerra|=(tecla==tecles_jugador_A[TECLA_ESQUERRA]);
   jugador_A.mov_dreta|=(tecla==tecles_jugador_A[TECLA_DRETA]);
   jugador_A.mov_baixar|=(tecla==tecles_jugador_A[TECLA_BAIXAR]);

   jugador_B.mov_rotar_esq|=(tecla==tecles_jugador_B[TECLA_ROTAR_ESQ]);
   jugador_B.mov_rotar_dre|=(tecla==tecles_jugador_B[TECLA_ROTAR_DRE]);
   jugador_B.mov_esquerra|=(tecla==tecles_jugador_B[TECLA_ESQUERRA]);
   jugador_B.mov_dreta|=(tecla==tecles_jugador_B[TECLA_DRETA]);
   jugador_B.mov_baixar|=(tecla==tecles_jugador_B[TECLA_BAIXAR]);

  /* només permeto activar l'abortar */
   jugador_A.mov_abortar|=(tecla==tecles_jugador_A[TECLA_ABORTAR]);
   jugador_B.mov_abortar|=(tecla==tecles_jugador_B[TECLA_ABORTAR]);
  } else { /* Break? */
   tecla&=0x7F;
   if (tecla==tecles_jugador_A[TECLA_ROTAR_ESQ]) jugador_A.mov_rotar_esq=FALS;
   if (tecla==tecles_jugador_A[TECLA_ROTAR_DRE]) jugador_A.mov_rotar_dre=FALS;
   if (tecla==tecles_jugador_A[TECLA_ESQUERRA]) jugador_A.mov_esquerra=FALS;
   if (tecla==tecles_jugador_A[TECLA_DRETA]) jugador_A.mov_dreta=FALS;
   if (tecla==tecles_jugador_A[TECLA_BAIXAR]) jugador_A.mov_baixar=FALS;

   if (tecla==tecles_jugador_B[TECLA_ROTAR_ESQ]) jugador_B.mov_rotar_esq=FALS;
   if (tecla==tecles_jugador_B[TECLA_ROTAR_DRE]) jugador_B.mov_rotar_dre=FALS;
   if (tecla==tecles_jugador_B[TECLA_ESQUERRA]) jugador_B.mov_esquerra=FALS;
   if (tecla==tecles_jugador_B[TECLA_DRETA]) jugador_B.mov_dreta=FALS;
   if (tecla==tecles_jugador_B[TECLA_BAIXAR]) jugador_B.mov_baixar=FALS;
  }

 EOI ();
 Restaurar ();
}


void actualitzar_tecles (Ttecles jugador_a, Ttecles jugador_b) {
 WORD flags=Inhibir(); /* No vull interrupcions ara */

 copiar_tecles (&tecles_jugador_A, jugador_a);
 copiar_tecles (&tecles_jugador_B, jugador_b);
 Desinhibir (flags);
}


void acabar_RSI_9 () {
 CanviarVectInt (9, antic_punter_int_9);
}
