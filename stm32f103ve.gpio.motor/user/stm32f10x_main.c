#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "bsp_led.h"
#include "bsp_stepping_motor.h"

int main(void)
{
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        
        led_init();
        systick_init();
        usart_init();
        
        stepping_motor_init();
        
        while(1)
        {
                stepping_motor_run();
        }
}
