#ifndef __BSP_OLED_H
#define __BSP_OLED_H

#include "stm32f10x.h"

#define OLED_SCLK_Clr() PBout(9)=0
#define OLED_SCLK_Set() PBout(9)=1

#define OLED_SDIN_Clr() PBout(8)=0
#define OLED_SDIN_Set() PBout(8)=1

#define OLED_RST_Clr()  PBout(7)=0
#define OLED_RST_Set()  PBout(7)=1

#define OLED_RS_Clr()   PBout(6)=0
#define OLED_RS_Set()   PBout(6)=1

#define OLED_CMD  0
#define OLED_DATA 1

void oled_write_dat(u8 dat);
void OLED_WR_Byte(u8 cmd);
void oled_display_on(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);
void oled_init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void oled_draw_rectangle(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowNumber(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y,const u8 *p);

#endif /* __BSP_OLED_H */
