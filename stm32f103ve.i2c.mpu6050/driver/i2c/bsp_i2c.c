#include "bsp_i2c.h"
#include "bsp_systick.h"
#include "bsp_usart.h"

/* 定义 I2C 总线在这里定义 */
I2C_BusDef i2c_bus_1;

/* 初始化 I2C 总线 */
void i2c_init(void)
{
        i2c_bus_1.scl_gpio_port = GPIOB;
        i2c_bus_1.scl_gpio_pin = GPIO_Pin_6;
        i2c_bus_1.sda_gpio_port = GPIOB;
        i2c_bus_1.sda_gpio_pin = GPIO_Pin_7;
        i2c_bus_init(&i2c_bus_1);
}

/**
 * @brief 控制 I2C 的速度
 * @param i2c_bus_x 指定操作的 I2C 对象
 */
void i2c_delay(I2C_BusDef* i2c_bus_x)
{
        delay_us(i2c_bus_x->speed);
}

/**
 * @brief I2C初始化函数
 * @param i2c_bus_x 指定操作的I2C对象
 * @note 设置 scl 和 sda 的 GPIO 模式
 */
void i2c_bus_init(I2C_BusDef* i2c_bus_x)
{
        GPIO_InitTypeDef GPIO_InitStructure;
        
        /* 计算 scl_gpio_clk 和 sda_gpio_clk */
        i2c_bus_x->scl_gpio_clk = RCC_APB2Periph_GPIOA << (((u32)(i2c_bus_x->scl_gpio_port) - (u32) GPIOA_BASE) >> 10);
        i2c_bus_x->sda_gpio_clk = RCC_APB2Periph_GPIOA << (((u32)(i2c_bus_x->sda_gpio_port) - (u32) GPIOA_BASE) >> 10);
        
        /* 开启 scl 和 sda 的时钟 */
        RCC_APB2PeriphClockCmd(i2c_bus_x->scl_gpio_clk, ENABLE);
        RCC_APB2PeriphClockCmd(i2c_bus_x->sda_gpio_clk, ENABLE);
        
        /* 配置 scl 和 sda 初始为输出模式 */
        GPIO_InitStructure.GPIO_Pin = i2c_bus_x->scl_gpio_pin;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(i2c_bus_x->scl_gpio_port, &GPIO_InitStructure);
        
        GPIO_InitStructure.GPIO_Pin = i2c_bus_x->sda_gpio_pin;
        GPIO_Init(i2c_bus_x->sda_gpio_port, &GPIO_InitStructure);
        
        /* 接收数据时, sda 为输入, 有特定的函数去改变 sda 引脚的 GPIO 模式 */
        /* STM32 作为 IIC 设备(从机), scl 也是输入, 但是我们本实验没有用到, 因此一直为输出 */

        i2c_bus_x->scl_pin_number = i2c_get_pin_num(i2c_bus_x->scl_gpio_pin);
        i2c_bus_x->sda_pin_number = i2c_get_pin_num(i2c_bus_x->sda_gpio_pin);
        
        i2c_bus_x->scl = (volatile unsigned long*) BITBAND((u32)(i2c_bus_x->scl_gpio_port) + 12, i2c_bus_x->scl_pin_number);
        i2c_bus_x->sda = (volatile unsigned long*) BITBAND((u32)(i2c_bus_x->sda_gpio_port) + 12, i2c_bus_x->sda_pin_number);
        i2c_bus_x->REC = (volatile unsigned long*) BITBAND((u32)(i2c_bus_x->sda_gpio_port) + 8, i2c_bus_x->sda_pin_number);
        
        i2c_bus_x->speed = 2;
        
        /* 数据线和时钟线默认都是高电平 */
        *(i2c_bus_x->scl) = 1;
        *(i2c_bus_x->sda) = 1;
}

/**
 * @brief 设置 sda 对应 GPIO 为输入模式
 * @param i2c_bus_x  指定操作的 I2C 对象
 */
void i2c_set_sda_input(I2C_BusDef* i2c_bus_x)
{
        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Pin = i2c_bus_x->sda_gpio_pin;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_Init(i2c_bus_x->sda_gpio_port,&GPIO_InitStructure);
}

/**
 * @brief 设置 sda 对应 GPIO 为输出模式
 * @param i2c_bus_x 指定操作的 I2C 对象
 */
void i2c_set_sda_output(I2C_BusDef* i2c_bus_x)
{
        GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_InitStructure.GPIO_Pin = i2c_bus_x->sda_gpio_pin;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
        GPIO_Init(i2c_bus_x->sda_gpio_port,&GPIO_InitStructure);
}

/**
 * @brief 产生 I2C 起始信号
 * @param i2c_bus_x 指定操作的 I2C 对象
 * @note 在 scl 为高电平的情况下, sda 产生一个下降沿表示起始
 */
void i2c_start(I2C_BusDef* i2c_bus_x)
{
        i2c_set_sda_output(i2c_bus_x);
        
        /* 这里需要注意顺序, sda 先拉高, scl 再拉高 */
        /* 因为 scl 为高电平的情况下, sda 的边沿动作是有意义的, 因此 sda 要先拉高 */
        *(i2c_bus_x->sda) = 1;
        *(i2c_bus_x->scl) = 1;
        i2c_delay(i2c_bus_x);
        
        /* 在 scl 为高电平的情况下, sda 产生一个下降沿表示起始*/
        *(i2c_bus_x->sda) = 0;
        i2c_delay(i2c_bus_x);
        
        /* 这里其实就是开始驱动传输的时钟了 */
        *(i2c_bus_x->scl) = 0;
}

/**
 * @brief 产生 I2C 停止信号
 * @param i2c_bus_x 指定操作的 I2C 对象
 * @note 在 scl 为高电平的情况下, sda 产生一个上升沿表示停止
 */
void i2c_stop(I2C_BusDef* i2c_bus_x)
{
        i2c_set_sda_output(i2c_bus_x);

        /* 这里需要注意顺序, scl 先拉低, sda 再拉低 */
        /* 因为 scl 为高电平的情况下, sda 的边沿动作是有意义的, 因此 scl 要先拉低 */
        *(i2c_bus_x->scl) = 0;
        *(i2c_bus_x->sda) = 0;
        i2c_delay(i2c_bus_x);

        *(i2c_bus_x->scl) = 1;
        i2c_delay(i2c_bus_x);

        *(i2c_bus_x->sda) = 1;
        i2c_delay(i2c_bus_x);
}

/**
 * @brief 等待应答信号到来
 * @param i2c_bus_x 指定操作的 I2C 对象
 * @return u8 0,接收应答成功; 1,接收应答失败;
 * @note 主机发送完一帧数据之后, 需要等待从机给出响应才会继续发出下一帧数据
 * @note 这时候主机需要放出 sda 的使用权, 由从机负责拉低 sda (响应)
 * @note 主机在下一个 scl 的上升沿(或说高电平)检测 sda 是否为低电平, 低电平则表示有应答
 */
int i2c_wait_response(I2C_BusDef* i2c_bus_x)
{
        /* 定义一个变量, 作为超时的标记 */
        int ucErrTime = 0;
        
        /* 先默认的把 sda 设置为高电平 */
        *(i2c_bus_x->sda) = 1;
        i2c_delay(i2c_bus_x);
        
        /* 这里需要接收从机的应答信号, 因此需要设置为输入 */
        i2c_set_sda_input(i2c_bus_x);
        
        /* 下一个 scl 时钟到了 */
        *(i2c_bus_x->scl) = 1;
        i2c_delay(i2c_bus_x);
        
        /* 是时候去读取 sda 看看有没有响应了 */
        /* 在没有超时之前只要读到了应答就可以自动跳出 while */
        while( *(i2c_bus_x->REC))
        {
                ucErrTime++;

                /* 超时了, 既然从机在规定的时间不应答*/
                if(ucErrTime > 255)
                {
                        /* 主机就认为从机没有正确的接收, 就此作罢 */
                        i2c_stop(i2c_bus_x);

                        /* 函数返回接收应答失败 */
                        return -1;
                }
        }

        /* 到这里, 读取应答信号的 scl 结束了 */
        *(i2c_bus_x->scl) = 0;

        /* 能执行到这里, 说明读取到了应答的 */
        return 0;
}

/**
 * @brief HOST 产生应答
 * @param i2c_bus_x 指定操作的 I2C 对象
 * @note 此函数是在 HOST(STM32) 接收数据时才能使用
 * @note 如果只要接收4个字节, 前3次应答, 最后一次不应答就自动结束了。
 */
void i2c_response(I2C_BusDef* i2c_bus_x)
{
        *(i2c_bus_x->scl) = 0;

        /* 要应答, 肯定要先获取sda的使用权 */
        i2c_set_sda_output(i2c_bus_x);

        /* 在从机发送完本帧最后一个位的低电平期间, 把 sda 拉低 */
        /* 千万不能在 scl 高电平期间拉低, 那就变成起始信号了 */
        *(i2c_bus_x->sda) = 0;
        i2c_delay(i2c_bus_x);

        /* 这里就是从机读取应答信号的一个 scl 时钟周期了, sda 不能动 */
        *(i2c_bus_x->scl) = 1;
        i2c_delay(i2c_bus_x);
        *(i2c_bus_x->scl) = 0;
}

/**
 * @brief HOST 不产生应答
 * @param i2c_bus_x  指定操作的 I2C 对象
 * @note 此函数是在 HOST(STM32) 接收数据时才能使用
 * @note 如果只要接收4个字节, 前3次应答, 最后一次不应答就自动结束了。
 */
void i2c_no_response(I2C_BusDef* i2c_bus_x)
{
        *(i2c_bus_x->scl) = 0;
        i2c_set_sda_output(i2c_bus_x);

        /* 既然拉低表示应答, 那我拉高不就行了 */
        *(i2c_bus_x->sda) = 1;
        i2c_delay(i2c_bus_x);

        /* 下一个时钟一检测, 就发现, 没有应答 */
        *(i2c_bus_x->scl) = 1;
        i2c_delay(i2c_bus_x);
        *(i2c_bus_x->scl) = 0;
}

/**
 * @brief I2C 发送一个字节
 * @param i2c_bus_x 指定操作的 I2C 对象
 * @param data 发送的数据
 */
void i2c_send_byte(I2C_BusDef* i2c_bus_x, u8 data)
{
        u8 t;
        i2c_set_sda_output(i2c_bus_x);

        /* 所有的数据的输出到 sda 线上都是在 scl 的低电平期间 */
        *(i2c_bus_x->scl) = 0;

        /* 依次发送8个数据值 */
        for(t = 0; t < 8; t++)
        {
                /* 写入数据的最高位 */
                *(i2c_bus_x->sda) = (data& 0x80) >> 7;

                /* 发送完了最高位, 数据左移一个, 次高位变成了新的最高位 */
                data <<= 1;
                i2c_delay(i2c_bus_x);

                /* 在 scl 的上升沿(或者高电平期间), 数据被从机接收读取 */
                *(i2c_bus_x->scl) = 1;
                i2c_delay(i2c_bus_x);
                *(i2c_bus_x->scl) = 0;
                i2c_delay(i2c_bus_x);
        }
        /* 这函数结束的时候是不是 scl = 0 */
        /* 一般这里接下来就会是 WaitResponse 操作了 */
        /* WaitResponse 的时候就是直接从 scl = 0 开始的, 这样就不会多出来一个 scl 的脉冲 */
}

/**
 * @brief I2C 读一个字节
 * @param i2c_bus_x 指定操作的 I2C 对象
 * @note 根据参数决定要不要应答, 如果只要接收4个字节, 前3次应答, 最后一次不应答就自动结束了。
 */
u8 i2c_receive_byte(I2C_BusDef* i2c_bus_x)
{
        u8 i, data = 0;
        i2c_set_sda_input(i2c_bus_x);

        for(i = 0; i < 8; i++)
        {
                *(i2c_bus_x->scl) = 0;
                i2c_delay(i2c_bus_x);
                *(i2c_bus_x->scl) = 1;
                data <<= 1;
                if(*(i2c_bus_x->REC))
                        data++;
                i2c_delay(i2c_bus_x);
        }

        return data;
}

/**
 * @brief 由GPIO_Pin_x 获取到引脚编号
 * @param GPIO_Pin_x 需要解析的 GPIO_Pin
 * @return u8 对应的引脚编号
 */
u8 i2c_get_pin_num(u16 GPIO_Pin_x)
{
        int num = 0;
        
        while(!(GPIO_Pin_x & 0x1))
        {
                GPIO_Pin_x = GPIO_Pin_x >> 1;
                num++;
        }
        
        return num;
}
