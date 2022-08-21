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
    I2CMSCR = 0x01; //发送START命令
    Wait();
}

void SendData(char dat)
{
    I2CTXD = dat;   //写数据到数据缓冲区
    I2CMSCR = 0x02; //发送SEND命令
    Wait();
}

void RecvACK()
{
    I2CMSCR = 0x03; //发送读ACK命令
    Wait();
}

char RecvData()
{
    I2CMSCR = 0x04; //发送RECV命令
    Wait();
    return I2CRXD;
}

void SendACK()
{
    I2CMSST = 0x00; //设置ACK信号
    I2CMSCR = 0x05; //发送ACK命令
    Wait();
}

void SendNAK()
{
    I2CMSST = 0x01; //设置NAK信号
    I2CMSCR = 0x05; //发送ACK命令
    Wait();
}

void Stop()
{
    I2CMSCR = 0x06; //发送STOP命令
    Wait();
}
#endif

void HAL_I2C_Mem_Write(uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    uint16_t i = 0;
    MemAddSize = 0;
    Timeout = 0;

#if (IIC_DMA_CFG != 1)   
    Start();                       //发送起始命令
    SendData((uint8_t)DevAddress); //发送设备地址+写命令
    RecvACK();
    if (MemAddSize != 0)
    {
        SendData((uint8_t)MemAddress); //发送存储地址
        RecvACK();
    }

    for (i = 0; i < Size; i++)
    {
        SendData(*(pData + i));
        RecvACK();
    }
    Stop(); //发送停止命令
#else

    DmaTxBuffer[0] = (uint8_t)DevAddress;
    memcpy(&DmaTxBuffer[1],pData,Size);
    while(I2CMSST & 0x80);    //检查I2C控制器忙碌状态
    DmaTxFlag = 1;

    I2CMSST = 0x00;
    I2CMSCR = 0x89;             //set cmd //write_start_combo
    DMA_I2C_CR = 0x01;
    DMA_I2CT_AMT = Size;	//设置传输总字节数(低8位)：number + 设备地址 + 存储地址
    DMA_I2CT_AMTH = 0x00;		//设置传输总字节数(高8位)：n+1
    DMA_I2C_ST1 = Size;		//设置需要传输字节数(低8位)：number + 设备地址 + 存储地址
    DMA_I2C_ST2 = 0x00;			//设置需要传输字节数(高8位)：n+1
    DMA_I2CT_CR |= 0x40;		//bit7 1:使能 I2CT_DMA, bit6 1:开始 I2CT_DMA

    while(DmaTxFlag);         //DMA忙检测
    DMA_I2C_CR = 0x00;
#endif
}

void HAL_I2C_Init()
{
    I2C_S1 = 0; // I2C功能脚选择，00:P1.5,P1.4; 01:P2.5,P2.4; 11:P3.2,P3.3
    I2C_S0 = 0;
    I2CCFG = 0xe0; //使能I2C主机模式
    I2CMSST = 0x00;

#if (IIC_DMA_CFG == 1)
	DMA_I2CT_STA = 0x00;
	DMA_I2CT_CFG = 0x80;	//bit7 1:Enable Interrupt
	DMA_I2CT_AMT = 0x1f;	//设置传输总字节数(低8位)：n+1
	DMA_I2CT_AMTH = 0x00;	//设置传输总字节数(高8位)：n+1

	DMA_I2CT_TXAH = (u8)((u16)&DmaTxBuffer >> 8);	//I2C发送数据存储地址
	DMA_I2CT_TXAL = (u8)((u16)&DmaTxBuffer);

	DMA_I2CT_CR = 0x80;		//bit7 1:使能 I2CT_DMA, bit6 1:开始 I2CT_DMA

	DMA_I2C_ST1 = 0xff;		//设置需要传输字节数(低8位)：n+1
	DMA_I2C_ST2 = 0x00;		//设置需要传输字节数(高8位)：n+1
#endif    
}

#if (IIC_DMA_CFG == 1)
void I2C_DMA_Interrupt(void) interrupt 13
{
	if(DMA_I2CT_STA & 0x01)   //发送完成
	{
		DMA_I2CT_STA &= ~0x01;  //清除标志位
		DmaTxFlag = 0;
	}
	if(DMA_I2CT_STA & 0x04)   //数据覆盖
	{
		DMA_I2CT_STA &= ~0x04;  //清除标志位
	}
	
	if(DMA_I2CR_STA & 0x01)   //接收完成
	{
		DMA_I2CR_STA &= ~0x01;  //清除标志位
	}
	if(DMA_I2CR_STA & 0x02)   //数据丢弃
	{
		DMA_I2CR_STA &= ~0x02;  //清除标志位
	}
}
#endif

#if (IIC_DMA_CFG == 1)
void I2C_Interrupt() interrupt 24
{
	I2CMSST &= ~0x40;       //I2C指令发送完成状态清除

	if(DMA_I2C_CR & 0x04)   //ACKERR
	{
		DMA_I2C_CR &= ~0x04;  //发数据后收到NAK
	}
}
#endif


