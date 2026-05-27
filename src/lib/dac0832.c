/**
 * @file dac0832.c
 * @brief DAC0832 数模转换芯片驱动实现
 */

#include "dac0832.h"

/**
 * @brief DAC0832 时序延时
 */
static void DAC0832_Delay(void)
{
    unsigned char i;
    for(i = 0; i < 20; i++);
}

void DAC0832_Init(void)
{
    DAC0832_DATA_PORT = 0x00;
    DAC0832_WR = 1;
    DAC0832_CLR = 1;
}

void DAC0832_Write(unsigned char value)
{
    DAC0832_DATA_PORT = value;
    DAC0832_CLR = 1;
    DAC0832_WR = 0;
    DAC0832_Delay();
    DAC0832_WR = 1;
    DAC0832_Delay();
}

void DAC0832_Clear(void)
{
    DAC0832_CLR = 0;
    DAC0832_Delay();
    DAC0832_CLR = 1;
    DAC0832_Delay();
}
