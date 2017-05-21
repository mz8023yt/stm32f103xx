#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_led.h"

int main(void)
{
        led_init();
        systick_init();
        
        while(1)
        {
                led_light_color(0x0000ff);
                delay_ms(1000);
                led_light_color(0x00ff00);
                delay_ms(1000);
                led_light_color(0xff0000);
                delay_ms(1000);
        }
}
