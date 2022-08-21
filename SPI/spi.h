#ifndef __SPI_H__
#define __SPI_H__

#include "system.h"

#define U8g2_SPI_MODE   0    /* I2C 模式使能*/
#define SPI_DMA_CFG     0   /* I2C DMA 模式使能*/

sbit    SPI_CS  = P2^2;     //PIN1
sbit    SPI_MISO  = P2^4;     //PIN2
sbit    SPI_MOSI  = P2^3;     //PIN5
sbit    SPI_SCK = P2^5;     //PIN6

extern void HAL_SPI_Init();
extern void HAL_SPI_Transmit(uint8_t *Mem, uint16_t MemSize, uint32_t Timeout);
#endif /*__SPI_H__*/

