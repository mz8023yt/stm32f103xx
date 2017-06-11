#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "bsp_led.h"
#include "data_scope.h"
#include <math.h>

static float a;

int main(void)
{
        systick_init();
        usart_init();

        while (1)
        {
                a += 0.1;
                if (a > 3.14)
                        a = -3.14;

                /* 绘制正弦波 */
                data_scope_wave(1, sin(a));

                delay_ms(50);
        }
}
