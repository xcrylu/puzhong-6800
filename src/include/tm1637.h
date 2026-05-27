/**
 * @file tm1637.h
 * @brief TM1637数码管驱动头文件
 *
 * 实现TM1637的初始化、显示、按键读取等功能
 */

#ifndef _TM1637_H_
#define _TM1637_H_

#include <reg51.h>
#include "common.h"

// ==================== TM1637引脚定义 ====================
sbit TM1637_CLK = P3^5;
sbit TM1637_DIO = P3^4;

// ==================== 亮度等级定义 ====================
#define BRIGHT_0 0x00
#define BRIGHT_1 0x01
#define BRIGHT_2 0x02
#define BRIGHT_3 0x03
#define BRIGHT_4 0x04
#define BRIGHT_5 0x05
#define BRIGHT_6 0x06
#define BRIGHT_7 0x07

// ==================== 函数声明 ====================

// void TM1637_Delay(void);
// void TM1637_Start(void);
// void TM1637_Stop(void);
// bit TM1637_Write(unsigned char dat);
// unsigned char TM1637_ReadByte(void);
void TM1637_Init(void);
void TM1637_SetBright(unsigned char bright);
void TM1637_DisplayInt(int num);
void TM1637_DisplayFloat(float num);
void TM1637_Clear(void);
unsigned char TM1637_ReadKey(void);

#endif