#include "tim.h"


//TIM COMMON-4 - Pulse count Project
#ifdef TIM_COMMON_4_PULSE_COUNT

TIM_HandleTypeDef init_TIM5_pulse;
GPIO_InitTypeDef init_gpio_pulse;
TIM_SlaveConfigTypeDef init_TIM5_pulse_slave;
TIM_IC_InitTypeDef init_IC_pulse;
#define liangji_TIM5_pulse_IRQHandler TIM5_IRQHandler

uint16_t g_timxchy_cnt_ofcnt = 0;

void liangji_init_TIM5_Pulse(uint32_t psc, uint32_t period)
{
		//NVIC
	HAL_NVIC_SetPriority(TIM5_IRQn,2,3);
	HAL_NVIC_EnableIRQ(TIM5_IRQn);
	
		//IO
	__HAL_RCC_GPIOA_CLK_ENABLE();
	init_gpio_pulse.Pin = GPIO_PIN_0;
	init_gpio_pulse.Mode = GPIO_MODE_AF_PP;
	init_gpio_pulse.Speed = GPIO_SPEED_FREQ_HIGH;
	init_gpio_pulse.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOA, &init_gpio_pulse);

	
	//enable TIM5 CLK
	__HAL_RCC_TIM5_CLK_ENABLE();
	
	//TIM5 TIM BASE
	init_TIM5_pulse.Instance = TIM5;
	init_TIM5_pulse.Init.Prescaler=psc;
	init_TIM5_pulse.Init.CounterMode=TIM_COUNTERMODE_UP;
	init_TIM5_pulse.Init.Period=period;
	init_TIM5_pulse.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	init_TIM5_pulse.Init.AutoReloadPreload=TIM_AUTORELOAD_PRELOAD_DISABLE;
	init_TIM5_pulse.Channel = HAL_TIM_ACTIVE_CHANNEL_1;
	HAL_TIM_IC_Init(&init_TIM5_pulse);
	
	//TIM5 CLK SOURCE SEL, Controller
	init_TIM5_pulse_slave.SlaveMode=TIM_SLAVEMODE_EXTERNAL1;
	init_TIM5_pulse_slave.InputTrigger=TIM_TS_TI1FP1;
	init_TIM5_pulse_slave.TriggerPolarity=TIM_TRIGGERPOLARITY_RISING;
	init_TIM5_pulse_slave.TriggerFilter=0;
	init_TIM5_pulse_slave.TriggerPrescaler=TIM_TRIGGERPRESCALER_DIV1;
	HAL_TIM_SlaveConfigSynchro(&init_TIM5_pulse, &init_TIM5_pulse_slave);
	
	//TIM5 Capture
	init_IC_pulse.ICFilter = 0;//dont use filter
	init_IC_pulse.ICPrescaler = TIM_ICPSC_DIV1;
	init_IC_pulse.ICSelection = TIM_ICSELECTION_DIRECTTI;
	init_IC_pulse.ICPolarity = TIM_ICPOLARITY_RISING;
	HAL_TIM_IC_ConfigChannel(&init_TIM5_pulse, &init_IC_pulse, TIM_CHANNEL_1);

	

	//TIM5 Capture IT and IT
	//HAL_TIM_IC_Start_IT(&init_TIM5_pulse, TIM_CHANNEL_1);
	HAL_TIM_Base_Start_IT(&init_TIM5_pulse);
	
}

void liangji_TIM5_pulse_IRQHandler(void)
{
		//HAL_TIM_IRQHandler(&init_TIM5_pulse);
	if(__HAL_TIM_GET_FLAG(&init_TIM5_pulse, TIM_FLAG_UPDATE) != RESET){
		//have update event int
		g_timxchy_cnt_ofcnt++; //count the times that input pulse cnt = 0xffff = arr, N
	}
	__HAL_TIM_CLEAR_FLAG(&init_TIM5_pulse, TIM_FLAG_UPDATE);
}

uint32_t get_count(void)
{
	uint32_t count = 0;
	count = g_timxchy_cnt_ofcnt * 65536; // ofcnt = N, 65536 = 0xffff = arr(max)(16bit) //count == all pulse input
	count += __HAL_TIM_GET_COUNTER(&init_TIM5_pulse);
	return count;
}
void cnt_restart(void)
{
	__HAL_TIM_DISABLE(&init_TIM5_pulse);
	g_timxchy_cnt_ofcnt = 0;
	__HAL_TIM_SET_COUNTER(&init_TIM5_pulse, 0);
	__HAL_TIM_ENABLE(&init_TIM5_pulse);
}



#endif






















//TIM COMMON-3 - Capture Project
//PA0 -> TIM5_CH1
#ifdef TIM_COMMON_3_Capture

TIM_HandleTypeDef init_TIM5;
GPIO_InitTypeDef init_gpio_capture;
TIM_IC_InitTypeDef init_IC;
#define liangji_COMMON3_TIM5_IRQHandler TIM5_IRQHandler
uint8_t g_timxchy_cap_sta = 0; //this count N
uint16_t g_timxchy_cap_val = 0; //count Once cnt < arr
uint16_t count_up = 0;


void liangji_init_TIM5_CAPTURE(uint32_t psc, uint32_t period)
{
	//NVIC
	HAL_NVIC_SetPriority(TIM5_IRQn,2,3);
	HAL_NVIC_EnableIRQ(TIM5_IRQn);
	
	//IO
	__HAL_RCC_GPIOA_CLK_ENABLE();
	init_gpio_capture.Pin = GPIO_PIN_0;
	init_gpio_capture.Mode = GPIO_MODE_AF_PP;
	init_gpio_capture.Speed = GPIO_SPEED_FREQ_HIGH;
	init_gpio_capture.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOA, &init_gpio_capture);

	
	//enable TIM5 CLK
	__HAL_RCC_TIM5_CLK_ENABLE();
	
	//TIM5 CLK SOURCE SEL
	//stay Default
	
	//TIM5 TIM BASE
	init_TIM5.Instance = TIM5;
	init_TIM5.Init.Prescaler=psc;
	init_TIM5.Init.CounterMode=TIM_COUNTERMODE_UP;
	init_TIM5.Init.Period=period;
	init_TIM5.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	init_TIM5.Init.AutoReloadPreload=TIM_AUTORELOAD_PRELOAD_DISABLE;
	init_TIM5.Channel = HAL_TIM_ACTIVE_CHANNEL_1;
	HAL_TIM_IC_Init(&init_TIM5);
	
	//TIM5 Capture
	init_IC.ICFilter = 0;//dont use filter
	init_IC.ICPrescaler = TIM_ICPSC_DIV1;
	init_IC.ICSelection = TIM_ICSELECTION_DIRECTTI;
	init_IC.ICPolarity = TIM_ICPOLARITY_RISING;
	HAL_TIM_IC_ConfigChannel(&init_TIM5, &init_IC, TIM_CHANNEL_1);
	
	//TIM5 Capture IT and IT
	HAL_TIM_IC_Start(&init_TIM5, TIM_CHANNEL_1);
	HAL_TIM_Base_Start_IT(&init_TIM5);
}

void liangji_TIM5_Capture(void)
{
	if((g_timxchy_cap_sta & 0x80)!= 0)
	{
		//we get a pulse
		uint16_t N = g_timxchy_cap_sta & 0x3F;
		N*= 65536;
		N+=g_timxchy_cap_val;
		printf("HIGH PULSE : %d us \r\n", N);
		g_timxchy_cap_sta = 0;
	}
	
	//just LED0 TOGGLE
	static uint8_t t = 0;
	t++;
	if(t>20){
		t=0;
		LED0_TOGGLE();
	}
	
}

void liangji_COMMON3_TIM5_IRQHandler(void)
{
		HAL_TIM_IRQHandler(&init_TIM5);
}

/*
			Update IT callback
			impel in main, toggle led1, and count up ++
      HAL_TIM_PeriodElapsedCallback(htim);
*/

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM6){
		//LED1_TOGGLE();
	} else if (htim->Instance == TIM3){
		//LED1_TOGGLE();
	} else if (htim->Instance == TIM5) {
			if((g_timxchy_cap_sta & 0X80) == 0){
					if(g_timxchy_cap_sta & 0X40){
							if ((g_timxchy_cap_sta & 0X3F) == 0X3F){
								//  wait for falling, over the N max, is too long,think it is success, so re-wait for up, give up wait falling
								TIM_RESET_CAPTUREPOLARITY(&init_TIM5, TIM_CHANNEL_1);
								TIM_SET_CAPTUREPOLARITY(&init_TIM5, TIM_CHANNEL_1, TIM_ICPOLARITY_RISING);
								g_timxchy_cap_sta |= 0x80;     
								g_timxchy_cap_val = 0xffff; //mean: N(max)*arr + N(max)
							}
							else {
								g_timxchy_cap_sta++;  //N +1
							}
					}
			
			}
	}
}

/*
      HAL_TIM_IC_CaptureCallback(htim);
*/
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	//get up cnt
	//set fall detect
	if((g_timxchy_cap_sta & 0X80) == 0){
		if(g_timxchy_cap_sta & 0X40){ //now 0 1, but now capture falling because when capture up, we detect falling
			g_timxchy_cap_sta |= 0x80;
			g_timxchy_cap_val = HAL_TIM_ReadCapturedValue(&init_TIM5, TIM_CHANNEL_1);//get falling cnt t2 (t2<arr)
			//reset detect up
			TIM_RESET_CAPTUREPOLARITY(&init_TIM5, TIM_CHANNEL_1);
			TIM_SET_CAPTUREPOLARITY(&init_TIM5, TIM_CHANNEL_1, TIM_ICPOLARITY_RISING);
		} else { //now 0 0, wait for up edge, but now capture interupt, so start a new detect
			g_timxchy_cap_sta = 0;
			g_timxchy_cap_val = 0;
			g_timxchy_cap_sta |= 0x40;   // set 0 1, we have get up edge
			//set polo falling detect, no need to record now cnt t1, because t1 + N*arr + t2 = now
			__HAL_TIM_DISABLE(&init_TIM5);
			__HAL_TIM_SET_COUNTER(&init_TIM5, 0); 
			TIM_RESET_CAPTUREPOLARITY(&init_TIM5, TIM_CHANNEL_1);			
			TIM_SET_CAPTUREPOLARITY(&init_TIM5, TIM_CHANNEL_1, TIM_ICPOLARITY_RISING);
			__HAL_TIM_ENABLE(&init_TIM5);
		}
	}
}

#endif

























//TIM COMMON-2 - PWM Project
// TIM3 CH2 PA7 REMAP to PB5
//TIM3_init_PWM(72-1, 20000-1, 2500);//72M/72 = 1Mhz   ARR = 500; PWM = 2000hz; ARR = 20000; PWM = 50hz 
#ifdef TIM_COMMON_2_PWM_OUTPUT

TIM_HandleTypeDef init_TIM3;
TIM_OC_InitTypeDef init_TIM3_OC;
GPIO_InitTypeDef init_gpio7;
void TIM3_init_PWM(uint32_t psc, uint32_t period, uint32_t pulse)
{
	//enable clk
	__HAL_RCC_TIM3_CLK_ENABLE();
	
	//enable NVIC
	HAL_NVIC_SetPriority(TIM3_IRQn,2,3);
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
	
	//io mux, PA7->TIM3_CH2, REMAP to PB5
	__HAL_RCC_GPIOB_CLK_ENABLE();
	init_gpio7.Pin = GPIO_PIN_5;
	init_gpio7.Mode = GPIO_MODE_AF_PP;
	init_gpio7.Speed = GPIO_SPEED_FREQ_HIGH;
	init_gpio7.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &init_gpio7);
	
	//GTIM_TIMX_PWM_CHY_GPIO_REMAP();
	__HAL_RCC_AFIO_CLK_ENABLE();
	__HAL_AFIO_REMAP_TIM3_PARTIAL();
	//AFIO_BASE
	
	
	//CLK src sel, default choose ck_int
	
	
	//config TIM BASE
	init_TIM3.Instance = TIM3;
	init_TIM3.Init.Prescaler=psc;
	init_TIM3.Init.CounterMode=TIM_COUNTERMODE_UP;
	init_TIM3.Init.Period=period;
	init_TIM3.Init.AutoReloadPreload=TIM_AUTORELOAD_PRELOAD_DISABLE;
	//init_TIM3.Channel = HAL_TIM_ACTIVE_CHANNEL_2;
	HAL_TIM_PWM_Init(&init_TIM3);
	
	//config TIM compare and output
	init_TIM3_OC.OCMode=TIM_OCMODE_PWM1;
	//init_TIM3_OC.Pulse=period/2;
	init_TIM3_OC.Pulse=pulse;
	init_TIM3_OC.OCPolarity=TIM_OCPOLARITY_LOW;
	HAL_TIM_PWM_ConfigChannel(&init_TIM3, &init_TIM3_OC, TIM_CHANNEL_2);
	
	//TIM3 IT
	//HAL_TIM_Base_Start_IT(&init_TIM3);
	HAL_TIM_PWM_Start(&init_TIM3, TIM_CHANNEL_2);

}

//max为PWM的led输出的最亮的比较值， 不停的变换比较值，实现LED呼吸灯, 需要放到while循环里面更新
void TIM3_set_led_max_val(uint32_t max)
{
	static uint8_t dir = 1;
	static uint16_t ledrpwmval = 0;
	
	if (dir)
		ledrpwmval++;
	else
		ledrpwmval--;
		
	//switch
	if (ledrpwmval > max)
		dir = 0;
	if (ledrpwmval == 0)
		dir = 1;

	__HAL_TIM_SET_COMPARE(&init_TIM3, TIM_CHANNEL_2, ledrpwmval);

}
#endif































// TIM COMMON-1 - INT Project
//TIM3

#ifdef TIM_COMMON_1_BASE_UPDATE_INT

#define liangji_TIM3_IRQHandler TIM3_IRQHandler

//传递更新事件中断信号
uint8_t TIM3_BASE_Update_Int_flag = 0;
TIM_HandleTypeDef init_TIM3;

void TIM3_init_BASE(uint32_t psc, uint32_t period)
{
		//enable clk
	__HAL_RCC_TIM3_CLK_ENABLE();
	
	//enable NVIC
	HAL_NVIC_SetPriority(TIM3_IRQn,2,3);
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
	
	//CLK src sel, default choose ck_int
	
	
	//config TIM BASE
	init_TIM3.Instance = TIM3;
	init_TIM3.Init.Prescaler=psc;
	init_TIM3.Init.CounterMode=TIM_COUNTERMODE_UP;
	init_TIM3.Init.Period=period;
	init_TIM3.Init.AutoReloadPreload=TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&init_TIM3);
	
	//TIM3 IT
	HAL_TIM_Base_Start_IT(&init_TIM3);
	
}

//重载NVIC的中断处理函数，但是我们不使用HAL的通用的中断处理函数，我们自己靠寄存器实现，读取更新事件中断，清中断
void liangji_TIM3_IRQHandler(void)
{
	//HAL_TIM_IRQHandler(&init_TIM3);
	if(__HAL_TIM_GET_FLAG(&init_TIM3, TIM_FLAG_UPDATE))
	{
		//LED1_TOGGLE();
		TIM3_BASE_Update_Int_flag = 1-TIM3_BASE_Update_Int_flag;
		__HAL_TIM_CLEAR_FLAG(&init_TIM3, TIM_FLAG_UPDATE);
	}
}
#endif

























/**
 * 基础定时器 Base Timer(TIM6, TIM7)
 * 正计数，产生更新事件中断
 * 
 * */
#ifdef TIM_BASE_UPDATE_INT

TIM_HandleTypeDef init_TIM;
#define liangji_TIM6_IRQHandler TIM6_IRQHandler
void TIM6_init(uint32_t psc, uint32_t period)
{
	//enable clk
	__HAL_RCC_TIM6_CLK_ENABLE();
	
	//enable NVIC
	HAL_NVIC_SetPriority(TIM6_IRQn,2,3);
	HAL_NVIC_EnableIRQ(TIM6_IRQn);
	
	//config tim reg
	init_TIM.Instance = TIM6;
	init_TIM.Init.Prescaler=psc;//7200 divf
	init_TIM.Init.CounterMode=TIM_COUNTERMODE_UP;
	init_TIM.Init.Period=period;//5000 ~ 500ms
	//init_TIM.Init.ClockDivision=TIM_CLOCKDIVISION_DIV2;//72M
	init_TIM.Init.AutoReloadPreload=TIM_AUTORELOAD_PRELOAD_DISABLE;

	HAL_TIM_Base_Init(&init_TIM);
	
	//enable TIM6, and enable TIM6_IT
	HAL_TIM_Base_Start_IT(&init_TIM);
}

//重载的定时器中断处理函数，调用HAL的中断处理函数来处理， 里面会清除中断
void liangji_TIM6_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&init_TIM);
}

/**
 * 
 * 定时器的中断回调函数，在更新事件中断回调函数中，

 * 1. 自己xxx操作

 */
/*
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	//__HAL_TIM_DISABLE(htim);
	//__HAL_TIM_DISABLE_IT (htim, TIM_IT_UPDATE);
	
	//do your thing
    if(htim->Instance == TIM6)
	    LED1_TOGGLE();
	//__HAL_TIM_ENABLE(htim);
	//__HAL_TIM_ENABLE_IT(htim, TIM_IT_UPDATE);
}
*/
#endif

