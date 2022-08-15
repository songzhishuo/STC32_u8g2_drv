#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <STC32G.H>

#include <intrins.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include "intrins.h"

#define __STC_51__      1

#define MAIN_Fosc 11059200L //定义主时钟
#define TIMER0_RELOAD (65536UL - (MAIN_Fosc / 1000)) //Timer 0 中断频率, 1000次/秒

typedef unsigned char u8;  //  8 bits
typedef unsigned int u16;  // 16 bits
typedef unsigned long u32; // 32 bits

typedef signed char int8;  //  8 bits
typedef signed int int16;  // 16 bits
typedef signed long int32; // 32 bits

typedef unsigned char uint8;  //  8 bits
typedef unsigned int uint16;  // 16 bits
typedef unsigned long uint32; // 32 bits

typedef signed char int8_t;  //  8 bits
typedef signed int int16_t;  // 16 bits
typedef signed long int32_t; // 32 bits

typedef unsigned char uint8_t;  //  8 bits
typedef unsigned int uint16_t;  // 16 bits
typedef unsigned long uint32_t; // 32 bits
typedef unsigned int bool;

#define TRUE 1
#define FALSE 0

#define true TRUE
#define false FALSE

#define HAL_MAX_DELAY 0xFF

#ifndef NULL
#define NULL ((void *) 0)
#endif

extern void HAL_GPIO_Init();
extern void HAL_Timer0_Init();

extern void HAL_Delay(unsigned int ms);
extern uint32_t HAL_GetTick();
#endif