/**
 * @file config.h
 * @brief 测温计系统配置文件
 * 
 * 定义系统配置和公共数据结构
 */

#ifndef CONFIG_H
#define CONFIG_H


#include "common.h"
#include "ds18b20.h"

// ==================== 系统配置 ====================

#define FOSC 11059200L
#define DELAY_TIME 100

// ==================== 公共数据结构 ====================

typedef struct {
    unsigned char hour;
    unsigned char minute;
    unsigned char second;
} TimeStruct;



// ==================== 全局变量声明 ====================

extern TimeStruct current_time;
extern TempStruct current_temp;

#endif