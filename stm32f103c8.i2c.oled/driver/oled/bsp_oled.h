#ifndef __BSP_OLED_H
#define __BSP_OLED_H

#include "stm32f10x.h"

int oled_write_data (u8 dat);
int oled_write_command(u8 cmd);
void oled_fill(u8 bmp_dat);
void oled_init(void);

//使用4线串行接口时使用 
#define OLED_RST PBout(7)
#define OLED_DC PBout(6)

//OLED控制用函数
void OLED_WR_Byte(u8 dat,u8 cmd);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);
void JTAG_Set(u8 mode);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowNumber(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y,const u8 *p);

#endif /* __BSP_OLED_H */
