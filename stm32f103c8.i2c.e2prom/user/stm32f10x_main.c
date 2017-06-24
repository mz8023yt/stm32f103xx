#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "bsp_led.h"
#include "bsp_i2c.h"
#include "bsp_at24c256.h"

u8 buffer[256];

int main(void)
{
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        systick_init();
        usart_init();
        i2c_init();
        led_init();
        
        WP = OFF;
        
        buffer[0] = 18;
        at24c256_byte_write(0x1020, buffer);
        printf("[%s][%s][%d]: write successful\r\n", __FILE__, __FUNCTION__, __LINE__);
        
        buffer[0] = 200;
        at24c02_random_read(0x1020, buffer);
        printf("[%s][%s][%d]: data = %d\r\n", __FILE__, __FUNCTION__, __LINE__, buffer[0]);
        
        while(1)
        {
                
        }
}
