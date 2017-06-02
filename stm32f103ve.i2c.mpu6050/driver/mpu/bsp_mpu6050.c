#include "bsp_systick.h"
#include "bsp_usart.h"
#include "bsp_i2c.h"
#include "bsp_mpu6050.h"

/**
 * @brief 写数据到MPU6050寄存器
 * @param
 * @retval
 */
void mpu6050_write_register(u8 reg_add,u8 reg_dat)
{
        i2c_start(&i2c_bus_1);
        i2c_send_byte(&i2c_bus_1, MPU6050_SLAVE_ADDRESS);
        i2c_wait_response(&i2c_bus_1);
        i2c_send_byte(&i2c_bus_1, reg_add);
        i2c_wait_response(&i2c_bus_1);
        i2c_send_byte(&i2c_bus_1, reg_dat);
        i2c_wait_response(&i2c_bus_1);
        i2c_stop(&i2c_bus_1);
}


/**
  * @brief   从MPU6050寄存器读取数据
  * @param   
  * @retval  
  */
void mpu6050_read_register(u8 reg_add, unsigned char* buffer, u8 num)
{
        unsigned char i;
        
        i2c_start(&i2c_bus_1);
        i2c_send_byte(&i2c_bus_1, MPU6050_SLAVE_ADDRESS);
        i2c_wait_response(&i2c_bus_1);
        i2c_send_byte(&i2c_bus_1, reg_add);
        i2c_wait_response(&i2c_bus_1);
        
        i2c_start(&i2c_bus_1);
        i2c_send_byte(&i2c_bus_1, MPU6050_SLAVE_ADDRESS+1);
        i2c_wait_response(&i2c_bus_1);
        
        for(i=0;i<(num-1);i++){
                buffer[i] = i2c_receive_byte(&i2c_bus_1);
                i++;
        }
        buffer[i] = i2c_receive_byte(&i2c_bus_1);
        i2c_stop(&i2c_bus_1);
}


/**
  * @brief 初始化 MPU6050 芯片
  * @param 
  * @retval  
  */
void mpu6050_init(void)
{
        int i=0,j=0;
        //在初始化之前要延时一段时间，若没有延时，则断电后再上电数据可能会出错
        for(i=0;i<1000;i++)
        {
                for(j=0;j<1000;j++);
        }
        mpu6050_write_register(MPU6050_RA_PWR_MGMT_1, 0x00);          // 解除休眠状态
        mpu6050_write_register(MPU6050_RA_SMPLRT_DIV , 0x07);         // 陀螺仪采样率，1KHz
        mpu6050_write_register(MPU6050_RA_CONFIG , 0x06);             // 低通滤波器的设置，截止频率是1K，带宽是5K
        mpu6050_write_register(MPU6050_RA_ACCEL_CONFIG , 0x00);       // 配置加速度传感器工作在2G模式，不自检
        mpu6050_write_register(MPU6050_RA_GYRO_CONFIG, 0x18);         // 陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
}

/**
  * @brief 读取MPU6050的ID
  * @param   
  * @retval  
  */
u8 mpu6050_read_id(void)
{
        unsigned char Re = 0;
        mpu6050_read_register(MPU6050_RA_WHO_AM_I,&Re,1);    //读器件地址
        if(Re != 0x68)
        {
                printf("MPU6050 dectected error!\r\n");
                return 0;
        }
        else
        {
                printf("MPU6050 ID = %d\r\n",Re);
                return 1;
        }
                
}
/**
  * @brief 读取MPU6050的加速度数据
  * @param   
  * @retval  
  */
void mpu6050_read_acceleration(short *accData)
{
    u8 buf[6];
    mpu6050_read_register(MPU6050_ACC_OUT, buf, 6);
    accData[0] = (buf[0] << 8) | buf[1];
    accData[1] = (buf[2] << 8) | buf[3];
    accData[2] = (buf[4] << 8) | buf[5];
}

/**
  * @brief 读取MPU6050的角加速度数据
  * @param   
  * @retval  
  */
void mpu6050_read_angular_acceleration(short *gyroData)
{
    u8 buf[6];
    mpu6050_read_register(MPU6050_GYRO_OUT,buf,6);
    gyroData[0] = (buf[0] << 8) | buf[1];
    gyroData[1] = (buf[2] << 8) | buf[3];
    gyroData[2] = (buf[4] << 8) | buf[5];
}


/**
  * @brief 读取MPU6050的原始温度数据
  * @param   
  * @retval  
  */
void mpu6050_read_temperature(short *tempData)
{
        u8 buf[2];
        mpu6050_read_register(MPU6050_RA_TEMP_OUT_H,buf,2);     //读取温度值
        *tempData = (buf[0] << 8) | buf[1];
}


/**
  * @brief   读取MPU6050的温度数据，转化成摄氏度
  * @param   
  * @retval  
  */
void mpu6050_transform_temperature(float *Temperature)
{
        short temp3;
        u8 buf[2];
        
        mpu6050_read_register(MPU6050_RA_TEMP_OUT_H,buf,2);     //读取温度值
        temp3= (buf[0] << 8) | buf[1];        
        *Temperature=((double) temp3/340.0)+36.53;

}
