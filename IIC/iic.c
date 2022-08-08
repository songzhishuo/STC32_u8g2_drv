#include "iic.h"
#include "system.h"


void Wait()
{
    while (!(I2CMSST & 0x40));
    I2CMSST &= ~0x40;
}

void Start()
{
    I2CMSCR = 0x01;                         //发送START命令
    Wait();
}

void SendData(char dat)
{
    I2CTXD = dat;                           //写数据到数据缓冲区
    I2CMSCR = 0x02;                         //发送SEND命令
    Wait();
}

void RecvACK()
{
    I2CMSCR = 0x03;                         //发送读ACK命令
    Wait();
}

char RecvData()
{
    I2CMSCR = 0x04;                         //发送RECV命令
    Wait();
    return I2CRXD;
}

void SendACK()
{
    I2CMSST = 0x00;                         //设置ACK信号
    I2CMSCR = 0x05;                         //发送ACK命令
    Wait();
}

void SendNAK()
{
    I2CMSST = 0x01;                         //设置NAK信号
    I2CMSCR = 0x05;                         //发送ACK命令
    Wait();
}

void Stop()
{
    I2CMSCR = 0x06;                         //发送STOP命令
    Wait();
}

void HAL_I2C_Mem_Write(uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
//void WriteNbyte(u8 addr, u8 *p, u8 number)  /*  WordAddress,First Data Address,Byte lenth   */
{
	MemAddSize = 0;
	Timeout = 0;
    Start();                                //发送起始命令
    SendData((uint8_t)DevAddress);                         //发送设备地址+写命令
    RecvACK();
    if(MemAddSize != 0)
    {
        SendData((uint8_t)MemAddress);                         //发送存储地址
        RecvACK();
    }
    do
    {
        SendData(*pData++);
        RecvACK();
    }
    while(--Size);
    Stop();                                 //发送停止命令
}

void ReadNbyte(u8 addr, u8 *p, u8 number)   /*  WordAddress,First Data Address,Byte lenth   */
{
    Start();                                //发送起始命令
    SendData(0);                         //发送设备地址+写命令
    RecvACK();
    SendData(addr);                         //发送存储地址
    RecvACK();
    Start();                                //发送起始命令
    SendData(0);                         //发送设备地址+读命令
    RecvACK();
    do
    {
        *p = RecvData();
        p++;
        if(number != 1) SendACK();          //send ACK
    }
    while(--number);
    SendNAK();                              //send no ACK	
    Stop();                                 //发送停止命令
}


void HAL_I2C_Init()
{
    I2C_S1 =0;   //I2C功能脚选择，00:P1.5,P1.4; 01:P2.5,P2.4; 11:P3.2,P3.3
    I2C_S0 =1;
    I2CCFG = 0xe0;              //使能I2C主机模式
    I2CMSST = 0x00;

    EA = 1;     //打开总中断
}
