/**
 * @file main.c
 * @brief 测温计系统主程序
 */

#include "tmp-time-at2402.h"
#include "ds18b20.h"
#include "spi.h"
#include "i2c.h"
#include "isd4004.h"
#include "lcd1602.h"

/**
 * @brief 当前时间结构体变量
 */
/**
 * @brief 当前时间全局变量
 */
TimeStruct current_time = {12, 0, 0};
/**
 * @brief 当前温度全局变量
 */
/**
 * @brief 当前温度结构体变量
 */
TempStruct current_temp = {0, 0};
unsigned char display_update_flag = 1;
unsigned char storage_flag = 0;
unsigned char voice_flag = 0;

void Timer0_Init(void);

void AT24C02_WriteTime(TimeStruct *time);
void AT24C02_WriteTemp(TempStruct *temp);
void AT24C02_ReadTime(TimeStruct *time);
void AT24C02_ReadTemp(TempStruct *temp);

void LCD1602_DisplayTemp(TempStruct *temp);
void LCD1602_DisplayTime(TimeStruct *time);




void main(void) {
    unsigned char temp_hour, temp_min, temp_sec;
    
    Timer0_Init();
    LCD1602_Init();
    AT24C02_ReadTime(&current_time);
    
    //如果读取存储出错，使用默认时间：12时0分0秒
    if(current_time.hour >= 24 || current_time.minute >= 60  || current_time.second >= 60) {                    
        current_time.hour = 12;
        current_time.minute = 0;
        current_time.second = 0;
    }
    
    AT24C02_ReadTemp(&current_temp);
    
    while(1) {              
        if(display_update_flag) {
            display_update_flag = 0;
            
            DS18B20_GetTemp(&current_temp);
            
            LCD1602_DisplayTime(&current_time);
            LCD1602_DisplayTemp(&current_temp);
        }
        
        if(storage_flag) {
            storage_flag = 0;
            AT24C02_WriteTime(&current_time);
            AT24C02_WriteTemp(&current_temp);
        }
    }
}




void Timer0_Init(void) {
    TMOD |= 0x01;
    TH0 = (65536 - 50000) / 256;
    TL0 = (65536 - 50000) % 256;
    ET0 = 1;
    EA = 1;
    TR0 = 1;
}

void Timer0_Interrupt() interrupt 1 {
    static unsigned char cnt = 0;
    
    TH0 = (65536 - 50000) / 256;
    TL0 = (65536 - 50000) % 256;
    
    cnt++;
    if(cnt >= 20) {
        cnt = 0;
        
        current_time.second++;
        if(current_time.second >= 60) {
            current_time.second = 0;
            current_time.minute++;
            
            if(current_time.minute >= 60) {
                current_time.minute = 0;
                current_time.hour++;
                
                if(current_time.hour >= 24) {
                    current_time.hour = 0;
                }
            }
        }
        
        display_update_flag = 1;
        
        if(current_time.second % 30 == 0) {
            storage_flag = 1;
        }
        
        if(current_time.second % 60 == 0) {
            voice_flag = 1;
        }
    }
}

/**
 * @brief 显示温度
 * @param temp 温度结构体指针
 */
void LCD1602_DisplayTemp(TempStruct *temp) {
    LCD1602_SetCursor(1, 0);
    LCD1602_WriteString("Temp: ");
    
    // 显示符号和整数部分
    if(temp->temp_integer < 0) {
        LCD1602_WriteData('-');
        LCD1602_WriteNum(-temp->temp_integer, 2);
    } else {
        LCD1602_WriteData('+');
        LCD1602_WriteNum(temp->temp_integer, 2);
    }
    
    // 显示小数部分
    LCD1602_WriteData('.');
    LCD1602_WriteNum(temp->temp_decimal, 2);
        LCD1602_WriteData(0xdf);  //摄氏度符号圈圈
    LCD1602_WriteString("C");
}


/**
 * @brief 显示时间
 * @param time 时间结构体指针
 */
void LCD1602_DisplayTime(TimeStruct *time) {
    LCD1602_SetCursor(0, 0);
    LCD1602_WriteString("Time: ");
    
    LCD1602_WriteNum(time->hour, 2);
    LCD1602_WriteData(':');
    
    LCD1602_WriteNum(time->minute, 2);
    LCD1602_WriteData(':');
    
    LCD1602_WriteNum(time->second, 2);
}

/**
 * @brief 写入时间数据到AT24C02
 * @param hour 小时 (0-23)
 * @param minute 分钟 (0-59)
 * @param second 秒 (0-59)
 */
void AT24C02_WriteTime(TimeStruct *time) {
   AT24C02_WriteByte(ADDR_TIME, time->hour);
   AT24C02_WriteByte(ADDR_TIME + 1, time->minute);
   AT24C02_WriteByte(ADDR_TIME + 2, time->second);
}

/**
 * @brief 从AT24C02读取时间数据
 * @param hour 小时存储地址
 * @param minute 分钟存储地址
 * @param second 秒存储地址
 */
void AT24C02_ReadTime(TimeStruct *time) {
   time->hour = AT24C02_ReadByte(ADDR_TIME);
   time->minute = AT24C02_ReadByte(ADDR_TIME + 1);
   time->second = AT24C02_ReadByte(ADDR_TIME + 2);
}

/**
 * @brief 写入温度数据到AT24C02
 * @param temp 温度值（放大100倍，如2550表示25.50°C）
 */
void AT24C02_WriteTemp(TempStruct *temp) {
    unsigned char *p = (unsigned char *)&temp;

    AT24C02_WriteByte(ADDR_TEMP, p[0]);
    AT24C02_WriteByte(ADDR_TEMP + 1, p[1]);
}

/**
 * @brief 从AT24C02读取温度数据
 * @param temp 温度结构体指针
 */
void AT24C02_ReadTemp(TempStruct *temp) {
    unsigned char buf[2];
    int temp;

    buf[0] = AT24C02_ReadByte(ADDR_TEMP);
    buf[1] = AT24C02_ReadByte(ADDR_TEMP + 1);

    temp->temp_integer = (int)buf[0] | ((int)buf[1] << 8);

}


