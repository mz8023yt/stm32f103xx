#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "bsp_led.h"
#include "bsp_at24c02.h"
#include "bsp_i2c.h"

void test_page_write(void);
void test_byte_write(void);
void test_random_read(void);
void test_sequentia_read(void);
void test_current_address_read(void);

int i;
int result = -1;
u8 address;
u8 buffer[256];

int main(void)
{
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        systick_init();
        usart_init();
        at24c02_init();
        
        test_page_write();
        test_byte_write();
        
        test_sequentia_read();
        printf("\r\n");
        
        test_random_read();
        printf("\r\n");
        
        test_current_address_read();
        
        while(1);
}

void test_byte_write(void)
{
        buffer[0] = 0xff;
        result = at24c02_byte_write(77, buffer);
        if(result)
        {
                printf("[at24c02][%s]: fail line = %d \r\n", __FUNCTION__, __LINE__);
        }
        delay_ms(5);
}

void test_random_read(void)
{
        buffer[0] = 0;
        result = at24c02_random_read(77, buffer);
        if(result)
        {
                printf("[stmf10x][%s]: fail line = %d \r\n", __FUNCTION__, __LINE__);
        }
        printf("[stmf10x][%s]: the data in address 77 is %d\r\n", __FUNCTION__, buffer[0]);
        delay_ms(5);
}

void test_page_write(void)
{
        /* 在buffer中准备好数据 */
        for(i = 0; i < 256; i++)
        {
                buffer[i] = i;
        }
        
        for(i = 0; i < 32; i++)
        {
                address = 8 * i;
                result = at24c02_page_write(address, &buffer[address], 8);
                delay_ms(5);
                if(result)
                {
                        printf("[stmf10x][%s]: fail line = %d \r\n", __FUNCTION__, __LINE__);
                }
        }
}

void test_sequentia_read(void)
{
        at24c02_set_current_address(0);
        at24c02_sequentia_read(buffer, 256);
        for(i = 0; i < 256; i++)
        {
                printf("%d\t", buffer[i]);
                if(i % 16 == 15)
                        printf("\r\n");
        }
}

void test_current_address_read(void)
{
        at24c02_current_address_read(buffer);
        printf("[stmf10x][%s]: the current address's data is %d\r\n", __FUNCTION__, buffer[0]);
}
