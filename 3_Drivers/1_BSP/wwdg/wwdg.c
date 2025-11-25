#include "wwdg.h"

//#define USE_HAL_WWDG_REGISTER_CALLBACKS 1
#define My_WWDG_IRQHandler WWDG_IRQHandler


//tr 重置的计数值（可以大于上限值，因为上限值只和喂狗时间有关，也就是说不能在tr>上限值的时候喂狗就行）
//wr 窗口值（上限值）
WWDG_HandleTypeDef init_wwdg;
void wwdg_init(uint8_t tr, uint8_t wr, uint32_t fprer)
{

	//ENABLE PCLK, you must enable clk first, and then you can config the reg
	__HAL_RCC_WWDG_CLK_ENABLE();

	//init WWDG REG
	init_wwdg.Instance=WWDG;
	init_wwdg.Init.Prescaler = fprer;
	init_wwdg.Init.Window = wr;
	init_wwdg.Init.Counter = tr;
	init_wwdg.Init.EWIMode = WWDG_EWI_ENABLE; //使能提前唤醒中断
	HAL_WWDG_Init(&init_wwdg);
	
		//enable NVIC WWDG_IRQ
	HAL_NVIC_SetPriority(WWDG_IRQn,2,3);
	HAL_NVIC_EnableIRQ(WWDG_IRQn);
}

//band my IRQ to reload startup.s WWDG_IRQHandler. now Tr is 0x40
void My_WWDG_IRQHandler(void)
{
	//we use HAL_IRQHandler, in this , hal will check the SR flag if valid
	HAL_WWDG_IRQHandler(&init_wwdg);
}

/*
TODO: this function shoule be write in main.c
TODO: callback function has been used in HAL_IRQHandler, you need do something in this, feeddog
void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef *hwwdg)
{
	HAL_WWDG_Refresh(hwwdg);
	LED1_TOGGLE();
	//printf("Refresh WWDG\n");
}
*/


