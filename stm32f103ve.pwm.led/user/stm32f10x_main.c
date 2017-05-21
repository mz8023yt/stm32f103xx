#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "bsp_led.h"

int main(void)
{
        u8 t;                                           // for循环计数变量
        u8 len;                                         // 保存接收数据的长度
        u16 times = 0;                                  // 一个用于计时的变量
        
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        
        led_init();
        systick_init();
        usart_init();
        
        while(1)
        {
                /* 如果接收完了所有的字符(每一次串口中断接收一个字符) */
                if (USART_RX_STA & 0x8000)
                {
                        /* 得到此次接收到的数据长度 */
                        len = USART_RX_STA & 0x3fff;
                        printf("Your input is: ");
                        
                        /* 将接收的数据原封不动的打印回上位机 */
                        for (t = 0; t < len; t++)
                        {
                                /* 向串口1发送数据, 依次发送接收到的每一个字符 */
                                USART_SendData(USART1, USART_RX_BUF[t]);
                                
                                /*等待发送结束*/
                                while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);
                        }
                        printf("\r\n");
                        
                        /*全部发送完成之后，清空自定义的标志寄存器*/
                        USART_RX_STA = 0;
                }
                else
                {
                        /*在数据没有接收到或者没有接收完的情况下*/
                        times++;
                        if (times % 100 == 0)
                                printf("I can output your input \r\n");
                        delay_ms(10);
                }
                
        }
}
