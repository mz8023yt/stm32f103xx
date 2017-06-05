#include "bsp_stepping_motor.h"
#include "bsp_systick.h"

int speed = 10;

void stepping_motor_init()
{
        GPIO_InitTypeDef GPIO_InitStructure;
        
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        
        GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void stepping_motor_run()
{
        PBout(7)  = 1;
        PAout(8)  = 0;
        PAout(11) = 0;
        PAout(12) = 0;
        delay_ms(speed);
        
        PBout(7)  = 0;
        PAout(8)  = 1;
        PAout(11) = 0;
        PAout(12) = 0;
        delay_ms(speed);
        
        PBout(7)  = 0;
        PAout(8)  = 0;
        PAout(11) = 1;
        PAout(12) = 0;
        delay_ms(speed);
        
        PBout(7)  = 0;
        PAout(8)  = 0;
        PAout(11) = 0;
        PAout(12) = 1;
        delay_ms(speed);
}

