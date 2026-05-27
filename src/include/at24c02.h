/**
 * @file at24c02.h
 * @brief AT24C02 EEPROM芯片驱动头文件
 *
 * 提供AT24C02的读写接口，支持单字节和多字节读写
 */

#ifndef _AT24C02_H_
#define _AT24C02_H_


#include "common.h"

// ==================== AT24C02芯片参数 ====================

#define AT24C02_ADDR         0xA0    // AT24C02设备地址
#define AT24C02_PAGE_SIZE    8      // 页大小（字节）
#define AT24C02_CAPACITY     256    // 总容量（字节）
#define AT24C02_WRITE_CYCLE  10     // 写周期时间（毫秒）

// ==================== 函数声明 ====================

/**
 * @brief AT24C02初始化
 */
void AT24C02_Init(void);

/**
 * @brief 向AT24C02指定地址写入一个字节
 * @param addr 字节地址 (0-255)
 * @param dat 要写入的数据
 */
void AT24C02_WriteByte(unsigned char addr, unsigned char dat);

/**
 * @brief 从AT24C02指定地址读取一个字节
 * @param addr 字节地址 (0-255)
 * @return 读取的数据
 */
unsigned char AT24C02_ReadByte(unsigned char addr);

/**
 * @brief 向AT24C02连续写入多个字节（跨页自动处理）
 * @param addr 起始地址
 * @param buf 数据缓冲区
 * @param len 写入长度
 */
void AT24C02_WritePage(unsigned char addr, const unsigned char *buf, unsigned char len);

/**
 * @brief 从AT24C02连续读取多个字节
 * @param addr 起始地址
 * @param buf 数据缓冲区
 * @param len 读取长度
 */
void AT24C02_Read(unsigned char addr, unsigned char *buf, unsigned int len);

// /**
//  * @brief 写入时间数据到AT24C02
//  * @param hour 小时 (0-23)
//  * @param minute 分钟 (0-59)
//  * @param second 秒 (0-59)
//  */
// void AT24C02_WriteTime(unsigned char hour, unsigned char minute, unsigned char second);

// /**
//  * @brief 从AT24C02读取时间数据
//  * @param hour 小时存储地址
//  * @param minute 分钟存储地址
//  * @param second 秒存储地址
//  */
// void AT24C02_ReadTime(unsigned char *hour, unsigned char *minute, unsigned char *second);

// /**
//  * @brief 写入温度数据到AT24C02
//  * @param temp 温度值（放大100倍，如2550表示25.50°C）
//  */
// void AT24C02_WriteTemp(int temp);

// /**
//  * @brief 从AT24C02读取温度数据
//  * @return 温度值（放大100倍）
//  */
// int AT24C02_ReadTemp(void);

// /**
//  * @brief AT24C02写入测试
//  */
// void AT24C02_Test(void);

#endif