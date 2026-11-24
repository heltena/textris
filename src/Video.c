#include "video.h"


const struct Tcolor color_nul = {' ', 0x00};


const Tvector_colors vector_color = {
 /* color 0 */  { ' ', ' ', 0x00 },   /* negre */
 /* color 1 */  { '[', ']', 0x4F },   /* vermell */
 /* color 2 */  { '(', ')', 0x3F },   /* cian */
 /* color 3 */  { '▒', '▒', 0x7E },   /* groc */
 /* color 4 */  { '@', '@', 0x7A },   /* verd clar */
 /* color 5 */  { 'X', 'X', 0x5F },   /* magenta */
 /* color 6 */  { '\\', '\\', 0x6F },   /* marró */
 /* color 7 */  { '/', '\\', 0x1F } }; /* blau fosc */


const Tvector_colors vector_mono = {
 /* color 0 */  { ' ', ' ', 0x00 },   /* negre */
 /* color 1 */  { '[', ']', 0x7F },   /* blanc */
 /* color 2 */  { '(', ')', 0x7F },   /* gris clar */
 /* color 3 */  { '{', '}', 0x07 },   /* gris fosc */
 /* color 4 */  { 'O', 'O', 0x7F },   /* gris més fosc */
 /* color 5 */  { 'X', 'X', 0x07 },   /* gris entre clar i fosc */
 /* color 6 */  { '\\', '\\', 0x7F },   /* quasi no es veu de lo fosc que és */
 /* color 7 */  { '/', '\\', 0x7F } }; /* una mica més clar que el gris clar */


Tvector_colors vector_joc; /* Vector que s'utilitzarà dins el joc. */
                           /* Em d'inicicalitzar-lo segons sigui Color/Mono */
                           /* veure iniciar_video () */


BYTE iniciar_video (void) {
 BYTE i;
 PTR ptr_aux=(PTR)BufferPantalla();

 if (ptr_aux==Ptr(0xB800, 0x0000))
  for (i=0; i<MAX_COLORS; i++) vector_joc[i]=vector_color[i];
 else if (ptr_aux==Ptr(0xB000, 0x0000))
          for (i=0; i<MAX_COLORS; i++) vector_joc[i]=vector_mono[i];
     else return (FALS);
 return (CERT);
}
