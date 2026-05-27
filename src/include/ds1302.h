/**
 * @file ds1302.h
 * @brief DS1302实时时钟芯片驱动头文件
 */

#ifndef DS1302_H
#define DS1302_H

#include <reg52.h>
#include "common.h"

// ==================== DS1302引脚定义 ====================

sbit DS1302_CE      = P3^5;
sbit DS1302_SCLK    = P3^6;
sbit DS1302_IO      = P3^4;

// sbit DS1302_WRITE_PROTECT     = P3^7;

// ==================== DS1302地址定义 ====================

#define DS1302_SECOND_ADDR  0x80
#define DS1302_MINUTE_ADDR  0x82
#define DS1302_HOUR_ADDR    0x84
#define DS1302_DAY_ADDR     0x86 
#define DS1302_MONTH_ADDR   0x88
#define DS1302_WEEKDAY_ADDR 0x8A
#define DS1302_YEAR_ADDR    0x8C
#define DS1302_CONTROL_ADDR 0x8E
#define DS1302_TRICKLE_ADDR 0x90


// ==================== DS1302命令定义 ====================

#define DS1302_CLOCK_BURST_READ  0xBF
#define DS1302_CLOCK_BURST_WRITE 0xBE
#define DS1302_RAM_BURST_READ  	 0xFF
#define DS1302_RAM_BURST_WRITE   0xFE


// ==================== 函数声明 ====================
typedef struct {
    unsigned char hour;
    unsigned char minute;
    unsigned char second;
} DS1302_Time;

typedef struct {
    unsigned char year;
    unsigned char month;
    unsigned char day;
    unsigned char weekday;
} DS1302_Date;

void DS1302_Init(void);

void DS1302_WriteByte(unsigned char addr, unsigned char dat);
unsigned char DS1302_ReadByte(unsigned char addr);

void DS1302_WriteTime(DS1302_Time time);
void DS1302_ReadTime(DS1302_Time *time);

void DS1302_WriteDate(DS1302_Date date);
void DS1302_ReadDate(DS1302_Date *date);

void DS1302_WriteReadBurst(unsigned char addr,const unsigned char *buf,unsigned char size);   
void DS1302_ReadBurst(unsigned char addr,unsigned char *buf,unsigned char size);
void DS1302_WriteProtect(unsigned char enable) ; 

#endif