#include "present.h"


void dibujar_inici (void) {
  BYTE i, j;

  EsborrarPantalla (0x17);


  EscriureString ( 2, 10, "Helio Tejedor Navarro i Joan Carol Serrat   presenten ...", 0x1F);

  EscriureString ( 3, 0, "                        ▄▄▄" ,0x1E);
  EscriureString ( 4, 0, "                        ███████▄▄▄▄▄▄▄█████████" ,0x1E);
  EscriureString ( 5, 0, "                        ███████████████████████▌" ,0x1E);
  EscriureString ( 6, 0, "                ▄▄█████▐███████████████████████▌                 ▄▄▄██▄▄▄" ,0x1E);
  EscriureString ( 7, 0, "             ▄█████████▌████▀     ▀██████    ▀█▌ ▄▄██▄   ▐▄    ▐████████████" ,0x1E);
  EscriureString ( 8, 0, "        ▄▄█████████████ ███▌       █████▌  ████  ██████▄ ██▌   ███████▀▀▀▀▀" ,0x1E);
  EscriureString ( 9, 0, "    ██████████████▀▀▀    ▀▀        ▐████▌     ███▀ ███▌  █████ ▐███▀" ,0x1E);
  EscriureString (10, 0, "   ▐███████████▀  ▄██████▀▀        █████▌   ███▀   ▐███  ████    ▀███████▄" ,0x1E);
  EscriureString (11, 0, "   ▐███▀   ████   ████▀   █▄ ▄█   ▐████▌    ▐███████▌    ████▌        █████▄" ,0x1E);
  EscriureString (12, 0, "   ███     ▐██▌   ████▄▄█  █▄█   ▄█████▌     ████████▄   ████▌        ██████" ,0x1E);
  EscriureString (13, 0, "   █       ▐██▌   ▐██████  ▄█▄  ██████▌      ███████████  ███▌        █████▀" ,0x1E);
  EscriureString (14, 0, "           ▐██▌   ▐██  ▀█ ██ █▄███████▌     ▐████▀▀██████▄ ██▌      ▄█████▌" ,0x1E);
  EscriureString (15, 0, "          ▐████   ▐███▄   █▀ ▀█ ██████▌    ▐████▌   █████▀ ██▌   ▄▄███████" ,0x1E);
  EscriureString (16, 0, "         ▐█████▄     ██████▀     █████▌    █████▌    ▐███ ▄▄▄▄███████████▀" ,0x1E);
  EscriureString (17, 0, "        ▐████████▀    ████        ████    ██████      █████████████████▀" ,0x1E);
  EscriureString (18, 0, "       ▐██████▀        ▀▀          ██   ▐███████         ▀██████████▀" ,0x1E);
  EscriureString (19, 0, "       █████▀                              ▀███▌" ,0x1E);
  EscriureString (20, 0, "       ███▀" ,0x1E);
  EscriureString (21, 0, "       █▀" ,0x1E);
  EscriureString (22, 30, "Data: 13 d'abril de 1997", 0x1E);

  for (i=0; i<80; i++) {
    EscriureCar ( 0, i, '█', 0x1E);
    EscriureCar (24, i, '▀', 0x1C);
  }
  for (i=0; i<25; i++) {
    EscriureCar ( i,  1, '█', 0x1E);
    EscriureCar ( i, 78, '█', 0x1C);
  }

}


void dibujar_presentacio (void) {
  BYTE i, j;

  EsborrarPantalla (0x17);
  for (i=0; i<80; i++) {
    EscriureCar ( 0, i, '█', 0x1E);
    EscriureCar (24, i, '▀', 0x1C);
  }
  for (i=0; i<25; i++) {
    EscriureCar ( i,  1, '█', 0x1E);
    EscriureCar ( i, 78, '█', 0x1C);
  }
  EscriureString (0, 24, STRING_TEXTRIS_1, 0x1E);
  EscriureString (1, 24, STRING_TEXTRIS_2, 0x1E);
  EscriureString (2, 24, STRING_TEXTRIS_3, 0x1E);
  EscriureString (3, 24, STRING_TEXTRIS_4, 0x1E);

  EscriureString ( 4, 20, "Començar nova", 0x1F);
  EscriureString ( 5, 20, "partida!", 0x1F);

  EscriureString (10,  3, "Baixar", 0x1F);
  EscriureString (11,  3, "nivell", 0x1F);

  EscriureString (16, 20, "Sortir", 0x1F);

  EscriureString ( 9, 35, "Continuar", 0x1F);
  EscriureString (10, 38, "amb", 0x1F);
  EscriureString (11, 35, "l'antiga", 0x1F);
  EscriureString (12, 36, "partida", 0x1F);

  EscriureString ( 4, 50, "Redefinir", 0x1F);
  EscriureString ( 5, 53, "tecles", 0x1F);

  EscriureString (10, 71, "Pujar", 0x1F);
  EscriureString (11, 70, "nivell", 0x1F);

  EscriureString (16, 50, "Sobre", 0x1F);
  EscriureString (17, 50, "textris...", 0x1F);

  EscriureString (20, 5, "Canviar de nivell no fa efecte sobre la partida en curs.", 0x1F);
  EscriureString (21, 5, "Hauràs de començar altre partida si vols canviar el nivell.", 0x1F);
}

void dibujar_nivell (BYTE nivell) {
  BYTE i, x=(nivell-1)*67/10+5;

  for (i=2; i<x; i++) EscriureCar (23, i, '■', 0x1F);
  for (i=x+10; i<78; i++) EscriureCar (23, i, '■', 0x1E);

  EscriureString (23, x, "Nivell: xx", 0x1F);
  EscriureCar (23, x+8, ((nivell/10==0) ? ' ' : '1'), 0x1F);
  EscriureCar (23, x+9, nivell%10 + '0', 0x1F);
}


void dibujar_pantalla_joc () {
  BYTE i, j;

  EsborrarPantalla (0x17);
  for (i=0; i<80; i++) {
    EscriureCar ( 0, i, '█', 0x1E);
    EscriureCar (24, i, '▀', 0x1C);
  }
  for (i=0; i<25; i++) {
    EscriureString ( i,  1, "█", 0x1E);
    EscriureString ( i, 78, "█", 0x1C);
  }

  EscriureString (0, 24, STRING_TEXTRIS_1, 0x1E);
  EscriureString (1, 24, STRING_TEXTRIS_2, 0x1E);
  EscriureString (2, 24, STRING_TEXTRIS_3, 0x1E);
  EscriureString (3, 24, STRING_TEXTRIS_4, 0x1E);

  for (j=0; j<20; j++) {
    for (i=(j%2); i<2; i++) {
      EscriureCar (3+j,  4+i, '▀', 0x16);
      EscriureCar (3+j, 26+i-(j%2), '▀', 0x16);
      EscriureCar (3+j, 52+i, '▀', 0x16);
      EscriureCar (3+j, 74+i-(j%2), '▀', 0x16);
    }
    for (i=0; i<20; i++) {
      EscriureCar (3+j,  6+i, ' ', 0x07);
      EscriureCar (3+j, 54+i, ' ', 0x07);
    }
  }
  for (i=0; i<24; i++) {
    EscriureCar (23,  4+i, '▀', 0x16);
    EscriureCar (23, 52+i, '▀', 0x16);
  }
  EscriureString (4, 29, "< Próxima peça", 0x1F);
  for (j=0; j<6; j++)
    for (i=0; i<6*2; i++) {
      EscriureCar (6+j, 29+i, ' ', 0x07);
      EscriureCar (13+j, 38+i, ' ', 0x07);
    }
  EscriureString (13, 29, "< Línies", 0x1F);
  EscriureString (14, 30, "00.000", 0x17);
  EscriureString (18, 29, "< Punts", 0x1F);
  EscriureString (17, 30, "00.000", 0x17);

  EscriureString ( 6, 43, "Línies >", 0x1F);
  EscriureString ( 7, 45, "00.000", 0x17);
  EscriureString (11, 44, "Punts >", 0x1F);
  EscriureString (10, 45, "00.000", 0x17);
  EscriureString (20, 37, "Próxima peça >", 0x1F);
  EscriureString (22, 32, "00", 0x17);
  EscriureString (22, 35, "< Nivell >", 0x1F);
  EscriureString (22, 46, "00", 0x17);

  for (i=0; i<12; i++) {
    EscriureCar (12, 30+i, '▀', 0x1B);
    EscriureCar (19, 39+i, '▀', 0x1B);
  }
  EscriureCar (6, 41, '█', 0x1B);
  EscriureCar (13, 50, '█', 0x1B);

  for (j=0; j<5; j++) {
    EscriureCar (7+j, 41, '█', 0x1B);
    EscriureCar (14+j, 50, '█', 0x1B);
  }

  EscriureString (24,  7, " [ Plaller Juan ] ", 0x5F);
  EscriureString (24, 56, " [ Plaller Tu ] ", 0x5F);

  EscriureString (24, 35, " [ 00:00 ] ", 0x5F);

  EscriureString (2,  6, "Et queden 00 línies ", 0x5F);
  EscriureString (2, 54, "Et queden 00 línies ", 0x5F);
}


void dibujar_demanar_tecles (void) {
  BYTE i, j;

  EsborrarPantalla (0x17);
  for (i=0; i<80; i++) {
    EscriureCar ( 0, i, '█', 0x1E);
    EscriureCar (24, i, '▀', 0x1C);
  }
  for (i=0; i<25; i++) {
    EscriureCar ( i,  1, '█', 0x1E);
    EscriureCar ( i, 78, '█', 0x1C);
  }
  EscriureString (0, 24, STRING_TEXTRIS_1, 0x1E);
  EscriureString (1, 24, STRING_TEXTRIS_2, 0x1E);
  EscriureString (2, 24, STRING_TEXTRIS_3, 0x1E);
  EscriureString (3, 24, STRING_TEXTRIS_4, 0x1E);

  for (j=4; j<24; j++) {
    EscriureCar (j, 39, '▐', 0x1E);
    EscriureCar (j, 40, '▌', 0x1C);
  }
  for (j=4; j<22; j++) {
    for (i=0; i<30; i++) {
      EscriureCar (j, 4+i, ' ', 0x3F);
      EscriureCar (j, 44+i, ' ', 0x3F);
    }
    EscriureString (j+1, 34, "  ", 0x0F);
    EscriureString (j+1, 74, "  ", 0x0F);
  }
  for (i=0; i<30; i++) {
    EscriureCar (22, 6+i, ' ', 0x0F);
    EscriureCar (22, 46+i, ' ', 0x0F);
  }

  EscriureString (24,  7, " [ Plaller Juan ] ", 0x5F);
  EscriureString (24, 56, " [ Plaller Tu ] ", 0x5F);
}


void dibujar_sobre_textris (void) {
  BYTE i, j;

  EsborrarPantalla (0x17);
  for (i=0; i<80; i++) {
    EscriureCar ( 0, i, '█', 0x1E);
    EscriureCar (24, i, '▀', 0x1C);
  }
  for (i=0; i<25; i++) {
    EscriureCar ( i,  1, '█', 0x1E);
    EscriureCar ( i, 78, '█', 0x1C);
  }
  EscriureString (0, 24, STRING_TEXTRIS_1, 0x1E);
  EscriureString (1, 24, STRING_TEXTRIS_2, 0x1E);
  EscriureString (2, 24, STRING_TEXTRIS_3, 0x1E);
  EscriureString (3, 24, STRING_TEXTRIS_4, 0x1E);

  for (j=0; j<11; j++) {
    for (i=0; i<58; i++)
      EscriureCar (j+5, i+10, ' ', 0x3F);
    EscriureString (j+6, 68, "  ", 0x0F);
  }
  for (i=0; i<58; i++) EscriureCar (16, i+12, ' ', 0x0F);

  EscriureString ( 6, 12, "Textris és un joc 'LearnWare' forçat pel              ", 0x3F);
  EscriureString ( 7, 12, "departament d'informàtica de la URV.       NO  SE  PER", 0x3F);
  EscriureString ( 8, 12, "Nosaltres, Helio Tejedor Navarro i         QUE LLEGEIX", 0x3F);
  EscriureString ( 9, 12, "Joan Carol Serrat em aprés molt fent-lo.   AIXO  SI NO", 0x3F);
  EscriureString (11, 12, "JAJAJA (Joves Alumnes que Juguen A Jocs    SERVEIX PER", 0x3F);
  EscriureString (12, 12, "com Aquest) no es responsabilitza dels     A RES.     ", 0x3F);
  EscriureString (13, 12, "mals de caps que poden sorgir de la                   ", 0x3F);
  EscriureString (14, 12, "exposició continuada del seu codi en C.    13 abril 97", 0x3F);

  EscriureString (24, 22, " [Prem una tecla per continuar ...] ", 0x5F);
}


void dibujar_geim_ober (BYTE x, BYTE y) {
  BYTE i, j;

  for (j=0; j<3; j++) {
    for (i=0; i<11; i++)
      EscriureCar (y+j, x+i, ' ', 0x3F);
    EscriureString (y+j+1, x+11, "  ", 0x0F);
  }
  for (i=0; i<11; i++) EscriureCar (y+3, x+i+2, ' ', 0x0F);
  EscriureString (y+1, x+1, "GEIM OVER", 0x3F);
}


void acomiadament (void) {
  EsborrarPantalla (0x0F);
}
