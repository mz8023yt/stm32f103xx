#ifndef __DATA_PRTOCOL_H
#define __DATA_PRTOCOL_H

#include "stm32f10x.h"

extern u8 mini_balance_buffer[42];                              // 待发送帧数据缓存区

void mini_balance_write_buffer(u8 channel, float data);         // 写通道数据至待发送帧数据缓存区
int mini_balance_ready_to_send(u8 channel);                     // 发送帧数据生成函数

#endif 
