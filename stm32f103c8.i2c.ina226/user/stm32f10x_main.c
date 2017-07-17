#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "bsp_led.h"
#include "bsp_i2c.h"
#include "bsp_ina226.h"

u8 buffer[256];

int main(void)
{
        int num = 0;
        u16 current = 0;
        
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        systick_init();
        usart_init();
        i2c_init();
        led_init();
        ina226_init();

        while(1)
        {
                ina226_read_register_data(0x01, buffer);
                current = buffer[0] * 256 + buffer[1];
                printf("current = %d \r\n", current);
                printf("I = %f  --------- %d\r\n\r\n", (float)current / 4.48, num++);
                delay_ms(500);
                LED = ON;
                delay_ms(500);
                LED = OFF;
        }
}
