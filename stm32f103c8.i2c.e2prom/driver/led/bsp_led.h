#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "stm32f10x.h"

#define LED     PCout(13)
#define ON      1
#define OFF     0

void led_init(void);

#endif /* __BSP_LED_H */
