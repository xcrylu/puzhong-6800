/**
 * @file spi.h
 * @brief SPI总线驱动头文件
 *
 * 定义通用SPI通信接口
 */

#ifndef _SPI_H_
#define _SPI_H_

#include <reg52.h>
#include "common.h"

// ==================== SPI引脚定义 ====================

sbit SPI_SS   = P1^0;   // 从机选择引脚
sbit SPI_SCLK = P1^1;   // 时钟信号引脚
sbit SPI_MOSI = P1^2;   // 主出从入数据线引脚
sbit SPI_MISO = P1^3;   // 主入从出数据线引脚  

// ==================== SPI模式定义 ====================

#define SPI_MODE_0  0   // CPOL=0, CPHA=0: 空闲低电平，上升沿采样
#define SPI_MODE_1  1   // CPOL=0, CPHA=1: 空闲低电平，下降沿采样
#define SPI_MODE_2  2   // CPOL=1, CPHA=0: 空闲高电平，下降沿采样
#define SPI_MODE_3  3   // CPOL=1, CPHA=1: 空闲高电平，上升沿采样

// ==================== SPI位序定义 ====================

#define SPI_MSB_FIRST  0  // 高位在前
#define SPI_LSB_FIRST  1  // 低位在前

// ==================== 函数声明 ====================

/**
 * @brief SPI微秒级延时
 * @param x 延时时间
 */
// void SPI_DelayUs(unsigned char x);

/**
 * @brief SPI初始化
 * @param mode SPI模式 (0-3)
 * @param lsb_first 是否低位在前 (0: MSB, 1: LSB)
 */
void SPI_Init(unsigned char mode, unsigned char lsb_first);

/**
 * @brief SPI写一个字节
 * @param dat 要发送的数据
 * @return 从机返回的数据
 */
unsigned char SPI_WriteByte(unsigned char dat);

/**
 * @brief SPI读取一个字节
 * @return 读取的数据
 */
unsigned char SPI_ReadByte(void);

/**
 * @brief SPI同时读写一个字节
 * @param dat 要发送的数据
 * @return 读取到的数据
 */
unsigned char SPI_Transfer(unsigned char dat);

#endif