/**
 * @file test.c
 * @brief DS1302时钟测试程序
 */

#include "ds1302.h"
#include "lcd1602.h"





void Test(void) {
    DS1302_Time time = {14, 30, 0};
    DS1302_Date date = {26, 5, 17, 7};

    LCD1602_Init();
    DS1302_Init();

    DS1302_WriteTime(time);
    DS1302_WriteDate(date);

    LCD1602_SetCursor(0, 0);
    LCD1602_WriteString("DS1302 Test");

        DS1302_Time time;
        DS1302_ReadTime(&time);

        LCD1602_SetCursor(1, 0);
        LCD1602_WriteNum(time.hour, 2);
        LCD1602_WriteData(':');
        LCD1602_WriteNum(time.minute, 2);
        LCD1602_WriteData(':');
        LCD1602_WriteNum(time.second, 2);   
        LCD1602_WriteNum(time.second, 2);
        LCD1602_WriteData(' ');

        delay_ms(500);
    }
}