#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "bsp_led.h"
#include "bsp_i2c.h"
#include "bsp_oled.h"

char *p1 = "Hello world!";
char *p2 = "I love you!";

int main(void)
{
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        led_init();
        usart_init();
        systick_init();
        i2c_init();
        oled_init();
        oled_display_on();
        
        oled_draw_string(0, 0, p1);
        
        oled_draw_string(0, 20, p2);
        
        oled_refresh_gram();
        
        while(1)
        {
                LED = ON;
                delay_ms(200);
                LED = OFF;
                delay_ms(800);
        }
}
