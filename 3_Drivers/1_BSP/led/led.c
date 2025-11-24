#include"led.h"


//LED0 ----  PB5
void led0_init()
{
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef gpio_init;
    gpio_init.Pin = LED0_GPIO_PIN;
    gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull = GPIO_PULLUP;
    gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(LED0_GPIO_PORT, &gpio_init);

}

void led1_init()
{
    __HAL_RCC_GPIOE_CLK_ENABLE();

    GPIO_InitTypeDef gpio_init;
    gpio_init.Pin = LED1_GPIO_PIN;
    gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull = GPIO_PULLUP;
    gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(LED1_GPIO_PORT, &gpio_init);
}


void led0_set(int value)
{
    HAL_GPIO_WritePin(LED0_GPIO_PORT, LED0_GPIO_PIN, value);
}

void led1_set(int value)
{
    HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_GPIO_PIN, value);
}