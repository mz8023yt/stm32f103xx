#ifndef __BSP_at24c256_H
#define __BSP_at24c256_H

#include "stm32f10x.h"

/* at24c256对应的器件地址为 10100XX */
#define AT24C256_READ      0xA1            // 前7位为固定器件地址, 最后一位为1表示读
#define AT24C256_WRITE     0xA0            // 前7位为固定器件地址, 最后一位为0表示写

int at24c256_byte_write(u16 address, u8* buffer);
int at24c256_page_write(u16 address, u8* buffer, u8 length);
int at24c256_current_address_read(u8* buffer);
int at24c02_random_read(u16 address, u8* buffer);

#endif /* __BSP_at24c256_H */
