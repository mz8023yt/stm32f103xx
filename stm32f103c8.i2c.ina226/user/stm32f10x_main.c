#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "bsp_led.h"
#include "bsp_i2c.h"
#include "bsp_ina226.h"

u8 buffer[256];

int main(void)
{
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        systick_init();
        usart_init();
        i2c_init();
        led_init();

        log("before runing");

        ina226_read_register_data(2, buffer);

        log("after runing");

        while(1)
        {
                LED = ON;
                delay_ms(100);
                
                LED = OFF;
                delay_ms(900);
        }
}
