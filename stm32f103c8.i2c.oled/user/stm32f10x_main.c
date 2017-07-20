#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "bsp_led.h"
#include "bsp_i2c.h"
#include "bsp_oled.h"

int main(void)
{
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        led_init();
        usart_init();
        systick_init();
        i2c_init();
        oled_init();
        while(1)
        {
                
        }
}
