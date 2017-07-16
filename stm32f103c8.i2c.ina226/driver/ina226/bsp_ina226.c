#include "bsp_ina226.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "bsp_i2c.h"

static I2C_BusDef* i2c = &i2c_bus_1;

/**
 * @brief 读取指定寄存器的值
 * @param address 寄存器地址地址
 * @param buffer 返回的寄存器中的值
 * @return int 返回程序执行的状态
 */
int ina226_read_register_data(u8 address, u8* buffer)
{
        u16 temp = 0;
        
        i2c_start(i2c);
        i2c_send_byte(i2c, INA226_WRITE);
        if(i2c_wait_response(i2c))
        {
                log("device address mapping fail");
                return -1;
        }

        i2c_send_byte(i2c, address);
        if(i2c_wait_response(i2c))
        {
                log("write address fail");
                return -1;
        }

        i2c_send_byte(i2c, INA226_READ);
        if(i2c_wait_response(i2c))
        {
                log("device address mapping fail");
                return -1;
        }

        debug("temp = %d", temp);
        temp = i2c_receive_byte(i2c);
        i2c_response(i2c);

        debug("temp = %d", temp);
        temp = i2c_receive_byte(i2c);
        i2c_no_response(i2c);
        temp = (temp << 8) | i2c_receive_byte(i2c);
        
        debug("temp = %d", temp);
        
        i2c_stop(&i2c_bus_1);
        return 0;
}
