#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "bsp_led.h"
#include "bsp_at24c02.h"
#include "bsp_i2c.h"
#include <string.h>
#include <stdlib.h>

void print_message(void);

int main(void)
{
        u8 buffer[8];
        int length = 0;
        int address = 0;
        int data = 0;
        char* cmd = NULL;
        int i;
        
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        systick_init();
        usart_init();
        i2c_init();
        led_init();
        
        print_message();
        
        while(1)
        {
                /* 如果接收完了所有的字符(每一次串口中断接收一个字符) */
                if (USART_RX_STA & 0x8000)
                {
                        /* 得到此次接收到的数据长度 */
                        length = USART_RX_STA & 0x3fff;
                        USART_RX_BUF[length] = '\0';
                        
                        char* p;
                        
                        /* 解析出命令值 */
                        p = strtok((char*)USART_RX_BUF, " ");
                        if(p)
                        {
                                cmd = p;
                        }
                        
                        /* 解析出第一个参数 */
                        p = strtok(NULL, " ");
                        if(p)
                        {
                                address = atoi(p);
                        }
                        
                        /* 解析出第二个参数 */
                        p = strtok(NULL, " ");
                        if(p)
                        {
                                data = atoi(p);
                        }
                        
                        /* 开始匹配命令 */
                        if(!strcmp("write", cmd) || !strcmp("w", cmd))
                        {
                                buffer[0] = data;
                                at24c02_byte_write(address, buffer);
                                printf("write is successful\r\n");
                                print_message();
                        }
                        /* 读取指定地址的值 */
                        else if(!strcmp("read", cmd) || !strcmp("r", cmd))
                        {
                                at24c02_random_read(address, buffer);
                                printf("[stm32f10x][%s][%d]: the %d address's data = %d\r\n", __FUNCTION__, __LINE__, address, buffer[0]);
                                print_message();
                        }
                        /* 格式化 e2prom 中的数据 */
                        else if(!strcmp("format", cmd) || !strcmp("f", cmd))
                        {
                                /* 在buffer中准备好数据 */
                                for(i = 0; i < 256; i++)
                                {
                                        buffer[i] = i;
                                }
                                
                                for(i = 0; i < 32; i++)
                                {
                                        address = 8 * i;
                                        at24c02_page_write(address, &buffer[address], 8);
                                        delay_ms(5);
                                }
                                printf("format is successful\r\n");
                                print_message();
                        }
                        /* 将 e2prom 中的数据全部清零 */
                        else if(!strcmp("clear", cmd) || !strcmp("c", cmd))
                        {
                                /* 在buffer中准备好数据 */
                                for(i = 0; i < 256; i++)
                                {
                                        buffer[i] = 0;
                                }
                                
                                for(i = 0; i < 32; i++)
                                {
                                        address = 8 * i;
                                        at24c02_page_write(address, &buffer[address], 8);
                                        delay_ms(5);
                                }
                                printf("clear is successful\r\n");
                                print_message();
                        }
                        /* 显示 e2prom 所有地址的数据 */
                        else if(!strcmp("display", cmd) || !strcmp("d", cmd))
                        {
                                at24c02_set_current_address(0);
                                at24c02_sequentia_read(buffer, 256);
                                
                                /* 打印提示信息 */
                                printf("\r\n\t");
                                for(i = 0; i < 16; i++)
                                {
                                        printf("col %d\t", i);
                                }
                                printf("\r\n");
                                
                                printf("1\t");
                                for(i = 0; i < 256; i++)
                                {
                                        printf(" %d\t", buffer[i]);
                                        if(i % 16 == 15 && i < 255)
                                        {
                                                printf("\r\n");
                                                printf("%d\t", i + 1);
                                        }
                                }
                                printf("\r\n");
                                print_message();
                        }
                        
                        /* 搞完事情后, 清空自定义的标志寄存器 */
                        USART_RX_STA = 0;
                }
        }
}

void print_message(void)
{
        printf("\r\nplease enter the cmd: the cmd end of new line symbol(\\r\\n)\r\n");
        printf("    display(d): display the e2prom all address data.\r\n");
        printf("    clear(c)  : write all address data to 0.\r\n");
        printf("    format(f) : write all address data to 0-255.\r\n");
        printf("    write(w)  : the specified address written to the specified value.\r\n");
        printf("    read(r)   : read the value of the specified address.\r\n");
        printf("\r\n");
}
