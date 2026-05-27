/**
 * @file keyScan.h
 * @brief 矩阵键盘扫描驱动头文件
 */

#ifndef _KEY_SCAN_H_
#define _KEY_SCAN_H_


#include "common.h"

// ==================== 按键模块引脚定义 ====================

#define KEY_PORT P1

// ==================== 函数声明 ====================

unsigned char KeyScan(void);
unsigned char KeyScan_v2(void); //无消抖

#endif