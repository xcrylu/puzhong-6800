/**
 * @file isd4004.h
 * @brief ISD4004语音芯片驱动头文件
 */

#ifndef ISD4004_H
#define ISD4004_H

#include <reg51.h>
#include "spi.h"

// ==================== ISD4004引脚定义 ====================

sbit ISD4004_RST = P1^4;  // 复位引脚
sbit ISD4004_INT = P1^5;  // 中断引脚

// ==================== ISD4004命令定义 ====================
// 位序规则: 表格最左边是LSB(bit0), 最右边是MSB(bit7)
// 编码方法: 表格格式反转(MSB→LSB)，xxx用0代替，转16进制

#define ISD4004_CMD_PWR_DOWN    0x10   // STOPPWRDN: xxx0 0001 → 0001 0xxx → 0x10
#define ISD4004_CMD_PWR_UP      0x50   // POWERUP: xxx0 0101 → 0101 0xxx → 0x50
#define ISD4004_CMD_STOP        0x30   // STOP: xxx0 0011 → 0011 0xxx → 0x30
#define ISD4004_CMD_STOPWRD     0xF1   // STOPWRD: xxx1 1111 → 1111 1xxx → 0xF1
#define ISD4004_CMD_PLAY        0xF0   // PLAY: xxx0 1111 → 1111 0xxx → 0xF0
#define ISD4004_CMD_REC         0x70   // REC: xxx0 0111 → 0111 0xxx → 0x70
#define ISD4004_CMD_RINT        0x60   // RINT: xxx0 0110 → 0110 0xxx → 0x60
#define ISD4004_CMD_SETPLAY     0xF1   // SETPLAY: xxx1 1111 → 1111 1xxx → 0xF1
#define ISD4004_CMD_SETREC      0x71   // SETREC: xxx1 0111 → 0111 1xxx → 0x71

// ==================== 函数声明 ====================

/**
 * @brief 毫秒级延时函数
 * @param x 延时毫秒数
 */
// void ISD4004_DelayMs(unsigned int x);

/**
 * @brief 初始化ISD4004语音芯片
 */
void ISD4004_Init(void);

/**
 * @brief 向ISD4004发送命令
 * @param opcode 8位操作码
 */
void ISD4004_SendCmd(unsigned char opcode);

/**
 * @brief 读取ISD4004状态寄存器
 * @return 状态寄存器值
 */
unsigned char ISD4004_ReadStatus(void);

/**
 * @brief 从当前地址播放语音
 */
void ISD4004_Play(void);

/**
 * @brief 从指定地址播放语音
 * @param addr 播放起始地址
 */
void ISD4004_SetPlay(unsigned int addr);

/**
 * @brief 停止播放/录音
 */
void ISD4004_Stop(void);

/**
 * @brief 从当前地址开始录音
 */
void ISD4004_Record(void);

/**
 * @brief 从指定地址开始录音
 * @param addr 录音起始地址
 */
void ISD4004_SetRec(unsigned int addr);

/**
 * @brief 复位ISD4004
 */
void ISD4004_Reset(void);

#endif