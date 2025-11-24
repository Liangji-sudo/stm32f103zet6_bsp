/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-20
 * @brief       跑马灯 实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 STM32F103开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "sys/sys.h"
#include "delay/delay.h"
#include "usart/usart.h"



#define LED_ENABLE_FLAG;


//init : HAL, SYS_CLK, delay, usart_0
void system_init()
{
    HAL_Init();                                 /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);         /* 设置时钟,72M */
    delay_init(72);                             /* 初始化延时函数 */
		usart_init(115200);
}

void bsp_init()
{
#ifdef LED_ENABLE_FLAG
    #include "led/led.h";
    led0_init(); // LED0  PB5
    led1_init(); // LED1  PE5
#endif 
}


int main(void)
{
		system_init();
    bsp_init();
	
    while(1)
    {
				printf("hello world\n");
        LED0_TOGGLE();
        LED1_TOGGLE();
        delay_ms(500);
    }
}

