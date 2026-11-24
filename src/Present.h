/* Definició de les funcions que intenten alegrar la vida al jugador */

#ifndef PRESENT_H
#define PRESENT_H

#include "pantalla.h"
#include "peces.h"


void dibujar_inici (void);
  /* dibuja la pantalla de començament */

void dibujar_presentacio (void);
  /* dibuja la presentació */


void dibujar_nivell (BYTE);
  /* dibuja la barra de nivell (Jaaal!) */


void dibujar_pantalla_joc (void);
  /* dibuja la pantalla de joc (molt bonica) */


void dibujar_demanar_tecles (void);
  /* dibuja la pantalla de demanar tecles (redefinir) */


void dibujar_sobre_textris (void);
  /* dibuja la pantalla on s'informa del joc */


void dibujar_geim_ober (BYTE, BYTE);
  /* dibuja a la posició indicada l'advertiment de que s'ha acabat la */
  /* partida del jugador */

void acomiadament (void);
  /* acomiadament. S'enten? */

#endif
