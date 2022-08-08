#include "iic.h"
#include "system.h"


void Wait()
{
    while (!(I2CMSST & 0x40));
    I2CMSST &= ~0x40;
}

void Start()
{
    I2CMSCR = 0x01;                         //����START����
    Wait();
}

void SendData(char dat)
{
    I2CTXD = dat;                           //д���ݵ����ݻ�����
    I2CMSCR = 0x02;                         //����SEND����
    Wait();
}

void RecvACK()
{
    I2CMSCR = 0x03;                         //���Ͷ�ACK����
    Wait();
}

char RecvData()
{
    I2CMSCR = 0x04;                         //����RECV����
    Wait();
    return I2CRXD;
}

void SendACK()
{
    I2CMSST = 0x00;                         //����ACK�ź�
    I2CMSCR = 0x05;                         //����ACK����
    Wait();
}

void SendNAK()
{
    I2CMSST = 0x01;                         //����NAK�ź�
    I2CMSCR = 0x05;                         //����ACK����
    Wait();
}

void Stop()
{
    I2CMSCR = 0x06;                         //����STOP����
    Wait();
}

void HAL_I2C_Mem_Write(uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
//void WriteNbyte(u8 addr, u8 *p, u8 number)  /*  WordAddress,First Data Address,Byte lenth   */
{
	MemAddSize = 0;
	Timeout = 0;
    Start();                                //������ʼ����
    SendData((uint8_t)DevAddress);                         //�����豸��ַ+д����
    RecvACK();
    if(MemAddSize != 0)
    {
        SendData((uint8_t)MemAddress);                         //���ʹ洢��ַ
        RecvACK();
    }
    do
    {
        SendData(*pData++);
        RecvACK();
    }
    while(--Size);
    Stop();                                 //����ֹͣ����
}

void ReadNbyte(u8 addr, u8 *p, u8 number)   /*  WordAddress,First Data Address,Byte lenth   */
{
    Start();                                //������ʼ����
    SendData(0);                         //�����豸��ַ+д����
    RecvACK();
    SendData(addr);                         //���ʹ洢��ַ
    RecvACK();
    Start();                                //������ʼ����
    SendData(0);                         //�����豸��ַ+������
    RecvACK();
    do
    {
        *p = RecvData();
        p++;
        if(number != 1) SendACK();          //send ACK
    }
    while(--number);
    SendNAK();                              //send no ACK	
    Stop();                                 //����ֹͣ����
}


void HAL_I2C_Init()
{
    I2C_S1 =0;   //I2C���ܽ�ѡ��00:P1.5,P1.4; 01:P2.5,P2.4; 11:P3.2,P3.3
    I2C_S0 =1;
    I2CCFG = 0xe0;              //ʹ��I2C����ģʽ
    I2CMSST = 0x00;

    EA = 1;     //�����ж�
}
