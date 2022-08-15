#include "spi.h"
#include "system.h"



void HAL_SPI_Init(void)
{
    SSIG = 1; //���� SS ���Ź��ܣ�ʹ�� MSTR ȷ���������������Ǵӻ�
    SPEN = 1; //ʹ�� SPI ����
    DORD = 0; //�ȷ���/�������ݵĸ�λ�� MSB��
    MSTR = 1; //��������ģʽ
    CPOL = 0; //SCLK ����ʱΪ�ߵ�ƽ��SCLK ��ǰʱ����Ϊ�½��أ���ʱ����Ϊ������
    CPHA = 0; //������ SCLK ǰʱ������������ʱ���ز���
    SPCTL = (SPCTL & ~3) | 0; //SPCTL & 0Xfc;  //SPI ʱ��Ƶ��ѡ��, 0: 4T, 1: 8T,  2: 16T,  3: 2T
    P_SW1 = (P_SW1 & ~(3<<2)) | (1<<2);     //IO���л�. 0: P1.2/P5.4 P1.3 P1.4 P1.5, 1: P2.2 P2.3 P2.4 P2.5, 2: P5.4 P4.0 P4.1 P4.3, 3: P3.5 P3.4 P3.3 P3.2

    SPI_SCK = 0;    // set clock to low initial state
    SPI_MOSI = 0;
    SPIF = 1;   //��SPIF��־
    WCOL = 1;   //��WCOL��־
}

void HAL_SPI_Transmit(uint8_t *Mem, uint16_t MemSize, uint32_t Timeout)
{
    uint32_t i = 0;
    for(i = 0; i< MemSize; i++)
    {
        SPDAT = *(Mem + i);
        while(SPIF == 0) ;
        SPIF = 1;   //��SPIF��־
        WCOL = 1;   //��WCOL��־
    }

}