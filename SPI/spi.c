#include "spi.h"
#include "system.h"



void HAL_SPI_Init(void)
{
    SSIG = 1; //忽略 SS 引脚功能，使用 MSTR 确定器件是主机还是从机
    SPEN = 1; //使能 SPI 功能
    DORD = 0; //先发送/接收数据的高位（ MSB）
    MSTR = 1; //设置主机模式
    CPOL = 0; //SCLK 空闲时为高电平，SCLK 的前时钟沿为下降沿，后时钟沿为上升沿
    CPHA = 0; //数据在 SCLK 前时钟沿驱动，后时钟沿采样
    SPCTL = (SPCTL & ~3) | 0; //SPCTL & 0Xfc;  //SPI 时钟频率选择, 0: 4T, 1: 8T,  2: 16T,  3: 2T
    P_SW1 = (P_SW1 & ~(3<<2)) | (1<<2);     //IO口切换. 0: P1.2/P5.4 P1.3 P1.4 P1.5, 1: P2.2 P2.3 P2.4 P2.5, 2: P5.4 P4.0 P4.1 P4.3, 3: P3.5 P3.4 P3.3 P3.2

    SPI_SCK = 0;    // set clock to low initial state
    SPI_MOSI = 0;
    SPIF = 1;   //清SPIF标志
    WCOL = 1;   //清WCOL标志
}

void HAL_SPI_Transmit(uint8_t *Mem, uint16_t MemSize, uint32_t Timeout)
{
    uint32_t i = 0;
    for(i = 0; i< MemSize; i++)
    {
        SPDAT = *(Mem + i);
        while(SPIF == 0) ;
        SPIF = 1;   //清SPIF标志
        WCOL = 1;   //清WCOL标志
    }

}