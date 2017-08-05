#include "bsp_led.h"

/**
 * @brief 初始化板载 LED
 * @return null
 */
void led_init(void)
{
        GPIO_InitTypeDef GPIO_InitStructure;

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOC, &GPIO_InitStructure);
}
