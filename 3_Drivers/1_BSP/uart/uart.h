#ifndef __UART_H
#define __UART_H

#include "stm32f1xx.h"

#define UART_TX_GPIO_PORT GPIOC
#define UART_RX_GPIO_PORT GPIOC

#define UART_TX_GPIO_PIN GPIO_PIN_10
#define UART_RX_GPIO_PIN GPIO_PIN_11

#define BSP_UART4_IRQHandler UART4_IRQHandler

void uart_init(uint32_t baud);



#endif