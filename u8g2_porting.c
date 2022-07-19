#include "u8g2_porting.h"
#include "system.h"
#include "iic.h"
#include "u8g2.h" /*u8g图形库*/

static u8g2_t u8g2; //首先定义u8g2的对象

static uint8_t u8x8_stm32_gpio_and_delay(U8X8_UNUSED u8x8_t *u8x8,
                                         U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int,
                                         U8X8_UNUSED void *arg_ptr);

static uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
                                      void *arg_ptr);

static uint8_t u8x8_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
                                void *arg_ptr);


void LCD_Init()
{
    u8g2_Setup_ssd1306_i2c_128x64_noname_1(&u8g2, U8G2_R2, u8x8_byte_hw_i2c, u8x8_stm32_gpio_and_delay);
    //u8g2_Setup_st7565_jlx12864_1(&u8g2, U8G2_R2, u8x8_byte_4wire_hw_spi, u8x8_stm32_gpio_and_delay);
    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);
}


void First_Src()
{
    uint8_t str_buf0[32] = {0};
    uint8_t str_buf1[32] = {0};
    uint8_t str_buf2[32] = {0};

    uint8_t x = 0, y = 0;

    u8g2_SetFontMode(&u8g2, 1);  // Transparent
    u8g2_SetFontDirection(&u8g2, 0);
    u8g2_SetFont(&u8g2, u8g2_font_u8glib_4_tf);

    /*显示时间*/
    y = y+20;
    u8g2_DrawStr(&u8g2, 0, y, (const char *)"Smart");

    // u8g2_SetFont(&u8g2, u8g2_font_fub17_tf);
    u8g2_DrawStr(&u8g2, 65, y, (const char *)" meters");


    // u8g2_SetFont(&u8g2, u8g2_font_fub17_tf);

    sprintf(str_buf0, " volt : %.03f V", 12.11);
    sprintf(str_buf1, " curr : %.03f A", 12.22);
    sprintf(str_buf2, " power : %.03f W", 11);

    y = y + 12; 
    u8g2_DrawStr(&u8g2, 5, y, (const char *)str_buf0);       //显示电压
    y = y + 12; 
    u8g2_DrawStr(&u8g2, 5, y, (const char *)str_buf1);       //显示电流
    y = y + 12; 
    u8g2_DrawStr(&u8g2, 5, y, (const char *)str_buf2);       //显示功
}

void LCD_show()
{
    u8g2_FirstPage(&u8g2);
    do
    {
        First_Src();
    } while (u8g2_NextPage(&u8g2));

}

/*
  -----------x
  |
  |
  y
*/
/*U8g2 hook*/
static uint8_t u8x8_byte_4wire_hw_spi(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int,
                                      void *arg_ptr)
{
#if 0    
    uint8_t *data = NULL;
    switch (msg)
    {
    case U8X8_MSG_BYTE_SEND:
        data = (uint8_t *)arg_ptr;
        do
        {
            HAL_SPI_Transmit(&hspi1, (data++), 1, 0x00FF);
            arg_int--;
        } while (arg_int > 0);
        break;
    case U8X8_MSG_BYTE_INIT:
        break;
    case U8X8_MSG_BYTE_SET_DC:
        HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, arg_int);
        break;
    case U8X8_MSG_BYTE_START_TRANSFER:
        break;
    case U8X8_MSG_BYTE_END_TRANSFER:
        break;
    default:
        return 0;
    }
#endif    
    return 1;
}

static uint8_t u8x8_stm32_gpio_and_delay(U8X8_UNUSED u8x8_t *u8x8,
                                         U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int,
                                         U8X8_UNUSED void *arg_ptr)
{
    switch (msg)
    {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:
        HAL_Delay(1);
        break;
    case U8X8_MSG_DELAY_MILLI:
        HAL_Delay(arg_int);

        break;
    case U8X8_MSG_GPIO_I2C_CLOCK:
        break;

    case U8X8_MSG_GPIO_I2C_DATA:
        break;

#if 0               //TODO：SPI后续实现
    case U8X8_MSG_GPIO_CS:
        //GPIO_WriteBit(OLED_CS_GPIO_Port, OLED_CS_Pin, arg_int);
        HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, arg_int);
    case U8X8_MSG_GPIO_DC:
        HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, arg_int);
        //GPIO_WriteBit(OLED_DC_GPIO_Port, OLED_DC_Pin, arg_int);
        break;
    case U8X8_MSG_GPIO_RESET:
        HAL_GPIO_WritePin(LCD_RES_GPIO_Port, LCD_RES_Pin, arg_int);
        //GPIO_WriteBit(OLED_RST_GPIO_Port, OLED_RST_Pin, arg_int);
        break;
#endif
    }
    return 1;
}

uint8_t u8x8_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    static uint8_t buffer[32]; /* u8g2/u8x8 will never send more than 32 bytes between START_TRANSFER and END_TRANSFER */
    static uint8_t buf_idx;
    uint8_t *p_data;

    switch (msg)
    {

    case U8X8_MSG_BYTE_SEND:
        p_data = (uint8_t *)arg_ptr;
        while (arg_int > 0)
        {
            buffer[buf_idx++] = *p_data;
            p_data++;
            arg_int--;
        }
        break;

    case U8X8_MSG_BYTE_INIT:
        /* add your custom code to init i2c subsystem */
        break;

    case U8X8_MSG_BYTE_START_TRANSFER:
        buf_idx = 0;
        break;

    case U8X8_MSG_BYTE_END_TRANSFER:
        HAL_I2C_Mem_Write(u8x8_GetI2CAddress(u8x8), 0x00, 0, &buffer, buf_idx, HAL_MAX_DELAY);
        break;

    default:
        return 0;
    }
    return 1;
}