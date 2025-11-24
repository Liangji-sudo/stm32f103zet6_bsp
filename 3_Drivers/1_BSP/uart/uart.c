#include "uart.h"
/*
UART4
TX PC10
RX PC11
*/

#define RX_Buffer_Len 128
uint8_t RX_Buffer[RX_Buffer_Len] = {0};
uint8_t TX_array[4] = {0x63, 0x6F, 0x70, 0x79};

UART_HandleTypeDef uart_struct_init;
void uart_init(uint32_t baud)
{

    //时钟
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_AFIO_CLK_ENABLE();
    __HAL_RCC_UART4_CLK_ENABLE();

    //io配置 + io复用
    //Tx
    GPIO_InitTypeDef gpio_Init;
    gpio_Init.Pin = UART_TX_GPIO_PIN;
    gpio_Init.Mode = GPIO_MODE_AF_PP;
    gpio_Init.Pull = GPIO_PULLUP;
    gpio_Init.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(UART_TX_GPIO_PORT, &gpio_Init);

    //Rx
    gpio_Init.Pin = UART_RX_GPIO_PIN;
    gpio_Init.Mode = GPIO_MODE_AF_INPUT; 
    HAL_GPIO_Init(UART_TX_GPIO_PORT, &gpio_Init);

    //uart配置

    uart_struct_init.Instance = UART4;
    uart_struct_init.Init.BaudRate = baud;
    uart_struct_init.Init.WordLength = UART_WORDLENGTH_8B;                      /* 字长为8位数据格式 */
    uart_struct_init.Init.StopBits = UART_STOPBITS_1;                           /* 一个停止位 */
    uart_struct_init.Init.Parity = UART_PARITY_NONE;                            /* 无奇偶校验位 */
    uart_struct_init.Init.HwFlowCtl = UART_HWCONTROL_NONE;  
    uart_struct_init.Init.Mode = UART_MODE_TX_RX;
    HAL_UART_Init(&uart_struct_init);

    //NVIC响应
    HAL_NVIC_SetPriority(UART4_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(UART4_IRQn);

    //USART1 接收中断
    HAL_UART_Receive_IT(&uart_struct_init, RX_Buffer, RX_Buffer_Len);
}


//重载接收中断处理函数
void BSP_USART1_IRQHandler()
{
    //习惯性调用HAL库来实现中断处理函数, 里面调用回调函数来接受，关中断
    HAL_UART_IRQHandler(&uart_struct_init);

    //开中断
    HAL_UART_Receive_IT(&uart_struct_init, RX_Buffer, RX_Buffer_Len);
}

//重载HAL库的接收中断回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    
    for(int i = 0; i< RX_Buffer_Len; i++)
    {
        if(RX_Buffer[i] == 0xaa){
            HAL_UART_Transmit_IT(huart, TX_array, 4);
        }
    }
}
