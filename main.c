
#include "system.h"
#include "iic.h"
#include "u8g2_porting.h"
//==========================================================================


/*************  本地常量声明    **************/

/*************  IO口定义    **************/

/*************  本地变量声明    **************/
static uint8_t ledIndex; //显示位索引
static bool flag = 0;
/*************  本地函数声明    **************/

/****************  外部函数声明和外部变量声明 *****************/

/******************** 主函数 **************************/
void main(void)
{
  WTST = 0;  //设置程序指令延时参数，赋值为0可将CPU执行指令的速度设置为最快
  EAXFR = 1; //扩展寄存器(XFR)访问使能
  CKCON = 0; //提高访问XRAM速度

  HAL_GPIO_Init();
  P40 = 0; //LED Power On
 // HAL_Timer0_Init();
  EA = 1; //打开总中断

  HAL_I2C_Init();
  LCD_Init();
	LCD_show();

  while (1)
  {
    flag = !flag;
    P34 = flag; //输出低驱动
    ledIndex++;

    HAL_Delay(500);
  }
}
