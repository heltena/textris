#include "joc.h"

void jugar (BYTE *hi_ha_partida) {
 BYTE j;
 void moure_jugador (struct Tjugador *, struct Tjugador *, BYTE, BYTE);
 void dibujar_dades (void);

 dibujar_pantalla_joc ();
 dibujar_matriu (jugador_A.matriu_joc, jugador_A.x, jugador_A.y);
 dibujar_matriu (jugador_B.matriu_joc, jugador_B.x, jugador_B.y);

 dibujar_peca (jugador_A.peca_actual, jugador_A.x+jugador_A.peca_act_x,
                    jugador_A.y+jugador_A.peca_act_y, NUL);
 dibujar_peca (jugador_A.peca_seguent, X_PECA_SEG_A, Y_PECA_SEG_A, NUL);

 if (!jugador_B.espera) { /* Si no juga, que no es vegi */
  dibujar_peca (jugador_B.peca_actual, jugador_B.x+jugador_B.peca_act_x,
                     jugador_B.y+jugador_B.peca_act_y, NUL);
  dibujar_peca (jugador_B.peca_seguent, X_PECA_SEG_B, Y_PECA_SEG_B, NUL);
 }


 /* només començar posar GEIM_OVER si pot ser */
 if (jugador_A.mort) dibujar_geim_ober (X_GEIM_OVER_A, Y_GEIM_OVER_A);
 if (jugador_B.mort) dibujar_geim_ober (X_GEIM_OVER_B, Y_GEIM_OVER_B);

 /* esperem cinc segons o que hagi alguna tecla polsada */
 cinc_segons=0; RSI_9_ha_canviat=FALS;
 while ( (cinc_segons<5) && (!RSI_9_ha_canviat) ) ;

 /* comença el joc (o continua) */
 hi_ha_rellotge=CERT; /* posem el rellotge */
 jugador_A.sortir=jugador_A.mort; jugador_B.sortir=jugador_B.mort;
 while ( !(jugador_A.sortir && jugador_B.sortir) ) {

  RSI_8_permet_canviar=FALS; RSI_8_fa_baixar=FALS;
  while ( !RSI_8_permet_canviar ) ; /* rellotge del joc */

  moure_jugador (&jugador_A, &jugador_B, X_PECA_SEG_A, Y_PECA_SEG_A);

  /* Si l'altre jugador no juga seria mala ostia pasar-li línies */
  if (jugador_A.pasar && jugador_B.espera) jugador_A.pasar=FALS;

  if ( jugador_B.espera &&   /* Si prem alguna tecla comença a jugar */
     ( jugador_B.mov_rotar_esq ||
       jugador_B.mov_rotar_dre ||
       jugador_B.mov_esquerra ||
       jugador_B.mov_dreta ||
       jugador_B.mov_baixar ) ) {
   jugador_B.espera = FALS;
   dibujar_peca (jugador_B.peca_actual, jugador_B.x+jugador_B.peca_act_x,
                      jugador_B.y+jugador_B.peca_act_y, NUL);
   dibujar_peca (jugador_B.peca_seguent, X_PECA_SEG_B, Y_PECA_SEG_B, NUL);
  }

  if (!jugador_B.espera) /* Si pot jugar */
   moure_jugador (&jugador_B, &jugador_A, X_PECA_SEG_B, Y_PECA_SEG_B);

  dibujar_dades ();

  if (jugador_A.mort) dibujar_geim_ober (X_GEIM_OVER_A, Y_GEIM_OVER_A);
  if (jugador_B.mort) dibujar_geim_ober (X_GEIM_OVER_B, Y_GEIM_OVER_B);

  if (jugador_A.mov_abortar) jugador_A.sortir=CERT;
  if (jugador_B.mov_abortar) jugador_B.sortir=CERT;
 }
 hi_ha_rellotge=FALS; /* treiem el rellotge */

 if (!(jugador_A.mov_abortar || jugador_B.mov_abortar)) {
  cinc_segons=0;          /* esperem una mica per veure Geim Ober */
  while (cinc_segons<2) ; /* només si no s'ha abortat */
 }

 (*hi_ha_partida)=!(jugador_A.mort && jugador_B.mort);

 jugador_A.mov_abortar=FALS; /* eliminem les tecles que estan polsades */
 jugador_A.mov_esquerra=FALS;
 jugador_A.mov_dreta=FALS;
 jugador_A.mov_rotar_esq=FALS;
 jugador_A.mov_rotar_dre=FALS;

 jugador_B.mov_abortar=FALS;
 jugador_B.mov_esquerra=FALS;
 jugador_B.mov_dreta=FALS;
 jugador_B.mov_rotar_esq=FALS;
 jugador_B.mov_rotar_dre=FALS;
}


void moure_jugador (struct Tjugador *jugador, struct Tjugador *altre_jugador,
                      BYTE x_peca_seg, BYTE y_peca_seg) {
 BYTE i,
    linies_aux; /* linies que esborra, de forma auxiliar, pels càlculs */

 /* Guardem les variables anteriors */
 (*jugador).peca_anterior=(*jugador).peca_actual;
 (*jugador).peca_ant_x=(*jugador).peca_act_x;
 (*jugador).peca_ant_y=(*jugador).peca_act_y;

 (*jugador).canvi_peca=FALS;
 if ( !(*jugador).mort ) { /* Es pot moure? */

  /* si vol anar a esquerra i pot anar a esquerra, que vagi a esquerra */
  if ( (*jugador).mov_esquerra &&
          peca_legal( (*jugador).peca_actual,
                      (*jugador).peca_act_x-1, (*jugador).peca_act_y,
                      (*jugador).matriu_joc) )
   (*jugador).peca_act_x--;


  /* si vol anar a dreta i pot anar a dreta, que vagi a dreta */
  if ( (*jugador).mov_dreta &&
          peca_legal( (*jugador).peca_actual,
                      (*jugador).peca_act_x+1, (*jugador).peca_act_y,
                      (*jugador).matriu_joc) )
   (*jugador).peca_act_x++;


  /* si vol rotar i pot rotar a l'esquerra, que roti a l'esquerra */
  if ( (*jugador).mov_rotar_esq &&
          peca_legal( vector_peces[(*jugador).peca_actual].seg_peca_rot_esq,
                      (*jugador).peca_act_x, (*jugador).peca_act_y,
                      (*jugador).matriu_joc) )
   (*jugador).peca_actual=vector_peces[(*jugador).peca_actual].seg_peca_rot_esq;


  /* si vol rotar i pot rotar a la dreta, que roti a la dreta */
  if ( (*jugador).mov_rotar_dre &&
          peca_legal( vector_peces[(*jugador).peca_actual].seg_peca_rot_dre,
                      (*jugador).peca_act_x, (*jugador).peca_act_y,
                      (*jugador).matriu_joc) )
   (*jugador).peca_actual=vector_peces[(*jugador).peca_actual].seg_peca_rot_dre;

  /* Si es vol baixar i es canvia la peca actual pot haver problemes */
  /* a l'hora de dibuixar la peça. Ens estalviem mal de caps i la */
  /* dibuixem aquí, després de modificar la peça */
  if ( (*jugador).peca_actual != (*jugador).peca_anterior) {
   /* S'ha modificat la peça */
   esborrar_peca ((*jugador).peca_anterior,
                      (*jugador).x+(*jugador).peca_ant_x,
                      (*jugador).y+(*jugador).peca_ant_y,
                      0x0F); /* esborrem l'antiga */
   dibujar_peca ((*jugador).peca_actual,
                      (*jugador).x+(*jugador).peca_act_x,
                      (*jugador).y+(*jugador).peca_act_y,
                      NUL); /* Dibujem la peça a la pantalla */
   /* Canviem els valors de la peça */
   (*jugador).peca_anterior=(*jugador).peca_actual;
   (*jugador).peca_ant_x=(*jugador).peca_act_x;
   (*jugador).peca_ant_y=(*jugador).peca_ant_y;
  }

  /* si vol baixar ... */
  if ( (*jugador).mov_baixar || RSI_8_fa_baixar ) {
   iniciar_canco (&musica_moure_peca, CERT);

   if (peca_legal((*jugador).peca_actual,
                      (*jugador).peca_act_x, (*jugador).peca_act_y+1,
                      (*jugador).matriu_joc) )
          (*jugador).peca_act_y++; /* pot baixar */
   else { /* no pot baixar */
          /* esborrar la seg. peca*/
          esborrar_peca ((*jugador).peca_seguent, x_peca_seg, y_peca_seg, NUL);
          (*jugador).peca_actual=(*jugador).peca_seguent;
          (*jugador).peca_seguent=aleatori(MAX_PECES);
          (*jugador).peca_act_x=X_INICIAL;
          (*jugador).peca_act_y=Y_INICIAL;
          (*jugador).canvi_peca=CERT;

          if (!peca_legal((*jugador).peca_actual,
                             (*jugador).peca_act_x, (*jugador).peca_act_y,
                             (*jugador).matriu_joc) ) {
           (*jugador).sortir=CERT;
     (*jugador).mort=CERT;
    }
   } /* fi del if-else */
  } /* fi del if */
 } /* fi del primer if */


 if (!(*jugador).sortir) { /* dibuixem al jugador */
  if ((*jugador).canvi_peca) { /* Nova peça ? */

   linies_aux=esborrar_linies ((*jugador).matriu_joc,
                                        (*altre_jugador).matriu_joc,
                                        (*jugador).peca_anterior,
                                        (*jugador).peca_ant_x,
                                        (*jugador).peca_ant_y,
                                        (*jugador).pasar && !(*altre_jugador).mort);
   /* si l'altre jugador està mort no fa falta que passem les línies */

   switch (linies_aux) {
          case 0 : dibujar_peca ((*jugador).peca_anterior,
                                 (*jugador).peca_ant_x,
                                 (*jugador).peca_ant_y,
                                 (*jugador).matriu_joc); break;
          case 1 : iniciar_canco (&musica_esborrar_una, CERT);
                    (*jugador).linies++; (*jugador).linies_queden--;
                    (*jugador).punts+=40; break;

          case 2 : iniciar_canco (&musica_esborrar_dos, CERT);
                    (*jugador).linies+=2;
                    if ( (*jugador).linies_queden > 2 )
                     (*jugador).linies_queden-=2;
                    else (*jugador).linies_queden=0;
                    (*jugador).punts+=100; break;

          case 3 : iniciar_canco (&musica_esborrar_tres, CERT);
                    (*jugador).linies+=3;
                    if ( (*jugador).linies_queden > 3 )
                     (*jugador).linies_queden-=3;
                    else (*jugador).linies_queden=0;
                    (*jugador).punts+=300; break;

          case 4 : iniciar_canco (&musica_esborrar_textris, CERT);
                    (*jugador).linies+=4;
                    if ( (*jugador).linies_queden > 4 )
                     (*jugador).linies_queden-=4;
                    else (*jugador).linies_queden=0;
                    (*jugador).punts+=1200; break;

          default: break; /* Imposible */
   }

   if (linies_aux!=0) { /* si ha fet línies */
          dibujar_matriu ((*jugador).matriu_joc, (*jugador).x, (*jugador).y);
          dibujar_peca ((*jugador).peca_actual,
                       (*jugador).x+(*jugador).peca_act_x,
                       (*jugador).y+(*jugador).peca_act_y, NUL);

          if ((*jugador).pasar && !(*altre_jugador).mort) {
           /* si pasa línies, dibuixa l'altre jugador */

           dibujar_matriu ((*altre_jugador).matriu_joc,
                              (*altre_jugador).x, (*altre_jugador).y);
           dibujar_peca ((*altre_jugador).peca_actual,
                             (*altre_jugador).x+(*altre_jugador).peca_act_x,
                             (*altre_jugador).y+(*altre_jugador).peca_act_y, NUL);
          }
          (*jugador).pasar=FALS; /* segur que aquí mai podrà passar */


          if ( (*jugador).linies_queden == 0) {
           (*jugador).nivell++; (*jugador).nivell%=(MAX_NIVELL+1); /* nou nivell */
           (*jugador).linies_queden=num_linies_nou_nivell[(*jugador).nivell];

           if (nivell_RSI_8<(*jugador).nivell) /* nivell del jugador major que l'actual? */
            canviar_nivell ((*jugador).nivell); /* canvia el nivell de RSI 8 */
            /* aixó es fa perque vagi més depresa */

           if ((*jugador).nivell==0) (*jugador).nivell++;
           copiar_matriu ((*jugador).matriu_joc,
                              matriu_nivell[(*jugador).nivell-1]);
           dibujar_matriu ((*jugador).matriu_joc,
                              (*jugador).x, (*jugador).y);
          }
          for (i=0; i<linies_aux; i++)
           if ( (((*jugador).linies-i) % NUM_LINIES_PASAR) == 0)
            (*jugador).pasar=CERT; /* pot pasar línies */
   }
   dibujar_peca ((*jugador).peca_seguent, x_peca_seg, y_peca_seg, NUL);
  } /* Doncs no hi ha nova peça */
  else
   esborrar_peca ((*jugador).peca_anterior,
                      (*jugador).x+(*jugador).peca_ant_x,
                      (*jugador).y+(*jugador).peca_ant_y,
                      0x0F); /* esborrem l'antiga */
  dibujar_peca ((*jugador).peca_actual,
                     (*jugador).x+(*jugador).peca_act_x,
                     (*jugador).y+(*jugador).peca_act_y,
                     NUL); /* Dibujem la peça a la pantalla */
 }
}


void dibujar_dades (void) {
 BYTE atribut;

 Escriure_WORD (14, 30, jugador_A.linies, 0x1F);
 Escriure_WORD (17, 30, jugador_A.punts, 0x1F);
 Escriure_WORD ( 7, 45, jugador_B.linies, 0x1F);
 Escriure_WORD (10, 45, jugador_B.punts, 0x1F);

 EscriureCar (22, 32, '0'+jugador_A.nivell/10, 0x1F);
 EscriureCar (22, 33, '0'+jugador_A.nivell%10, 0x1F);

 EscriureCar (22, 46, '0'+jugador_B.nivell/10, 0x1F);
 EscriureCar (22, 47, '0'+jugador_B.nivell%10, 0x1F);


/* a continuació poso les panpallugues. No van al mateix desfasament per */
/* donar impresió. */

 if ( (jugador_A.linies_queden < 5) && panpalluga ) atribut=0x75;
 else atribut=0x5F;
 EscriureCar (2, 16, '0'+jugador_A.linies_queden/10, atribut);
 EscriureCar (2, 17, '0'+jugador_A.linies_queden%10, atribut);

 if ( (jugador_B.linies_queden < 5) && !panpalluga ) atribut=0x75;
 else atribut=0x5F;
 EscriureCar (2, 64, '0'+jugador_B.linies_queden/10, atribut);
 EscriureCar (2, 65, '0'+jugador_B.linies_queden%10, atribut);

 if (jugador_A.pasar) {
  if (!panpalluga) atribut=0x71;
  else atribut=0x1F;
 } else atribut=0x11;
 EscriureString (23, 27, " >> pasar >> ", atribut);

 if (jugador_B.pasar) {
  if (panpalluga) atribut=0x71;
  else atribut=0x1F;
 } else atribut=0x11;
 EscriureString (23, 40, " << pasar << ", atribut);
}
