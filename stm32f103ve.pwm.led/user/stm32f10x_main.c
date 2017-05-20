#include "stm32f10x.h"
#include "bsp_led.h"

int main(void)
{
        led_init();
        
        while(1)
        {
                led_light_color(0x0000ff);
        }
}
