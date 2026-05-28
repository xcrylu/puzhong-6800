/**
 * @file ds1302.c
 * @brief DS1302实时时钟芯片驱动
 *
 * 实现DS1302的初始化、时间读写、日期读写等功能
 */

#include "ds1302.h"
// #include <intrins.h>
// #include "uart.h"

void DS1302_Init(void)
{
    DS1302_CE = 0;   // disable chip by default
    //DS1302_SCLK = 0; // clock idle low
    DS1302_IO = 1;   // release IO (input/high) so device can drive it
    delay_us(1);
}
void DS1302_Write8(unsigned char dat)
{
    unsigned char i;

    for (i = 0; i < 8; i++)
    {
        DS1302_SCLK = 0;
        delay_us(1);
        DS1302_IO = (dat & 0x01);
        delay_us(1);
        DS1302_SCLK = 1; // clk上升沿输入数据
        delay_us(1);
        dat >>= 1;
    };
}

unsigned char DS1302_Read8(void)
{
    unsigned char dat = 0;
    unsigned char i;
    // DS1302_SCLK = 0;
    for (i = 0; i < 8; i++)
    {
        dat >>= 1;
        DS1302_SCLK = 1; // sclk下降沿输出数据
        delay_us(1);
        DS1302_SCLK = 0; // sclk下降沿输出数据
        if (DS1302_IO)
        {
            dat |= 0x80;
        }
        delay_us(1);
    }

    return dat;
}

void DS1302_WriteByte(unsigned char addr, unsigned char dat)
{
    
    DS1302_CE = 1; // 设置IO为输出
    delay_us(1);
    // 写命令/地址
  
    DS1302_Write8(addr);
    // 写数据

    DS1302_Write8(dat);
    DS1302_CE = 0; // 结束传输
}

// ds1302读取字节函数,clk输出数据
unsigned char DS1302_ReadByte(unsigned char addr)
{

    unsigned char dat = 0;
    DS1302_CE = 1; // 设置IO为输入
    delay_us(1);
    // 写命令/地址
    DS1302_Write8(addr);
    
    // 读数据
    // 设置IO口位输入模式
    DS1302_IO = 1;
    dat = DS1302_Read8();
    DS1302_CE = 0; // 结束传输

    return dat;
}

void DS1302_WriteTime(DS1302_Time time)
{

    // DS1302_WriteProtect(0);
    DS1302_WriteByte(DS1302_SECOND_ADDR, (time.second / 10 << 4) | (time.second % 10));
    DS1302_WriteByte(DS1302_MINUTE_ADDR, (time.minute / 10 << 4) | (time.minute % 10));
    DS1302_WriteByte(DS1302_HOUR_ADDR, (time.hour / 10 << 4) | (time.hour % 10));
    // DS1302_WriteProtect(1);
}

void DS1302_ReadTime(DS1302_Time *time)
{
    unsigned char sec, min, hr;

    sec = DS1302_ReadByte(DS1302_SECOND_ADDR | 0x01);
    min = DS1302_ReadByte(DS1302_MINUTE_ADDR | 0x01);
    hr = DS1302_ReadByte(DS1302_HOUR_ADDR | 0x01);

    time->second = (sec >> 4) * 10 + (sec & 0x0F);
    time->minute = (min >> 4) * 10 + (min & 0x0F);
    time->hour = (hr >> 4) * 10 + (hr & 0x0F);
}

void DS1302_WriteDate(DS1302_Date date)
{

    DS1302_WriteByte(DS1302_YEAR_ADDR, (date.year / 10 << 4) | (date.year % 10));
    DS1302_WriteByte(DS1302_MONTH_ADDR, (date.month / 10 << 4) | (date.month % 10));
    DS1302_WriteByte(DS1302_DAY_ADDR, (date.day / 10 << 4) | (date.day % 10));
    DS1302_WriteByte(DS1302_WEEKDAY_ADDR, date.weekday);
}

void DS1302_ReadDate(DS1302_Date *date)
{
    unsigned char yr, mon, d;

    yr = DS1302_ReadByte(DS1302_YEAR_ADDR | 0x01);
    mon = DS1302_ReadByte(DS1302_MONTH_ADDR | 0x01);
    d = DS1302_ReadByte(DS1302_DAY_ADDR | 0x01);
    date->weekday = DS1302_ReadByte(DS1302_WEEKDAY_ADDR | 0x01);

    date->year = (yr >> 4) * 10 + (yr & 0x0F);
    date->month = (mon >> 4) * 10 + (mon & 0x0F);
    date->day = (d >> 4) * 10 + (d & 0x0F);
}

void DS1302_WriteBurst(unsigned char addr, unsigned char *buf, unsigned char size)
{
    unsigned char i;

    DS1302_CE = 1; // 使能芯片
    delay_us(10);
    
    DS1302_Write8(addr);  

    for (i = 0; i < size; i++)
    {
        DS1302_Write8(buf[i]);
    }

    DS1302_CE = 0; // 禁用芯片
}

void DS1302_ReadBurst(unsigned char addr, unsigned char *buf, unsigned char size)
{
    unsigned char i;

    DS1302_CE = 1; // 使能芯片
    delay_us(1);

    DS1302_Write8(addr);
        DS1302_IO = 1;
    for (i = 0; i < size; i++)
    {
        buf[i] = DS1302_Read8();
    }

    DS1302_CE = 0; // 禁用芯片
}

void DS1302_WriteProtect(unsigned char enable)
{

    DS1302_CE = 1; // 使能芯片
    delay_us(1);

    if (enable)
    {
        DS1302_Write8(DS1302_CONTROL_ADDR);
        DS1302_Write8(0x80);
    }
    else
    {
        DS1302_Write8(DS1302_CONTROL_ADDR);
        DS1302_Write8(0x00);
    }

    DS1302_CE = 0; // 禁用芯片
}


// // 测试DS1302
// #include "uart.h"
// #include "stdio.h"

// void Test()
// {
    
//     DS1302_Time time;
//     DS1302_Date date;

//     uchar buf[32];

//     DS1302_Time init_time = {12, 30, 0};
//     DS1302_Date init_date = {23, 12, 25, 5};

//     uart_init();

//     delay_ms(100);
//     DS1302_WriteBurst(DS1302_RAM_BURST_WRITE, "abcedfghigklmnopqrstuvwxyz", 26);

//     DS1302_WriteTime(init_time);
//     DS1302_WriteDate(init_date);

//     DS1302_ReadTime(&time);
//     DS1302_ReadDate(&date);		

//     DS1302_WriteBurst(DS1302_RAM_BURST_WRITE, "abcdefghij1234567890", 20);
//     DS1302_ReadBurst(DS1302_RAM_BURST_READ, buf, 20);

//     while (1)
//     {
//         DS1302_ReadTime(&time);
//         DS1302_ReadDate(&date);
        
//         sprintf(buf, "%02U:%02U:%02U %02U-%02U-%02U %U **", (unsigned int )time.hour, (unsigned int )time.minute, (unsigned int )time.second,\
//                                  (unsigned int )date.year, (unsigned int )date.month, ((unsigned int )date.day) & 0x0F, (unsigned int )date.weekday);
//         uart_send_str(buf);	
            
//     }
// }
// void main(void)
// {
//     Test();
// }