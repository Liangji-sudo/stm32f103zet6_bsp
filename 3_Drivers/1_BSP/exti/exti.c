#include "exti.h"
#include "led/led.h";

/*
外部gpio中断，涉及：由下至上，涉及：GPIO(IO), AFIO, EXTI, NVIC 这几个主要的寄存器
GPIO(io)
AFIO ==> EXTI
NVIC
*/



/*
KEY0  PE4/TRACED1/FSMC_A20
PE4 EXTI4
*/
void key0_init()
{
    //使能时钟，NVIC在一开始就默认打开时钟了, EXTI时钟不需要配置，hal库没有就不需要
    __HAL_RCC_GPIOE_CLK_ENABLE();
    //__HAL_RCC_AFIO_CLK_ENABLE(); //AFIO时钟在HAL_GPIO_Init中已经实现了，可写可不写
    
    //config gpio（afio）
    GPIO_InitTypeDef gpio_init_struct;
    gpio_init_struct.Pin = KEY0_INT_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_IT_FALLING; // 指定ext i mode上升沿触发
    gpio_init_struct.Pull = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(KEY0_INT_GPIO_PORT, &gpio_init_struct);


    //config exti route(use AFIO register)
    // this has been done in the HAL_GPIO_Init

    //设置NVIC的中断优先级，并使能
    HAL_NVIC_SetPriority(KEY0_INT_IRQn, 0, 2);
    HAL_NVIC_EnableIRQ(KEY0_INT_IRQn);
}

//重载NVIC的对EXTI4_IRQn 的中断处理函数：weak EXTI4_IRQHandler
void BSP_EXTI4_IRQHandler()
{
    //一般调用HAL的的GPIO 的 EXTI 的HAL中断处理函数（现在依然是处理函数），先关中断，然后调用回调函数（在你自己的程序中重载weak 回调函数）
    HAL_GPIO_EXTI_IRQHandler(KEY0_INT_GPIO_PIN);

    //HAL的中断处理函数，一定要进去看一下，结合中断标志位寄存器，看看是否有开中断。
    __HAL_GPIO_EXTI_CLEAR_IT(KEY0_INT_GPIO_PIN);
}






void key1_init()
{
    //enable gpio afio exti nvic clk
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_AFIO_CLK_ENABLE();

    //config gpio and afio
    GPIO_InitTypeDef gpio_init_struct;
    gpio_init_struct.Pin = KEY1_INT_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_IT_FALLING; // 指定ext i mode上升沿触发
    gpio_init_struct.Pull = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(KEY1_INT_GPIO_PORT, &gpio_init_struct);

    //config and enable nvic
    HAL_NVIC_SetPriority(KEY1_INT_IRQn, 0, 2);
    HAL_NVIC_EnableIRQ(KEY1_INT_IRQn);
}

void BSP_EXTI3_IRQHandler()
{
    HAL_GPIO_EXTI_IRQHandler(KEY1_INT_GPIO_PIN);
    __HAL_GPIO_EXTI_CLEAR_IT(KEY1_INT_GPIO_PIN);
}