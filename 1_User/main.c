#include "sys/sys.h"
#include "delay/delay.h"
#include "usart/usart.h"
#include "led/led.h"
#include "exti/exti.h"
#include "iwdg/iwdg.h"
#include "wwdg/wwdg.h"
#include "tim/tim.h"


//#define WDG_DBG
#define UART_DBG
//#define BASE_TIM_DBG
//#define TIM3_BASE_DBG
#define TIM3_PWM_DBG


#ifdef UART_DBG
extern uint8_t USART1_Rx_flag;
extern uint8_t UART4_Rx_flag;
#endif

#ifdef TIM3_BASE_DBG
extern uint8_t TIM3_BASE_Update_Int_flag;
#endif

//init : HAL, SYS_CLK, delay, usart_0
void system_init()
{
    HAL_Init();                                 /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);         /* 设置时钟,72M */
    delay_init(72);                             /* 初始化延时函数 */

    //UART1(PA9, PA10), UART4(PC10, PC11)
	usart_init(115200);
    uart4_init(115200);

    //LED0(PB5) LED1(PE5)
    #ifndef TIM3_PWM_DBG
    led0_init(); // LED0  PB5
    #endif
    led1_init(); // LED1  PE5

    //GPIO + EXTI (KEY0 PE4, KEY1 PE3)
    key0_init(); // exti4 PE4 KEY0
    key1_init(); // exti3 PE3 KEY1

    //Watch Dog
    #ifdef WDG_DGB
    iwdg_init(IWDG_PRESCALER_64,625); //64分频，上限值为625
    wwdg_init(0X7F, 0X5F, WWDG_PRESCALER_8);//8分频，重载值0x75, 上限值0x50,下限值0x3f
    #endif

    #ifdef BASE_TIM_DBG
    TIM6_init(7200-1, 5000-1); //set psc 7200, arr auto reload = 5000 = 500ms
    #endif

    #ifdef TIM3_BASE_DBG
    TIM3_init_BASE(7200-1, 5000-1); //set psc 7200, arr auto reload = 5000 = 500ms
    #endif

    //TIM3 PWM remap to PB5, so disable LED0
    #ifdef TIM3_PWM_DBG
    TIM3_init_PWM(72-1, 20000-1, 15000);//72M/72 = 1Mhz   ARR = 500; PWM = 2000hz; ARR = 20000; PWM = 50hz 
    #endif

}

int main(void)
{
	system_init();
	
	uint32_t count = 0;
	
    while(1)
    {
		//printf("program count = %d\n", count);
		count++;

        #ifdef UART_DBG
        if(USART1_Rx_flag){
            LED0_TOGGLE();
            USART1_Rx_flag = 0;
        }
        if(UART4_Rx_flag){
            LED1_TOGGLE();
            UART4_Rx_flag = 0;
        }
        #endif
        
        #ifdef TIM3_BASE_DBG
        LED0_TOGGLE();
        #endif

        #ifdef TIM3_PWM_DBG
        TIM3_set_led_max_val(15000);
        #endif

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


        #ifdef WDG_DGB
        iwdg_feed();
        #endif
    }

    if(GPIO_Pin == KEY1_INT_GPIO_PIN)
    {
        printf("test2");
        LED1_TOGGLE();
    }
}


//窗口看门狗倒计时到下限的提前唤醒中断的回调函数
#ifdef WDG_DGB
void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef *hwwdg)
{
	HAL_WWDG_Refresh(hwwdg);
	LED1_TOGGLE();
	//printf("Refresh WWDG\n");
}
#endif


#ifdef BASE_TIM_DBG
/**
 * 
 * 定时器的中断回调函数，在更新事件中断回调函数中，

 * 1. 自己xxx操作

 */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	//do your thing
    if(htim->Instance == TIM6)
	    LED1_TOGGLE();
}
#endif