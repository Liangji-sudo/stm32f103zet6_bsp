#ifndef __TIM_H
#define __TIM_H

#include "stm32f1xx.h"

//#define TIM_COMMON_4_PULSE_COUNT
#define TIM_BASE_UPDATE_INT



/**
 * Advanced Timer: TIM1~TIM8
 * 
 */

//TODO:


/**
 * Common Timer: TIM2~TIM5
 */
#ifdef TIM_COMMON_4_PULSE_COUNT
void liangji_init_TIM5_Pulse(uint32_t psc, uint32_t period);
uint32_t get_count(void);
void cnt_restart(void);

#endif

#ifdef TIM_COMMON_3_Capture
void liangji_init_TIM5_CAPTURE(uint32_t psc, uint32_t period);
void liangji_TIM5_Capture(void);
#endif

#ifdef TIM_COMMON_2_PWM_OUTPUT
void liangji_init_TIM3_PWM(uint32_t psc, uint32_t period, uint32_t pulse);
void liangji_pwm_led(uint32_t max);
#endif

#ifdef TIM_COMMON_1_BASE_UPDATE_INT
void liangji_init_TIM3(uint32_t psc, uint32_t period);
#endif



/*
Base Timer: TIM6, TIM7
TIM6
*/
#ifdef TIM_BASE_UPDATE_INT
void TIM6_init(uint32_t psc, uint32_t period);
#endif

#endif