#include "spi.h"
#include "system.h"

#if (SPI_DMA_CFG == 1)
bit	SpiDmaFlag;
uint8_t xdata SpiDmaTxBuffer[256]; //_at_ DMA_TX_ADDR;
uint16_t data_index = 0;
#endif

void HAL_SPI_Init(void)
{
    SSIG = 1; //忽略 SS 引脚功能，使用 MSTR 确定器件是主机还是从机
    SPEN = 1; //使能 SPI 功能
    DORD = 0; //先发送/接收数据的高位（ MSB）
    MSTR = 1; //设置主机模式
    CPOL = 0; //SCLK 空闲时为高电平，SCLK 的前时钟沿为下降沿，后时钟沿为上升沿
    CPHA = 0; //数据在 SCLK 前时钟沿驱动，后时钟沿采样
    SPCTL = (SPCTL & ~3) | 3; //SPCTL & 0Xfc;  //SPI 时钟频率选择, 0: 4T, 1: 8T,  2: 16T,  3: 2T
    P_SW1 = (P_SW1 & ~(3<<2)) | (1<<2);     //IO口切换. 0: P1.2/P5.4 P1.3 P1.4 P1.5, 1: P2.2 P2.3 P2.4 P2.5, 2: P5.4 P4.0 P4.1 P4.3, 3: P3.5 P3.4 P3.3 P3.2

    SPI_SCK = 0;    // set clock to low initial state
    SPI_MOSI = 1;
    SPIF = 1;   //清SPIF标志
    WCOL = 1;   //清WCOL标志

#if (SPI_DMA_CFG == 1)
    /*DMA Mode*/
	DMA_SPI_STA = 0x00;
	DMA_SPI_CFG = 0xE0;		//bit7 1:Enable Interrupt
	DMA_SPI_AMT = 0x02;		//设置传输总字节数：n+1

	DMA_SPI_TXAH = (u8)((u16)&SpiDmaTxBuffer >> 8);	//SPI发送数据存储地址
	DMA_SPI_TXAL = (u8)((u16)&SpiDmaTxBuffer);

	DMA_SPI_CFG2 = 0x00;	//01:P2.2
	DMA_SPI_CR = 0x81;		//bit7 1:使能 SPI_DMA, bit6 1:开始 SPI_DMA 主机模式, bit0 1:清除 SPI_DMA FIFO
#endif    
}

void HAL_SPI_Transmit(uint8_t *Mem, uint16_t MemSize, uint32_t Timeout)
{		
    uint16_t i = 0; 
#if (SPI_DMA_CFG == 1)
    if(MemSize > 0)
    {
        while(SpiDmaFlag); 
        for(i = 0; i< MemSize; i++)
        {
            SpiDmaTxBuffer[i] = Mem[i];
        }  
        SpiDmaFlag = 1;
        DMA_SPI_AMT = MemSize - 1;
        DMA_SPI_CR |= 0x40;
    }
#else
    for(i = 0; i< MemSize; i++)
    {
        SPDAT = *(Mem + i);
        while(SPIF == 0) ;
        SPIF = 1;   //清SPIF标志
        WCOL = 1;   //清WCOL标志
    }
#endif

}

#if (SPI_DMA_CFG == 1)
void SPI_DMA_Interrupt(void) interrupt 13		
{
	DMA_SPI_STA = 0;
	SpiDmaFlag = 0;
	
}
#endif