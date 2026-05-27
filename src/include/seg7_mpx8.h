/**
 * @file seg7_mpx8.h
 * @brief 8位数码管驱动头文件
 * @details 该文件包含了8位数码管的驱动函数声明。
 * @author xcr
 * @date 2026-06-05
 * @version 1.0
 * @copyright 2026
 */

#ifndef _SEG7_MPX8_H_
#define _SEG7_MPX8_H_       

#include "common.h"

// ==================== 数码管引脚定义 ====================

#define SEG7_DATA_PORT  P0
#define SEG7_DIGIT_PORT P2

// ==================== 函数声明 ====================
/*  
 * @brief 显示整数到数码管
 * @details 该函数将指定的整数显示到8位数码管上。
 * @param num 要显示的整数，范围为 -32768 到 32767
 */
void SEG7_mpx8_display_int(int num);

/*  
 * @brief 设置8位数码管的位显示
 * @details 该函数将指定的位号和显示数据设置到8位数码管上。
 * @param digit 数码管的位号，0-7
 * @param dat 数码管显示的数字，0-18，分别对应 0-9、A-F、-、 . 和 空白
 */
void SEG7_mpx8_set_digit(unsigned char digit, unsigned char dat);

/*  
 * @brief 扫描显示8位数码管
 * @details 该函数将8位数码管显示的数字刷新到数码管上。
 */
void SEG7_mpx8_display();

#endif