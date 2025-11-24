
#ifndef __LED_H
#define __LED_H

#include "stm32f1xx.h"

#define LED0_GPIO_PORT GPIOB
#define LED0_GPIO_PIN GPIO_PIN_5

#define LED1_GPIO_PORT GPIOE
#define LED1_GPIO_PIN GPIO_PIN_5

#define LED_ON 1
#define LED_OFF 0


/* LED0 = !LED0 */
#define LED0_TOGGLE() do{ HAL_GPIO_TogglePin(LED0_GPIO_PORT, LED0_GPIO_PIN); \
                        }while(0) 

#define LED1_TOGGLE() do{ HAL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_GPIO_PIN); \
                        }while(0) 





//LED0 ----  PB5
void led0_init();
//LED1 ----  PE5
void led1_init();


void led0_set(int value);
void led1_set(int value);

#endif
