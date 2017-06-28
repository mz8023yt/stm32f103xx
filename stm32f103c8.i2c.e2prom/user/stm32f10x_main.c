#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "bsp_led.h"
#include "bsp_i2c.h"
#include "bsp_at24c256.h"

u8 buffer[256];

int main(void)
{
        int ret = -1;
        
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        systick_init();
        usart_init();
        i2c_init();
        led_init();
        
        buffer[0] = 120;
        ret = at24c256_byte_write(0x1000, buffer);
        if(ret)
        {
                printf("[%s][%s][%d]:\r\nbyte write error!\r\n\r\n", __FILE__, __FUNCTION__, __LINE__);
        }
        else
        {
                printf("[%s][%s][%d]:\r\nwrite successful\r\n\r\n", __FILE__, __FUNCTION__, __LINE__);
        }
        
        buffer[0] = 200;
        at24c256_random_read(0x1000, buffer);
        if(ret)
        {
                printf("[%s][%s][%d]:\r\nbyte read error!\r\n\r\n", __FILE__, __FUNCTION__, __LINE__);
        }
        else
        {
                printf("[%s][%s][%d]:\r\ndata = %d\r\n\r\n", __FILE__, __FUNCTION__, __LINE__, buffer[0]);
        }
        
        while(1)
        {
                LED = ON;
                delay_ms(100);
                
                LED = OFF;
                delay_ms(900);
        }
}
