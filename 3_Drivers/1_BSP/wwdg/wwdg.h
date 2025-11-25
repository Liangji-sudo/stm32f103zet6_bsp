#ifndef __WWDG_H
#define __WWDG_H

#include "stm32f1xx.h"

void wwdg_init(uint8_t tr, uint8_t wr, uint32_t fprer);


#endif