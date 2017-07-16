#ifndef __BSP_INA226_H
#define __BSP_INA226_H

#include "stm32f10x.h"

/* INA226 A0 = GND A1 = GND 对应的器件地址为 1000000 */
#define INA226_READ     0x81            // 前7位为固定器件地址, 最后一位为1表示读
#define INA226_WRITE    0x80            // 前7位为固定器件地址, 最后一位为0表示写

int ina226_read_register_data(u8 address, u8* buffer);

#endif /* __BSP_INA226_H */
