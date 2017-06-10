#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "bsp_led.h"
#include "data_scope.h"
#include <math.h>

u8 i;                        // 计数变量
u8 channel_count;            // 串口需要发送的数据个数
static float a, b = 600;

int main(void)
{
        systick_init();
        usart_init();

        while (1)
        {
                a += 0.1;
                if (a > 3.14)
                {
                        a = -3.14;
                        b = -b;
                }

                // 从这里开始到串口发送的代码, 最好能用一个可变参数的函数封装就好了
                data_scope_write_buffer(1, 500 * sin(a) +2000);
                data_scope_write_buffer(2, 200 * a);
                data_scope_write_buffer(3, b - 2000);
                channel_count = data_scope_ready_to_send(3);

                // 通过查询的方式发送数据帧
                for (i = 0; i < channel_count; i++)
                {
                        while ((USART1->SR & 0X40) == 0);
                        USART1->DR = data_scope_buffer[i];
                }
                delay_ms(30); 
        }
}
