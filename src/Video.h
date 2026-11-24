/* Definició dels tipus de dades que defineixen el tractament del video */

#ifndef VIDEO_H
#define VIDEO_H

#include "convtips.h"
#include "gral.h"
#include "pantalla.h"

#define MAX_COLORS 8    /* 0 negre, 1..7 peces */

struct Tcolor {
 char caracter_esq, caracter_dre, atribut;
};

extern struct Tcolor color_nul;


typedef struct Tcolor Tvector_colors[MAX_COLORS];

extern Tvector_colors vector_color;
extern Tvector_colors vector_mono;
extern Tvector_colors vector_joc;


BYTE iniciar_video (void);
/* Inicialitza el sistema de vídeo. Crida a la funció BufferPantalla() */
/* en cas de produir-se un error retorna FALS. En cas contrari retorna CERT */


#endif
