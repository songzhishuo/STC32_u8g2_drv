#ifndef __IIC_H__
#define __IIC_H__

#include "system.h"

#define U8g2_I2C_MODE   1	/* I2C 模式使能*/
#define IIC_DMA_CFG     1	/* I2C DMA 模式使能*/

extern void HAL_I2C_Init();
extern void HAL_I2C_Mem_Write(uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);

#endif /*__IIC_H__*/

