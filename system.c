/**
 * @file system.c
 * @author songzhishuo@hotmail.com
 * @brief 系统级函数声明
 * @version 0.1
 * @date 2022-07-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "system.h"

uint32_t sysTick = 0;

/**
 * @brief 死循环延时
 * 
 * @param ms 延时时间 单位ms
 */
void HAL_Delay(unsigned int ms)
{
     u16 i;
     do
     {
          i = MAIN_Fosc / 6000;
          while (--i)
               ;
     } while (--ms);
}

/**
 * @brief 将GPIO 默认初始化为双向状态
 * 
 */
void HAL_GPIO_Init()
{
  P0M1 = 0x00;
  P0M0 = 0x00; //设置为准双向口
  P1M1 = 0x00;
  P1M0 = 0x00; //设置为准双向口
  P2M1 = 0x00;
  P2M0 = 0x00; //设置为准双向口
  P3M1 = 0x00;
  P3M0 = 0x00; //设置为准双向口
  P4M1 = 0x00;
  P4M0 = 0x00; //设置为准双向口
  P5M1 = 0x00;
  P5M0 = 0x00; //设置为准双向口
  P6M1 = 0x00;
  P6M0 = 0x00; //设置为准双向口
  P7M1 = 0x00;
  P7M0 = 0x00; //设置为准双向口     
}

/**
 * @brief HW定时器0 初始化,用来提供systemtick
 * 
 */
void HAL_Timer0_Init()
{
     AUXR = 0x80; //Timer0 set as 1T, 16 bits timer auto-reload,
     TH0 = (u8)(TIMER0_RELOAD / 256);
     TL0 = (u8)(TIMER0_RELOAD % 256);
     ET0 = 1; //Timer0 interrupt enable
     TR0 = 1; //Tiner0 run
}

/**
 * @brief 定时器0 中断反诬函数
 * 
 */
void Timer0_Isr(void) interrupt 1
{
     sysTick++;
     if (sysTick >= 0xFFFFE)
     {
          sysTick = 0;
     }
}

/**
 * @brief 获取systick的值
 * 
 * @return uint32_t 
 */
uint32_t HAL_GetTick()
{
     return (sysTick);
}
