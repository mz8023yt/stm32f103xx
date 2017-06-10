#include "mini_balance.h"

/* 串口发送的数据帧的帧缓冲区 */
u8 mini_balance_buffer[42] = {0};

/**
 * @brief 将单精度浮点数据转成 4 字节数据并存入指定地址
 * @param target 传入的单精度浮点数, 上位机显示的就是它
 * @param buffer 将但精度浮点数转化后保存在此 buffer 中
 * @param begin  指定保存在 buffer 中的位置
 * @return null
 */
void mini_balance_float_to_byte(float *target, u8 *buffer, u8 begin)
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
 * @note mini balance 上位机最多支持 10 个通道
 */
void mini_balance_write_buffer(u8 channel, float data)
{
        if ((channel >= 1) && (channel <= 10))
                mini_balance_float_to_byte(&data, mini_balance_buffer, 4 * channel - 3);
}

/**
 * @brief 生成 mini balance 上位机能正确识别的帧格式
 * @param channel 指定本次格式化帧中使用到了多少个通道 (1 - 10)
 * @return succed: 发送缓冲区数据个数 fail: -1
 */
int mini_balance_ready_to_send(u8 channel)
{
        if ((channel >= 1) && (channel <= 10))
        {
                /* 一帧数据帧以 $ 符号开始 */
                mini_balance_buffer[0] = '$';
                mini_balance_buffer[4 * channel + 1] = 4 * channel + 1;
                return (4 * channel + 2);
        }
        
        return -1;
}
