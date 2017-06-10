#include "data_scope.h"
#include <stdarg.h>
#include <string.h>

/* 串口发送的数据帧的帧缓冲区 */
u8 data_scope_buffer[42] = {0};

/**
 * @brief 将单精度浮点数据转成 4 字节数据并存入指定地址
 * @param target 传入的单精度浮点数, 上位机显示的就是它
 * @param buffer 将但精度浮点数转化后保存在此 buffer 中
 * @param begin  指定保存在 buffer 中的位置
 * @return null
 */
void data_scope_float_to_byte(float *target, u8 *buffer, u8 begin)
{
        /* 得到 float 数据的首地址 */
        u8 *point = (u8*) target;
        buffer[begin] = point[0];
        buffer[begin + 1] = point[1];
        buffer[begin + 2] = point[2];
        buffer[begin + 3] = point[3];
}

/**
 * @brief 将待发送通道的单精度浮点数据写入发送缓冲区
 * @param channel 指定此但精度浮点数对应的显示通道 (1 - 10)
 * @param data 传入的单精度浮点数, 上位机显示的就是它
 * @return null
 * @note data_scope 上位机最多支持 10 个通道
 */
void data_scope_write_buffer(u8 channel, float data)
{
        if ((channel >= 1) && (channel <= 10))
                data_scope_float_to_byte(&data, data_scope_buffer, 4 * channel - 3);
}

/**
 * @brief 生成 data_scope 上位机能正确识别的帧格式
 * @param channel 指定本次格式化帧中使用到了多少个通道 (1 - 10)
 * @return succed: 发送缓冲区数据个数 fail: -1
 */
int data_scope_ready_to_send(u8 channel)
{
        if ((channel >= 1) && (channel <= 10))
        {
                /* 一帧数据帧以 $ 符号开始 */
                data_scope_buffer[0] = '$';
                data_scope_buffer[4 * channel + 1] = 4 * channel + 1;
                return (4 * channel + 2);
        }
        
        return -1;
}

/**
 * @brief 发送波形数据到上位机
 * @param num 同时发送的通道数
 * @return ... 每一个通道的波形数据公式
 */
void data_scope_wave(int num, ... )
{
        int i;
        int length;
        va_list list;                                   /* 定义保存函数参数的结构 */    
        float param;                                    /* 存放取出的字符串参数 */    
        va_start(list, num);                            /* list 指向传入的第一个可选参数, num 是最后一个确定的参数 */
        
        for(i = 1; i <= num ; i++)
        {
                param = va_arg(list, double);           /* 取出当前的参数 */
                data_scope_write_buffer(i, param);
        }
        va_end(list);                                   /* 将 list 置为 NULL */
        length = data_scope_ready_to_send(num);
        
        for(i = 0; i < length; i++)                     /* 通过查询的方式发送数据帧 */
        {
                while ((USART1->SR & 0x40) == 0);
                USART1->DR = data_scope_buffer[i];
        }
}
