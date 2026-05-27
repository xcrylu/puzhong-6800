/**
 * @file ds18b20.h
 * @brief DS18B20温度传感器驱动头文件
 */

#ifndef DS18B20_H
#define DS18B20_H

#include <reg52.h>
#include "common.h"

// ==================== DS18B20引脚定义 ====================

sbit DS18B20_DQ = P3^7;

// ==================== DS18B20命令定义 ====================

#define DS18B20_CMD_SKIP_ROM 0xCC    // 跳过ROM匹配
#define DS18B20_CMD_CONVERT 0x44     // 启动温度转换
#define DS18B20_CMD_READ_SCRATCH 0xBE // 读取暂存器

typedef struct {
    int temp_integer;
    int temp_fractional;
} TempStruct;

// ==================== 函数声明 ====================

//void DS18B20_DelayUs(unsigned char us);
unsigned char DS18B20_Init(void);
void DS18B20_WriteByte(unsigned char dat);
unsigned char DS18B20_ReadByte(void);
void DS18B20_GetTemp(TempStruct *temp);

#endif