#include "gral.h"


/* Logotip */
char *STRING_TEXTRIS_1 = "▌    ▄▄▀ ▄   ▄▄▀ ▄▄    ▀  ▄▄   ▐";
char *STRING_TEXTRIS_2 = " ───▀▐──█▄──▀▐──█▄▄█──█──▀█──── ";
char *STRING_TEXTRIS_3 = "   +-█-▐▄▄-X-█-▐--▀▄-▐--▄▄▀-+   ";
char *STRING_TEXTRIS_4 = "       --   -  -  -  - --       ";


/* Variable que controla el so */
BYTE hi_ha_so = CERT, hi_ha_EFESP = CERT; /* Per defecte ... */


/* número de línies que s'han de fer per pasar de nivell */
extern BYTE num_linies_nou_nivell[MAX_NIVELL] = {
 10, 15, 15, 20, 20, 25, 20, 25, 25, 30};


void Escriure_WORD (BYTE fil, BYTE col, WORD valor, BYTE atribut) {
 WORD i=10000, j=0;

 while (i>0) {
  EscriureCar (fil, col+j, '0' + ( (i==0) ? 0 : (valor / i) ), atribut);
  valor%=i; /* agafem residus */
  i/=10; /* Baixem una xifra decimal */
  j++;  /* un altre caracter */
  if (j==2) { /* únicament per posar el punt de les mil.lèsimes */
   EscriureCar (fil, col+2, '.', atribut);
   j++; /* posem el punt */
  }
 }
}
