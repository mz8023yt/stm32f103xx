#ifndef __BSP_USART_H
#define __BSP_USART_H

#include <stdio.h>
#include <string.h>
#include "stm32f10x.h"

#define log(info) printf("[%s][%s][%d]:\r\n"info"\r\n\r\n", strrchr(__FILE__, '\\') + 1, __func__, __LINE__)
#define debug(fmt, args...) \
        printf("[%s][%s][%d]:\r\n"fmt"\r\n\r\n", strrchr(__FILE__, '\\') + 1, __func__, __LINE__, args)

#define USART_REC_LEN 200                                       // 定义最大接收字节数
extern u8 USART_RX_BUF[USART_REC_LEN];                          // 接收缓冲数组
extern u16 USART_RX_STA;                                        // 接收状态标记

void usart_init(void);                                          // 串口初始化函数

#endif /* __BSP_USART_H */
