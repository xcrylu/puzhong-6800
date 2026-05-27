/**
 * @file uart.h
 * @brief UART串口通信驱动头文件
 */

#ifndef __UART_H__
#define __UART_H__

#include <reg52.h>
#include "common.h"

// ==================== 函数声明 ====================

void uart_init(void);
void uart_send_byte(unsigned char byte);
unsigned char uart_recv_byte(void);
void uart_send_str(char *str);

#endif