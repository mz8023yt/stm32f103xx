#include "stm32f10x.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "bsp_led.h"
#include "bsp_i2c.h"
#include "bsp_mpu6050.h"

int main(void)
{
        int ret = -1;
        short Accel[3];
        short Gyro[3];
        float Temp;
        
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        systick_init();
        usart_init();
        led_init();
        i2c_init();
        mpu6050_init();
        
        ret = mpu6050_read_id();
        if(ret)
        {
                printf("[%s][%d] read mpu device id fali!\r\n", __FUNCTION__, __LINE__);
                return ret;
        }
        
        while(1)
        {
                mpu6050_read_acceleration(Accel);
                printf("acceleration : %8d%8d%8d \r\n", Accel[0], Accel[1], Accel[2]);
                
                mpu6050_read_angular_acceleration(Gyro);
                printf("angula       : %8d%8d%8d \r\n", Gyro[0], Gyro[1], Gyro[2]);
                
                mpu6050_transform_temperature(&Temp); 
                printf("temperature  : %8.2f\r\n", Temp);
                printf("\r\n");
                
                delay_ms(1000);
        }
}
