
#include "system.h"
#include "iic.h"
// #include "ssd1306.h"
// #include "ssd1306_tests.h"
#include "u8g2_porting.h"
//==========================================================================


/*************  ���س�������    **************/
//typedef struct
//{
//	const uint8_t Font;    /*!< Font width in pixels */
//	const uint16_t *p_data; /*!< Pointer to data font data array */
//} Hlell;

//Hlell hello_type;

/*************  IO�ڶ���    **************/

/*************  ���ر�������    **************/

uint8 ledIndex; //��ʾλ����
int flag = 0;
/*************  ���غ�������    **************/

/****************  �ⲿ�����������ⲿ�������� *****************/

/******************** ������ **************************/
void main(void)
{
  WTST = 0;  //���ó���ָ����ʱ��������ֵΪ0�ɽ�CPUִ��ָ����ٶ�����Ϊ���
  EAXFR = 1; //��չ�Ĵ���(XFR)����ʹ��
  CKCON = 0; //��߷���XRAM�ٶ�

  HAL_GPIO_Init();
  P40 = 0; //LED Power On

 // HAL_Timer0_Init();
  EA = 1; //�����ж�

  HAL_I2C_Init();

  LCD_Init();
  LCD_show();
  // ssd1306_Init();
  // ssd1306_TestAll();
  while (1)
  {
    flag = !flag;
    P34 = flag; //���������
    ledIndex++;

    HAL_Delay(500);
  }
}
