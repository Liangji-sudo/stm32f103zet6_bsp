#ifndef __EXTI_H
#define __EXTI_H

#include "stm32f1xx.h"

//KEY0 PE4 EXTI4
#define KEY0_INT_GPIO_PORT GPIOE
#define KEY0_INT_GPIO_PIN GPIO_PIN_4
#define KEY0_INT_IRQn EXTI4_IRQn
#define BSP_EXTI4_IRQHandler EXTI4_IRQHandler 


//KEY1 PE3 EXTI3
#define KEY1_INT_GPIO_PORT GPIOE
#define KEY1_INT_GPIO_PIN GPIO_PIN_3
#define KEY1_INT_IRQn EXTI3_IRQn
#define BSP_EXTI3_IRQHandler EXTI3_IRQHandler

void key0_init();
void key1_init();

#endif