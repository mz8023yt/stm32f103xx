#ifndef __BSP_OLED_H
#define __BSP_OLED_H

#include "stm32f10x.h"

#define OLED_SCLK       PBout(9)
#define OLED_SDIN       PBout(8)
#define OLED_RST        PBout(7)
#define OLED_RS         PBout(6)

void oled_write_dat(u8 dat);
void oled_write_cmd(u8 cmd);
void oled_display_on(void);
void oled_display_off(void);
void oled_refresh_gram(void);
void oled_init(void);
void oled_clear_screen(void);
void oled_draw_point(u8 x, u8 y, u8 t);
void oled_draw_rectangle(u8 x1, u8 y1, u8 x2, u8 y2, u8 dot);
void oled_draw_char(u8 x, u8 y, u8 chr, u8 size, u8 mode);
void oled_draw_number(u8 x, u8 y, u32 num, u8 len, u8 size);
void oled_draw_string(u8 x, u8 y, const u8 *p);

#endif /* __BSP_OLED_H */
