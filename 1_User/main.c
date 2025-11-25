#include "sys/sys.h"
#include "delay/delay.h"
#include "usart/usart.h"
#include "led/led.h"
#include "exti/exti.h"
#include "iwdg/iwdg.h"
#include "wwdg/wwdg.h"


extern uint8_t USART1_Rx_flag;
extern uint8_t UART4_Rx_flag;
//init : HAL, SYS_CLK, delay, usart_0
void system_init()
{
    HAL_Init();                                 /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);         /* 设置时钟,72M */
    delay_init(72);                             /* 初始化延时函数 */
	usart_init(115200);
    uart4_init(115200);
    led0_init(); // LED0  PB5
    led1_init(); // LED1  PE5
    key0_init(); // exti4 PE4 KEY0
    key1_init(); // exti3 PE3 KEY1
    iwdg_init(IWDG_PRESCALER_64,625); //64分频，上限值为625
    wwdg_init(0X7F, 0X5F, WWDG_PRESCALER_8);//8分频，重载值0x75, 上限值0x50,下限值0x3f
}

int main(void)
{
	system_init();
	
	uint32_t count = 0;
	
    while(1)
    {
		printf("program count = %d\n", count);
		count++;
        if(USART1_Rx_flag){
            LED0_TOGGLE();
            USART1_Rx_flag = 0;
        }
        if(UART4_Rx_flag){
            LED1_TOGGLE();
            UART4_Rx_flag = 0;
        }
        
        delay_ms(100);
    }
}


//重载HAL库的EXTI中断回调函数
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == KEY0_INT_GPIO_PIN)
    {
        printf("test");
        LED0_TOGGLE();
        iwdg_feed();
    }

    if(GPIO_Pin == KEY1_INT_GPIO_PIN)
    {
        printf("test2");
        LED1_TOGGLE();
    }
}


//窗口看门狗倒计时到下限的提前唤醒中断的回调函数
void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef *hwwdg)
{
	HAL_WWDG_Refresh(hwwdg);
	LED1_TOGGLE();
	//printf("Refresh WWDG\n");
}

