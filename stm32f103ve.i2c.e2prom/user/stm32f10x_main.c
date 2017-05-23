#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "bsp_led.h"
#include "bsp_at24c02.h"
#include "bsp_i2c.h"
#include <string.h>
#include <stdlib.h>

int main(void)
{
        u8 buffer[8];
        int length = 0;
        int address = 0;
        int data = 0;
        char* cmd = NULL;
        
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        systick_init();
        usart_init();
        at24c02_init();
        led_init();
        
        while(1)
        {
                /* 如果接收完了所有的字符(每一次串口中断接收一个字符) */
                if (USART_RX_STA & 0x8000)
                {
                        /* 得到此次接收到的数据长度 */
                        length = USART_RX_STA & 0x3fff;
                        USART_RX_BUF[length] = '\0';
                        
                        printf("[stm32f10x][%s][%d]: %s \r\n", __FUNCTION__, __LINE__, (char *)USART_RX_BUF);
                        
                        char* p;
                        p = strtok((char*)USART_RX_BUF, " ");
                        if(p)
                        {
                                cmd = p;
                                // printf("[stm32f10x][%s][%d]: %s \r\n", __FUNCTION__, __LINE__, cmd);
                        }
                        
                        p = strtok(NULL, " ");
                        if(p)
                        {
                                address = atoi(p);
                                // printf("[stm32f10x][%s][%d]: %d \r\n", __FUNCTION__, __LINE__, address);
                        }
                        
                        p = strtok(NULL, " ");
                        if(p)
                        {
                                data = atoi(p);
                                // printf("[stm32f10x][%s][%d]: %d \r\n", __FUNCTION__, __LINE__, data);
                        }
                        
                        if(!strcmp("write", cmd))
                        {
                                // printf("[stm32f10x][%s][%d]: write the e2prom \r\n", __FUNCTION__, __LINE__);
                                buffer[0] = data;
                                at24c02_byte_write(address, buffer);
                        }
                        else if(!strcmp("read", cmd))
                        {
                                // printf("[stm32f10x][%s][%d]: read the e2prom \r\n", __FUNCTION__, __LINE__);
                                at24c02_random_read(address, buffer);
                                printf("[stm32f10x][%s][%d]: %d address's data = %d\r\n", __FUNCTION__, __LINE__, address, buffer[0]);
                        }
                        
                        /* 搞完事情后, 清空自定义的标志寄存器 */
                        USART_RX_STA = 0;
                }
        }
}
