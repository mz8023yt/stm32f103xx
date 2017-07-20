#include "bsp_oled.h"
#include "bsp_i2c.h"
#include "bsp_usart.h"
#include "bsp_systick.h"
#include "stdlib.h"
#include "bsp_oled_font.h"

/**
 * @brief
 */

//[0]0 1 2 3 ... 127
//[1]0 1 2 3 ... 127
//[2]0 1 2 3 ... 127
//[3]0 1 2 3 ... 127
//[4]0 1 2 3 ... 127
//[5]0 1 2 3 ... 127
//[6]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127
u8 OLED_GRAM[128][8];

void OLED_Refresh_Gram(void)
{
        u8 i,n;
        for(i=0;i<8;i++)
        {
                OLED_WR_Byte (0xb0 + i);
                OLED_WR_Byte (0x00);
                OLED_WR_Byte (0x10);
                for(n=0;n<128;n++)
                        oled_write_dat(OLED_GRAM[n][i]);
        }
}

void OLED_WR_Byte(u8 cmd)
{
        u8 i;
        OLED_RS_Clr();
        for(i=0;i<8;i++)
        {
                OLED_SCLK_Clr();
                if(cmd&0x80)
                   OLED_SDIN_Set();
                else
                   OLED_SDIN_Clr();
                OLED_SCLK_Set();
                cmd<<=1;
        }
        OLED_RS_Set();
}

void oled_write_dat(u8 dat)
{
        u8 i;
        OLED_RS_Set();
        for(i=0;i<8;i++)
        {
                OLED_SCLK_Clr();
                if(dat&0x80)
                   OLED_SDIN_Set();
                else
                   OLED_SDIN_Clr();
                OLED_SCLK_Set();
                dat<<=1;
        }
        OLED_RS_Set();
}

void oled_display_on(void)
{
        OLED_WR_Byte(0X8D);
        OLED_WR_Byte(0X14);
        OLED_WR_Byte(0XAF);
}

void OLED_Display_Off(void)
{
        OLED_WR_Byte(0X8D);
        OLED_WR_Byte(0X10);
        OLED_WR_Byte(0XAE);
}

void OLED_Clear(void)
{
        u8 i,n;
        for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0X00;
        OLED_Refresh_Gram();
}

void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
        u8 pos,bx,temp=0;
        if(x>127||y>63)return;
        pos=7-y/8;
        bx=y%8;
        temp=1<<(7-bx);
        if(t)OLED_GRAM[x][pos]|=temp;
        else OLED_GRAM[x][pos]&=~temp;
}

void oled_draw_rectangle(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)
{
        u8 x,y;
        for(x=x1;x<=x2;x++)
        {
                for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
        }
        OLED_Refresh_Gram();
}

void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{
        u8 temp,t,t1;
        u8 y0=y;
        chr=chr-' ';
    for(t=0;t<size;t++)
    {
                if(size==12)temp=oled_asc2_1206[chr][t];
                else temp=oled_asc2_1608[chr][t];
        for(t1=0;t1<8;t1++)
                {
                        if(temp&0x80)OLED_DrawPoint(x,y,mode);
                        else OLED_DrawPoint(x,y,!mode);
                        temp<<=1;
                        y++;
                        if((y-y0)==size)
                        {
                                y=y0;
                                x++;
                                break;
                        }
                }
    }
}

u32 oled_pow(u8 m,u8 n)
{
        u32 result=1;
        while(n--)result*=m;
        return result;
}

void OLED_ShowNumber(u8 x,u8 y,u32 num,u8 len,u8 size)
{
        u8 t,temp;
        u8 enshow=0;
        for(t=0;t<len;t++)
        {
                temp=(num/oled_pow(10,len-t-1))%10;
                if(enshow==0&&t<(len-1))
                {
                        if(temp==0)
                        {
                                OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
                                continue;
                        }else enshow=1;

                }
                 OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1);
        }
}

void OLED_ShowString(u8 x,u8 y,const u8 *p)
{
#define MAX_CHAR_POSX 122
#define MAX_CHAR_POSY 58
    while(*p!='\0')
    {
        if(x>MAX_CHAR_POSX){x=0;y+=16;}
        if(y>MAX_CHAR_POSY){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,12,1);
        x+=8;
        p++;
    }
}

void oled_init(void)
{
        GPIO_InitTypeDef GPIO_InitStructure;

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        
        OLED_RST_Clr();
        delay_ms(300);
        OLED_RST_Set();
        
        OLED_WR_Byte(0xAE);
        OLED_WR_Byte(0xD5);
        OLED_WR_Byte(0x80);
        OLED_WR_Byte(0xA8);
        OLED_WR_Byte(0X3F);
        OLED_WR_Byte(0xD3);
        OLED_WR_Byte(0X00);

        OLED_WR_Byte(0x40);

        OLED_WR_Byte(0x8D);
        OLED_WR_Byte(0x14);
        OLED_WR_Byte(0x20);
        OLED_WR_Byte(0x02);
        OLED_WR_Byte(0xA1);
        OLED_WR_Byte(0xC0);
        OLED_WR_Byte(0xDA);
        OLED_WR_Byte(0x12);

        OLED_WR_Byte(0x81);
        OLED_WR_Byte(0xEF);
        OLED_WR_Byte(0xD9);
        OLED_WR_Byte(0xf1);
        OLED_WR_Byte(0xDB);
        OLED_WR_Byte(0x30);

        OLED_WR_Byte(0xA4);
        OLED_WR_Byte(0xA6);
        OLED_WR_Byte(0xAF);
        OLED_Clear();
        
        delay_ms(100);
}

