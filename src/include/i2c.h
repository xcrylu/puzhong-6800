/**
 * @file i2c.h
 * @brief I2C总线驱动头文件
 */

#ifndef _I2C_H_
#define _I2C_H_

#include "common.h"

// ==================== I2C引脚定义 ====================

sbit I2C_SDA    = P2^0;
sbit I2C_SCL    = P2^1;

// ==================== I2C配置参数 ====================

// #define I2C_ADDR      0xA0      // 设备地址

// #define I2C_TIME_ADDR 0x00 // 时间数据存储起始地址
// #define I2C_TEMP_ADDR 0x10 // 温度数据存储起始地址

// #define I2C_MAX_ADDR 0x80 // 1kbit/128byte

// ==================== 函数声明 ====================
/**
 * @brief I2C总线延时函数
 * 
 * 延时时间根据具体I2C时钟频率调整
 */
// void I2C_Delay(void);

void I2C_Start(void);
void I2C_Stop(void);
unsigned char I2C_Ack(void);
void I2C_SendAck(unsigned char ack);


void I2C_Write8(unsigned char dat);
unsigned char I2C_Read8(void);

/**
 * @brief 从EEPROM读取数据
 * @param slave     I2C设备地址
 * @param start_addr 起始地址
 * @param buf     存储读取数据的缓冲区
 * @param count   要读取的数据大小
 * @return n 读取的数据大小，-1表示失败
 */
int I2C_Read(unsigned char slave, unsigned char start_addr, char *buf, int count);

/**
 * @brief 写入数据到数据到EEPROM
 * @param slave     I2C设备地址
 * @param start_addr 起始地址
 * @param buf     写入得数据缓冲区
 * @param count   写入数据大小
 * @return n 写入的数据大小，-1表示失败
 */
int I2C_Write(unsigned char slave, unsigned char start_addr, const char *buf, int count);  

  

#endif