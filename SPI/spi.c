#include "spi.h"
#include "system.h"

#if (SPI_DMA_CFG == 1)
bit	SpiDmaFlag;
uint8_t xdata SpiDmaTxBuffer[256]; //_at_ DMA_TX_ADDR;
uint16_t data_index = 0;
#endif

void HAL_SPI_Init(void)
{
    SSIG = 1; //���� SS ���Ź��ܣ�ʹ�� MSTR ȷ���������������Ǵӻ�
    SPEN = 1; //ʹ�� SPI ����
    DORD = 0; //�ȷ���/�������ݵĸ�λ�� MSB��
    MSTR = 1; //��������ģʽ
    CPOL = 0; //SCLK ����ʱΪ�ߵ�ƽ��SCLK ��ǰʱ����Ϊ�½��أ���ʱ����Ϊ������
    CPHA = 0; //������ SCLK ǰʱ������������ʱ���ز���
    SPCTL = (SPCTL & ~3) | 3; //SPCTL & 0Xfc;  //SPI ʱ��Ƶ��ѡ��, 0: 4T, 1: 8T,  2: 16T,  3: 2T
    P_SW1 = (P_SW1 & ~(3<<2)) | (1<<2);     //IO���л�. 0: P1.2/P5.4 P1.3 P1.4 P1.5, 1: P2.2 P2.3 P2.4 P2.5, 2: P5.4 P4.0 P4.1 P4.3, 3: P3.5 P3.4 P3.3 P3.2

    SPI_SCK = 0;    // set clock to low initial state
    SPI_MOSI = 1;
    SPIF = 1;   //��SPIF��־
    WCOL = 1;   //��WCOL��־

#if (SPI_DMA_CFG == 1)
    /*DMA Mode*/
	DMA_SPI_STA = 0x00;
	DMA_SPI_CFG = 0xE0;		//bit7 1:Enable Interrupt
	DMA_SPI_AMT = 0x02;		//���ô������ֽ�����n+1

	DMA_SPI_TXAH = (u8)((u16)&SpiDmaTxBuffer >> 8);	//SPI�������ݴ洢��ַ
	DMA_SPI_TXAL = (u8)((u16)&SpiDmaTxBuffer);

	DMA_SPI_CFG2 = 0x00;	//01:P2.2
	DMA_SPI_CR = 0x81;		//bit7 1:ʹ�� SPI_DMA, bit6 1:��ʼ SPI_DMA ����ģʽ, bit0 1:��� SPI_DMA FIFO
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
        SPIF = 1;   //��SPIF��־
        WCOL = 1;   //��WCOL��־
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