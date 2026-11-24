#include "musica.h"

struct Tmusica musica_tetris = {
 CERT, /* repetir */
 FALS, /* pausa */
 57, /* notes */
 0, /* nota_actual */
 0, /* duracio_so_actual */
 0, /* duracio_so_parcial */
 0, /* duracio_so_total */
 {
 {2, 1, LA+OCTAVA}, {2, 1, 0},
 {2, 1, MI+OCTAVA}, {2, 1, FA+OCTAVA}, {2, 1, SOL+OCTAVA}, {2, 1, 0},
 {2, 1, FA+OCTAVA}, {2, 1, MI+OCTAVA}, {2, 1, RE+OCTAVA}, {2, 1, 0},
 {2, 1, RE+OCTAVA}, {2, 1, FA+OCTAVA}, {2, 1, LA+OCTAVA}, {2, 1, 0},
 {2, 1, SOL+OCTAVA}, {2, 1, FA+OCTAVA}, {2, 1, MI+OCTAVA}, {2, 1, 0},
 {2, 1, MI+OCTAVA}, {2, 1, FA+OCTAVA}, {2, 1, SOL+OCTAVA}, {2, 1, 0},
 {2, 1, LA+SEMI_TO+OCTAVA}, {2, 1, 0},
 {2, 1, LA+OCTAVA}, {2, 1, 0},
 {2, 1, FA+OCTAVA}, {2, 1, 0},
 {2, 1, FA+OCTAVA}, {9, 1, 0},

 {2, 1, SOL+OCTAVA}, {2, 1, 0},
 {2, 1, LA+SEMI_TO+OCTAVA}, {2, 1, MI+OCTAVA}, {2, 1, 0},
 {2, 1, DO+2*OCTAVA}, {2, 1, LA+SEMI_TO+OCTAVA}, {2, 1, LA+OCTAVA}, {2, 1, 0},
 {2, 1, FA+OCTAVA}, {2, 1, LA+OCTAVA}, {2, 1, LA+OCTAVA},
   {2, 1, SOL+OCTAVA}, {2, 1, FA+OCTAVA}, {2, 1, MI+OCTAVA}, {2, 1, 0},
 {2, 1, MI+OCTAVA}, {2, 1, FA+OCTAVA}, {2, 1, SOL+OCTAVA}, {2, 1, 0},
 {2, 1, LA+OCTAVA}, {2, 1, 0},
 {2, 1, FA+OCTAVA}, {2, 1, 0},
 {2, 1, RE+OCTAVA}, {2, 1, RE+OCTAVA}, {18, 9, 0},
 }
};

struct Tmusica musica_moure_peca = {
 FALS, /* repetir */
 FALS, /* pausa */
 2, /* notes */
 2, /* nota_actual */
 0, /* duracio_so_actual */
 0, /* duracio_so_parcial */
 0, /* duracio_so_total */
 {
 {0, 0, DO}, {0, 0, 0}
 }
};

struct Tmusica musica_esborrar_una = {
 FALS, /* repetir */
 FALS, /* pausa */
 4, /* notes */
 4, /* nota_actual */
 0, /* duracio_so_actual */
 0, /* duracio_so_parcial */
 0, /* duracio_so_total */
 {
 {0, 0, DO}, {0, 0, RE}, {0, 0, MI}, {0, 0, 0}
 }
};

struct Tmusica musica_esborrar_dos = {
 FALS, /* repetir */
 FALS, /* pausa */
 4, /* notes */
 4, /* nota_actual */
 0, /* duracio_so_actual */
 0, /* duracio_so_parcial */
 0, /* duracio_so_total */
 {
 {0, 0, FA}, {0, 0, SOL}, {0, 0, LA}, {0, 0, 0}
 }
};

struct Tmusica musica_esborrar_tres = {
 FALS, /* repetir */
 FALS, /* pausa */
 6, /* notes */
 6, /* nota_actual */
 0, /* duracio_so_actual */
 0, /* duracio_so_parcial */
 0, /* duracio_so_total */
 {
 {0, 0, DO}, {0, 0, DO}, {0, 0, RE}, {0, 0, LA}, {0, 0, SOL}, {0, 0, 0}
 }
};

struct Tmusica musica_esborrar_textris = {
 FALS, /* repetir */
 FALS, /* pausa */
 17, /* notes */
 17, /* nota_actual */
 0, /* duracio_so_actual */
 0, /* duracio_so_parcial */
 0, /* duracio_so_total */
 {
 {1, 0, SOL+OCTAVA}, {1, 0, LA+OCTAVA}, {2, 1, SI+OCTAVA},
 {1, 0, RE+2*OCTAVA}, {1, 0, RE+2*OCTAVA}, {1, 0, MI+2*OCTAVA},
 {1, 0, RE+2*OCTAVA}, {1, 0, SI+OCTAVA}, {1, 0, SOL+OCTAVA},
 {1, 0, SOL+OCTAVA}, {2, 1, LA+OCTAVA}, {1, 0, SI+OCTAVA},
 {1, 0, SI+OCTAVA}, {1, 0, LA+OCTAVA}, {1, 0, SOL+OCTAVA},
 {3, 2, LA+OCTAVA}, {0, 0, 0}
 }
};


void iniciar_canco (struct Tmusica *musica, BYTE sonar) {
 WORD flags=Inhibir(); /* per no interrompre aquí, canvio les variables */
 (*musica).nota_actual=0;
 (*musica).duracio_so_actual=0;
 (*musica).duracio_so_parcial=0;
 (*musica).duracio_so_total=0;
 (*musica).pausa=!sonar;
 Desinhibir (flags); /* que utilitza la RSI */
}


void canviar_pausa (struct Tmusica *musica) {
 (*musica).pausa=!(*musica).pausa;
}


void posar_pausa (struct Tmusica *musica) {
 (*musica).pausa=CERT;
}


void treure_pausa (struct Tmusica *musica) {
 (*musica).pausa=FALS;
}


BYTE actualitzar_canco (struct Tmusica *musica) {

 /* la cançó a de sonar ? */
 if (!(*musica).pausa) {

  /* comença la pausa fins la següent nota? */
  if ((*musica).duracio_so_actual>(*musica).duracio_so_parcial) So (0);

  /* canviem la nota actual ? */
  if (++(*musica).duracio_so_actual>(*musica).duracio_so_total) {

   /* anem per la nota següent */
   So ((*musica).canco[(*musica).nota_actual].nota);
   (*musica).duracio_so_total=(*musica).canco[(*musica).nota_actual].duracio;
   (*musica).duracio_so_parcial=(*musica).canco[(*musica).nota_actual].parcial;
   (*musica).duracio_so_actual=0;

   /* em arribat al final de la cançó ? */
   if (++(*musica).nota_actual>=(*musica).notes) {
         (*musica).nota_actual=0;
         (*musica).pausa=!((*musica).repetir);
   }
  }
  return CERT;
 } else return FALS;
}
