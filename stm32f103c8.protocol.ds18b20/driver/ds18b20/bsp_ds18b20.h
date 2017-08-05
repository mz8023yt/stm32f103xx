#ifndef __BSP_DS18B20_H
#define __BSP_DS18B20_H

#include "stm32f10x.h"
#include "bsp_systick.h"

#define DQ PCout(13)

void delay(u16 i);
void ds18b20_init(void);
u8 ds18b20_start(void);
u16 read_temperature(void);

#endif /* __BSP_DS18B20_H */
