#include "bsp_ina226.h"
#include "bsp_systick.h"
#include "bsp_usart.h"
#include "bsp_i2c.h"

static I2C_BusDef* i2c = &i2c_bus_1;
static u8 buffer[8];

/**
 * @brief 初始化 ina226
 * @return int 返回程序执行的状态
 */
int ina226_init(void)
{
        int ret = -1;
        
        buffer[0] = 0x40;
        buffer[1] = 0x27;
        ret = ina226_write_register(0x00, buffer);
        if(ret)
        {
                log("write 0x00 register fail");
        }

        buffer[0] = 0xA0;
        buffer[1] = 0x00;
        ina226_write_register(0x05, buffer);
        {
                log("write 0x05 register fail");
        }

        return 0;
}

/**
 * @brief ina226 读取指定寄存器的值
 * @param address 寄存器地址地址
 * @param buffer 返回的寄存器中的值
 * @return int 返回程序执行的状态
 */
int ina226_read_register_data(u8 address, u8* buffer)
{
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

        i2c_stop(&i2c_bus_1);

        i2c_start(i2c);

        i2c_send_byte(i2c, INA226_READ);
        if(i2c_wait_response(i2c))
        {
                log("device address mapping fail");
                return -1;
        }

        buffer[0] = i2c_receive_byte(i2c);
        i2c_response(i2c);

        buffer[1] = i2c_receive_byte(i2c);
        i2c_no_response(i2c);

        i2c_stop(&i2c_bus_1);
        return 0;
}

/**
 * @brief ina226 向指定的寄存器写数据
 * @return int 返回程序执行的状态
 */
int ina226_write_register(u8 address, u8* buffer)
{
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

        i2c_send_byte(i2c, buffer[0]);
        if(i2c_wait_response(i2c))
        {
                log("device address mapping fail");
                return -1;
        }
        
        i2c_send_byte(i2c, buffer[1]);
        if(i2c_wait_response(i2c))
        {
                log("device address mapping fail");
                return -1;
        }
        
        i2c_stop(i2c);
        return 0;
}
