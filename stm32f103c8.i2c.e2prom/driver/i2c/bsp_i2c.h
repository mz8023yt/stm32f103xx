#ifndef __BSP_I2C_H
#define __BSP_I2C_H

#include "stm32f10x.h"

/**
 * @brief 使用此结构创建一个对象便创建了一个 I2C 的总线控制器(即 I2C 主机)
 * @usage 1. 先创建一个 I2C_BusDef 对象:
 *           I2C_BusDef i2c_bus_1;
 *        2. 填充结构中的 scl sda 具体软件模拟的引脚信息
 *           i2c_bus_1.scl_gpio_port = GPIOB;
 *           i2c_bus_1.scl_gpio_pin = GPIO_Pin_6;
 *           i2c_bus_1.sda_gpio_port = GPIOB;
 *           i2c_bus_1.sda_gpio_pin = GPIO_Pin_7;
 *        3. 调用 i2c_bus_init() 函数初始化 I2C 总线控制器
 *           i2c_bus_init(&i2c_bus_1);
 */
typedef struct
{
        volatile unsigned long* scl;            // scl 时钟线引脚经位带映射后的地址
        volatile unsigned long* sda;            // sda 数据线引脚经位带映射后的地址
        volatile unsigned long* REC;            // sda 作为输入时位带映射的地址
        u32                     scl_gpio_clk;   // scl 的时钟, 示例
        u32                     sda_gpio_clk;   // sda 的时钟, 示例
        GPIO_TypeDef*           scl_gpio_port;  // scl 的 GPIO 端口, 示例: GPIOB
        GPIO_TypeDef*           sda_gpio_port;  // sda 的 GPIO 端口, 示例: GPIOB
        u16                     scl_gpio_pin;   // scl 的 GPIO 引脚编号(标准格式), 示例: GPIO_Pin_6
        u16                     sda_gpio_pin;   // sda 的 GPIO 引脚编号(标准格式), 示例: GPIO_Pin_7
        u8                      scl_pin_number; // scl 的 GPIO 引脚编号(数字格式), 示例: 6
        u8                      sda_pin_number; // sda 的 GPIO 引脚编号(数字格式), 示例: 7
        u8                      speed;          // I2C 通信时, 每次延时的时间值, 用来控制 I2C 通信的速度
} I2C_BusDef;

/* 外部声明 I2C 总线结构, 其他文件包含此头文件就可以直接拿到 I2C 总线结构对象 */
extern I2C_BusDef i2c_bus_1;

void i2c_init(void);
void i2c_bus_init(I2C_BusDef* I2Cx);
void i2c_set_sda_input(I2C_BusDef* I2Cx);
void i2c_set_sda_output(I2C_BusDef* I2Cx);
void i2c_start(I2C_BusDef* I2Cx);
void i2c_stop(I2C_BusDef* I2Cx);
void i2c_response(I2C_BusDef* I2Cx);
void i2c_no_response(I2C_BusDef* I2Cx);
void i2c_send_byte(I2C_BusDef* I2Cx, u8 data);
int i2c_wait_response(I2C_BusDef* I2Cx);
u8 i2c_receive_byte(I2C_BusDef* I2Cx);
u8 i2c_get_pin_num(u16 GPIO_Pin_x);

#endif /* __BSP_I2C_H */
