#include "aleatori.h"

WORD aleatori (WORD rang) {
 valor_aleatori_RSI_8+=100; /* per "aleatoriar-lo" més */
 return (valor_aleatori_RSI_8 % rang);
}
