/**
 * @file dac0832.h
 * @brief DAC0832 数模转换芯片驱动头文件
 */

#ifndef _DAC0832_H_
#define _DAC0832_H_

#include "common.h"

// ==================== DAC0832 引脚定义 ====================
// 根据实际硬件连接修改数据口和控制口
#define DAC0832_DATA_PORT P1

sbit DAC0832_WR  = P2^2;  // 写脉冲，低电平有效
sbit DAC0832_CLR = P2^3;  // 清零复位，低电平有效

// ==================== 函数声明 ====================

/**
 * @brief 初始化 DAC0832
 */
void DAC0832_Init(void);

/**
 * @brief 向 DAC0832 写入 8 位数据
 * @param value 0~255 数字量
 */
void DAC0832_Write(unsigned char value);

/**
 * @brief 清零 DAC0832 输出
 */
void DAC0832_Clear(void);

#endif // _DAC0832_H_
