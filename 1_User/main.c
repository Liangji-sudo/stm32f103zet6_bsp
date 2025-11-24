#include "sys/sys.h"
#include "delay/delay.h"
#include "usart/usart.h"
#include "led/led.h"
#include "exti/exti.h"


extern uint8_t Rx_flag;
//init : HAL, SYS_CLK, delay, usart_0
void system_init()
{
    HAL_Init();                                 /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);         /* 设置时钟,72M */
    delay_init(72);                             /* 初始化延时函数 */
	usart_init(115200);
}

int main(void)
{
	system_init();
	

	led0_init(); // LED0  PB5
    led1_init(); // LED1  PE5
    key0_init(); // exti4 PE4 KEY0
    key1_init(); // exti3 PE3 KEY1
    //uart_init(115200);
	
		uint32_t count = 0;
	
    while(1)
    {
			printf("program count = %d\n", count);
			count++;
        if(Rx_flag){
            LED0_TOGGLE();
            Rx_flag = 0;
        }
        delay_ms(500);
    }
}


//重载HAL库的EXTI中断回调函数
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == KEY0_INT_GPIO_PIN)
    {
        printf("test");
        LED0_TOGGLE();
    }

    if(GPIO_Pin == KEY1_INT_GPIO_PIN)
    {
        printf("test2");
        LED1_TOGGLE();
    }
}

