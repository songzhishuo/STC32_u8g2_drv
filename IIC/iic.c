#include "iic.h"
#include "system.h"

 #if (IIC_DMA_CFG == 1)
static bit	DmaTxFlag=0;
static uint8_t xdata DmaTxBuffer[32] = {0};
#endif

#if (IIC_DMA_CFG != 1)
void Wait()
{
    while (!(I2CMSST & 0x40))
        ;
    I2CMSST &= ~0x40;
}

void Start()
{
    I2CMSCR = 0x01; //����START����
    Wait();
}

void SendData(char dat)
{
    I2CTXD = dat;   //д���ݵ����ݻ�����
    I2CMSCR = 0x02; //����SEND����
    Wait();
}

void RecvACK()
{
    I2CMSCR = 0x03; //���Ͷ�ACK����
    Wait();
}

char RecvData()
{
    I2CMSCR = 0x04; //����RECV����
    Wait();
    return I2CRXD;
}

void SendACK()
{
    I2CMSST = 0x00; //����ACK�ź�
    I2CMSCR = 0x05; //����ACK����
    Wait();
}

void SendNAK()
{
    I2CMSST = 0x01; //����NAK�ź�
    I2CMSCR = 0x05; //����ACK����
    Wait();
}

void Stop()
{
    I2CMSCR = 0x06; //����STOP����
    Wait();
}
#endif

void HAL_I2C_Mem_Write(uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    uint16_t i = 0;
    MemAddSize = 0;
    Timeout = 0;

#if (IIC_DMA_CFG != 1)   
    Start();                       //������ʼ����
    SendData((uint8_t)DevAddress); //�����豸��ַ+д����
    RecvACK();
    if (MemAddSize != 0)
    {
        SendData((uint8_t)MemAddress); //���ʹ洢��ַ
        RecvACK();
    }

    for (i = 0; i < Size; i++)
    {
        SendData(*(pData + i));
        RecvACK();
    }
    Stop(); //����ֹͣ����
#else

    DmaTxBuffer[0] = (uint8_t)DevAddress;
    memcpy(&DmaTxBuffer[1],pData,Size);
    while(I2CMSST & 0x80);    //���I2C������æµ״̬
    DmaTxFlag = 1;

    I2CMSST = 0x00;
    I2CMSCR = 0x89;             //set cmd //write_start_combo
    DMA_I2C_CR = 0x01;
    DMA_I2CT_AMT = Size;	//���ô������ֽ���(��8λ)��number + �豸��ַ + �洢��ַ
    DMA_I2CT_AMTH = 0x00;		//���ô������ֽ���(��8λ)��n+1
    DMA_I2C_ST1 = Size;		//������Ҫ�����ֽ���(��8λ)��number + �豸��ַ + �洢��ַ
    DMA_I2C_ST2 = 0x00;			//������Ҫ�����ֽ���(��8λ)��n+1
    DMA_I2CT_CR |= 0x40;		//bit7 1:ʹ�� I2CT_DMA, bit6 1:��ʼ I2CT_DMA

    while(DmaTxFlag);         //DMAæ���
    DMA_I2C_CR = 0x00;
#endif
}

void HAL_I2C_Init()
{
    I2C_S1 = 0; // I2C���ܽ�ѡ��00:P1.5,P1.4; 01:P2.5,P2.4; 11:P3.2,P3.3
    I2C_S0 = 0;
    I2CCFG = 0xe0; //ʹ��I2C����ģʽ
    I2CMSST = 0x00;

#if (IIC_DMA_CFG == 1)
	DMA_I2CT_STA = 0x00;
	DMA_I2CT_CFG = 0x80;	//bit7 1:Enable Interrupt
	DMA_I2CT_AMT = 0x1f;	//���ô������ֽ���(��8λ)��n+1
	DMA_I2CT_AMTH = 0x00;	//���ô������ֽ���(��8λ)��n+1

	DMA_I2CT_TXAH = (u8)((u16)&DmaTxBuffer >> 8);	//I2C�������ݴ洢��ַ
	DMA_I2CT_TXAL = (u8)((u16)&DmaTxBuffer);

	DMA_I2CT_CR = 0x80;		//bit7 1:ʹ�� I2CT_DMA, bit6 1:��ʼ I2CT_DMA

	DMA_I2C_ST1 = 0xff;		//������Ҫ�����ֽ���(��8λ)��n+1
	DMA_I2C_ST2 = 0x00;		//������Ҫ�����ֽ���(��8λ)��n+1
#endif    
}

#if (IIC_DMA_CFG == 1)
void I2C_DMA_Interrupt(void) interrupt 13
{
	if(DMA_I2CT_STA & 0x01)   //�������
	{
		DMA_I2CT_STA &= ~0x01;  //�����־λ
		DmaTxFlag = 0;
	}
	if(DMA_I2CT_STA & 0x04)   //���ݸ���
	{
		DMA_I2CT_STA &= ~0x04;  //�����־λ
	}
	
	if(DMA_I2CR_STA & 0x01)   //�������
	{
		DMA_I2CR_STA &= ~0x01;  //�����־λ
	}
	if(DMA_I2CR_STA & 0x02)   //���ݶ���
	{
		DMA_I2CR_STA &= ~0x02;  //�����־λ
	}
}
#endif

#if (IIC_DMA_CFG == 1)
void I2C_Interrupt() interrupt 24
{
	I2CMSST &= ~0x40;       //I2Cָ������״̬���

	if(DMA_I2C_CR & 0x04)   //ACKERR
	{
		DMA_I2C_CR &= ~0x04;  //�����ݺ��յ�NAK
	}
}
#endif


