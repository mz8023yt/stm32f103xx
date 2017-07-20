#include "bsp_oled.h"
#include "bsp_i2c.h"
#include "bsp_usart.h"
#include "bsp_systick.h"
#include "stdlib.h"
#include "bsp_oled_font.h"

/**
 * @brief
 */
int oled_write_data(u8 dat)
{
        OLED_DC = 0;
        i2c_send_byte(&i2c_bus_1, dat);
        return 0;
}

int oled_write_command(u8 cmd)
{
        OLED_DC = 1;
        i2c_send_byte(&i2c_bus_1, cmd);
        return 0;
}

void OLED_WR_Byte(u8 dat,u8 cmd)
{
        u8 i;
        if(cmd)
                OLED_DC = 1;
        else 
                OLED_DC = 0;
        i2c_send_byte(&i2c_bus_1, cmd);
        OLED_DC = 1;
} 

void oled_fill(u8 bmp_dat)
{
        u8 y,x;
        for(y=0;y<8;y++)
        {
                oled_write_command(0xb0+y);
                oled_write_command(0x01);
                oled_write_command(0x10);
                for(x=0;x<128;x++)
                        oled_write_data(bmp_dat);
        }
}

//开启OLED显示    
void oled_display_on(void)
{
        oled_write_command(0X8D);  //SET DCDC命令
        oled_write_command(0X14);  //DCDC ON
        oled_write_command(0XAF);  //DISPLAY ON
}
//关闭OLED显示     
void oled_display_off(void)
{
        oled_write_command(0X8D);  //SET DCDC命令
        oled_write_command(0X10);  //DCDC OFF
        oled_write_command(0XAE);  //DISPLAY OFF
}                                            

//初始化SSD1306
void oled_init(void)
{
        /* 设置 */
        GPIO_InitTypeDef GPIO_InitStructure;

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        
        OLED_RST = 0;
        delay_ms(100);
        OLED_RST = 1; 
        oled_write_command(0xAE); //关闭显示
        oled_write_command(0xD5); //设置时钟分频因子,震荡频率
        oled_write_command(0x80); //[3:0],分频因子;[7:4],震荡频率
        oled_write_command(0xA8); //设置驱动路数
        oled_write_command(0X3F); //默认0X3F(1/64) 
        oled_write_command(0xD3); //设置显示偏移
        oled_write_command(0X00); //默认为0
        oled_write_command(0x40); //设置显示开始行 [5:0],行数.
        oled_write_command(0x8D); //电荷泵设置
        oled_write_command(0x14); //bit2，开启/关闭
        oled_write_command(0x20); //设置内存地址模式
        oled_write_command(0x02); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
        oled_write_command(0xA1); //段重定义设置,bit0:0,0->0;1,0->127;
        oled_write_command(0xC0); //设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
        oled_write_command(0xDA); //设置COM硬件引脚配置
        oled_write_command(0x12); //[5:4]配置
        oled_write_command(0x81); //对比度设置
        oled_write_command(0xEF); //1~255;默认0X7F (亮度设置,越大越亮)
        oled_write_command(0xD9); //设置预充电周期
        oled_write_command(0xf1); //[3:0],PHASE 1;[7:4],PHASE 2;
        oled_write_command(0xDB); //设置VCOMH 电压倍率
        oled_write_command(0x30); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
        oled_write_command(0xA4); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
        oled_write_command(0xA6); //设置显示方式;bit0:1,反相显示;0,正常显示
        oled_write_command(0xAF); //开启显示
        
}  


