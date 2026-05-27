/**
 * @file lcd1602.h
 * @brief LCD1602显示模块驱动头文件
 */

#ifndef LCD1602_H
#define LCD1602_H


#include "common.h"

// ==================== LCD1602引脚定义 ====================

sbit LCD1602_RS = P2^6;
sbit LCD1602_RW = P2^5;
sbit LCD1602_EN = P2^7;
#define LCD1602_DATA P0

// ==================== LCD1602命令定义 ====================

#define LCD1602_CMD_CLEAR       0x01    // 清屏
#define LCD1602_CMD_HOME        0x02    // 光标返回
#define LCD1602_CMD_ENTRY_MODE  0x06    // 输入模式设置
#define LCD1602_CMD_DISPLAY_ON  0x0C    // 显示开，光标关
#define LCD1602_CMD_DISPLAY_OFF 0x08    // 显示关
#define LCD1602_CMD_FUNCTION_SET 0x38   // 8位数据，2行显示，5x8点阵
#define LCD1602_CMD_SET_CGRAM   0x40    // 设置CGRAM地址
#define LCD1602_CMD_SET_DDRAM   0x80    // 设置DDRAM地址

// ==================== 函数声明 ====================

void LCD1602_Init(void);

void LCD1602_Delay(unsigned int t);
void LCD1602_WriteCmd(unsigned char cmd);
void LCD1602_WriteData(unsigned char dat);

void LCD1602_SetCursor(unsigned char row, unsigned char col);
void LCD1602_WriteString(unsigned char *str);
void LCD1602_WriteInt(int num);

#endif