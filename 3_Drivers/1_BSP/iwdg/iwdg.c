#include "iwdg.h"

IWDG_HandleTypeDef myIwdg;

void iwdg_init(uint8_t prer, uint16_t rlr)
{
	myIwdg.Instance = IWDG;
	
	myIwdg.Init.Prescaler=prer; //分频系数
	myIwdg.Init.Reload=rlr; //预装载的倒计时数
	
	HAL_IWDG_Init(&myIwdg);
}
void iwdg_feed(void)
{
	HAL_IWDG_Refresh(&myIwdg);
}