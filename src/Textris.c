/************************* Módul principal del joc. *************************\
|*                                                                          *|
|* TExTRIS:                                                                 *|
|*  Autors: Helio Tejedor Navarro                                           *|
|*          Joan Carol Serrat                                               *|
|*  Estudi: E.T.I.S. Estructura de computadors. URV.                        *|
|*                                                                          *|
\****************************************************************************/

#include "aleatori.h"
#include "altaveu.h"
#include "estats.h"
#include "gral.h"
#include "int_8.h"
#include "int_9.h"
#include "joc.h"
#include "jugador.h"
#include "musica.h"
#include "pantalla.h"
#include "peces.h"
#include "present.h"
#include "teclat.h"
#include "tecles.h"


BYTE iniciar_textris (void);
  /* inicialitza totes les variables, veu si es pot accedir a pantalla i */
  /* enllaça les RSI's corresponents. */


void acabar_textris (void);
  /* desenllaça les RSI's corresponents. S'acomiada */


BYTE main (void) {
  BYTE i, nivell=1, nivell_anterior=-1,
       peca_actual=PECA_INICIAL, peca_anterior=NO_PECA,
       pantalla_nova=CERT, x_peca=X_PECA_ESQ, y_peca=Y_PECA,
       x_peca_ant, y_peca_ant;
  WORD j;

  BYTE continuar_partida=FALS, comencar_jugar=FALS, baixar_nivell=FALS,
       sortir=FALS, error=FALS, pujar_nivell=FALS, redefinir_tecles=FALS,
       sobre_textris=FALS;

  BYTE hi_ha_partida=FALS; /* Estan jugant una partida actualment? */

  void iniciar_jugadors (BYTE);


  if (!iniciar_textris ()) return (-1);

  dibujar_inici (); /* dibujem presentació */
  cinc_segons=0;    /* i esperem tres segons */
  while (cinc_segons < 3) ;

  do {
    if (pantalla_nova) dibujar_presentacio ();

    /* si hem canviat la peça, l'esborrem abans de dibujar la nova */
    if ( ( (peca_anterior!=NO_PECA) && (peca_anterior!=peca_actual) ) ||
        ( x_peca_ant != x_peca) )
      esborrar_peca (peca_anterior, x_peca_ant, y_peca_ant, 0x1F);

    /* dibujem la nova peça */
    if ( (peca_anterior!=peca_actual) || pantalla_nova ||
        (x_peca_ant != x_peca) )
      dibujar_peca (peca_actual, x_peca, y_peca, NUL);

    /* si em canviat de nivell el dibujem */
    if ( (nivell_anterior!=nivell) || pantalla_nova )
      dibujar_nivell (nivell);

    /* actualitzem les variables "anteriors" */
    pantalla_nova=FALS;
    x_peca_ant=x_peca; y_peca_ant=y_peca;
    peca_anterior=peca_actual;
    nivell_anterior=nivell;

    /* Esperem que ens arrivi una interrupció del teclat */
    while (!RSI_9_ha_canviat) ;
    RSI_9_ha_canviat=FALS;

    /* algún jugador prem rotar esquerra ? */
    if ( jugador_A.mov_rotar_esq || jugador_B.mov_rotar_esq )
      peca_actual=vector_peces[peca_actual].seg_peca_rot_esq;

    /* algún jugador prem rotar dreta ? */
    if ( jugador_A.mov_rotar_dre || jugador_B.mov_rotar_dre )
      peca_actual=vector_peces[peca_actual].seg_peca_rot_dre;

    /* algún jugador prem dreta ? */
    if ( jugador_A.mov_dreta || jugador_B.mov_dreta )
      x_peca=X_PECA_DRE;

    /* algún jugador prem esquerra ? */
    if ( jugador_A.mov_esquerra || jugador_B.mov_esquerra )
      x_peca=X_PECA_ESQ;

    /* algún jugador prem abortar ? */
    if ( jugador_A.mov_abortar || jugador_B.mov_abortar ) {
      peca_actual=7; x_peca=X_PECA_ESQ;
    }

    /* algún jugador prem baixar (accionar) ? */
    if ( jugador_A.mov_baixar || jugador_B.mov_baixar )
      switch (x_peca) {
        case X_PECA_ESQ: /* Cercle de la esquerra ? */
          switch (peca_actual) {
            case 4: continuar_partida=CERT; break;
            case 5: comencar_jugar=CERT; break;
            case 6: baixar_nivell=CERT; break;
            case 7: sortir=CERT; break;
            default: error=CERT; break; /* imposible !!! */
          }; break;
        case X_PECA_DRE: /* Cercle de la dreta ? */
          switch (peca_actual) {
            case 4: pujar_nivell=CERT; break;
            case 5: redefinir_tecles=CERT; break;
            case 6: continuar_partida=CERT; break;
            case 7: sobre_textris=CERT; break;
            default: error=CERT; break; /* imposible !!! */
          }
        default: break; /* imposible !!! */
      }

    jugador_A.mov_rotar_esq=jugador_B.mov_rotar_esq=FALS;
    jugador_A.mov_rotar_dre=jugador_B.mov_rotar_dre=FALS;
    jugador_A.mov_dreta=jugador_B.mov_dreta=FALS;
    jugador_A.mov_esquerra=jugador_B.mov_esquerra=FALS;
    jugador_A.mov_abortar=jugador_B.mov_abortar=FALS;
    jugador_A.mov_baixar=jugador_B.mov_baixar=FALS;

    if ( continuar_partida && hi_ha_partida ) {
      /* aquí no canviem el nivell de la RSI_8 */
      jugar (&hi_ha_partida);
      pantalla_nova=CERT;
      continuar_partida=FALS;
    }

    if ( comencar_jugar ) {
      iniciar_jugadors (nivell);
      segons=0; minuts=0;
      canviar_nivell (nivell); /* de la RSI_8 */
      jugar (&hi_ha_partida);
      pantalla_nova=CERT;
      comencar_jugar=FALS;
    }

    if ( baixar_nivell ) {
      if (nivell>1) nivell--;
      baixar_nivell=FALS;
    }

    if ( pujar_nivell ) {
      if ( nivell < MAX_NIVELL ) nivell++;
      pujar_nivell=FALS;
    }

    if ( redefinir_tecles ) {
      demanar_tecles (&jugador_A.tecles, &jugador_B.tecles);
      pantalla_nova=CERT;
      redefinir_tecles=FALS;
      actualitzar_tecles (jugador_A.tecles, jugador_B.tecles);
    }

    if ( sobre_textris ) {
      estat_del_joc=ESTAT_SOBRE_TEXTRIS;
      dibujar_sobre_textris ();
      while ( !RSI_9_ha_canviat ) ;
      RSI_9_ha_canviat=FALS;
      sobre_textris=FALS;
      pantalla_nova=CERT;
      estat_del_joc=ESTAT_MENU_PRAL;
    }

  } while ( !(sortir || error) );

  acabar_textris ();
  if ( error ) {
    EscriureString (0, 0, "Error inesperat !!!!.", 0x2F);
    return (-2);
  }
  return (0);
}

/* fi del programa principal */



BYTE iniciar_textris (void) {
  BYTE i;
  WORD j;
  void iniciar_jugadors (BYTE);


  if ( !iniciar_video () ) {
    So (1200);                          /* fent piiiip! el que farà sirà... */
    for (i=0; i<127; i++) for (j=0; j<65535; j++); /*...informar de l'error */
    So(0);
    return (FALS); /* Retorna amb un ErrorLevel -1 */
  }

  iniciar_matriu_nivells ();
  iniciar_jugadors (1);
  hi_ha_rellotge=FALS; segons=0; minuts=0;

  copiar_tecles (&jugador_A.tecles, tecles_def_jugador_A);
  copiar_tecles (&jugador_B.tecles, tecles_def_jugador_B);

/* actualitzem les tecles que necesita la interrupció del teclat */
  actualitzar_tecles (jugador_A.tecles, jugador_B.tecles);

  estat_del_joc=ESTAT_MENU_PRAL;

  iniciar_RSI_8 (1); /* comença la interrupció del rellotge (música, ...) */
  iniciar_RSI_9 (); /* comença a llegir del teclat */
  return (CERT);
}


void iniciar_jugadors (BYTE nou_nivell) {
  /* Iniciar jugador A */
  jugador_A.peca_anterior=NO_PECA;
  jugador_A.peca_actual=aleatori(MAX_PECES);
  jugador_A.peca_seguent=aleatori(MAX_PECES);
  jugador_A.peca_act_x=X_INICIAL; jugador_A.peca_act_y=Y_INICIAL;
  jugador_A.peca_ant_x=-1;        jugador_A.peca_ant_y=-1;
  jugador_A.sortir=FALS;          jugador_A.canvi_peca=CERT;
  jugador_A.espera=FALS; /* El jugador A mai espera */
  jugador_A.punts=0;              jugador_A.linies=0;
  jugador_A.x=3;                  jugador_A.y=3;
  jugador_A.nivell=nou_nivell;    jugador_A.mort=FALS;
  jugador_A.linies_queden=num_linies_nou_nivell[nou_nivell-1];

  copiar_matriu (jugador_A.matriu_joc, matriu_nivell[nou_nivell-1]);

  /* Iniciar jugador B */
  jugador_B.peca_anterior=NO_PECA;
  jugador_B.peca_actual=aleatori(MAX_PECES);
  jugador_B.peca_seguent=aleatori(MAX_PECES);
  jugador_B.peca_act_x=X_INICIAL; jugador_B.peca_act_y=Y_INICIAL;
  jugador_B.peca_ant_x=-1;        jugador_B.peca_ant_y=-1;
  jugador_B.sortir=FALS;          jugador_B.canvi_peca=CERT;
  jugador_B.espera=CERT; /* Pot que només vulgui jugar un jugador */
  jugador_B.punts=0;              jugador_B.linies=0;
  jugador_B.x=27;                 jugador_B.y=3;
  jugador_B.nivell=nou_nivell;    jugador_B.mort=FALS;
  jugador_B.linies_queden=num_linies_nou_nivell[nou_nivell-1];

  copiar_matriu (jugador_B.matriu_joc, matriu_nivell[nou_nivell-1]);
}


void acabar_textris (void) {
  acabar_RSI_9 (); /* acaba la interrupció del teclat */
  acabar_RSI_8 (); /* acaba la interrupció del rellotge */
  acomiadament ();
}
