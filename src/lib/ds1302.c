/**
 * @file ds1302.c
 * @brief DS1302实时时钟芯片驱动
 *
 * 实现DS1302的初始化、时间读写、日期读写等功能
 */


#include "ds1302.h"
#include <intrins.h>
#include "uart.h"

// void DS1302_Init(void) {
//     DS1302_SCLK = 0;
//     DS1302_CE = 1;

// }

void DS1302_Write8(unsigned char dat) {
    unsigned char i ;
    for(i = 0; i < 8; i++) {
        DS1302_SCLK = 0;
       // delay_us(1);
			  _nop_();
        DS1302_IO = (dat & 0x01);       
        //delay_us(1);

        DS1302_SCLK = 1;         //clk上升沿输入数据
        //delay_us(1);
        _nop_();
        DS1302_SCLK = 0;
         
        dat >>= 1;
    };
}

unsigned char DS1302_Read8(void) {
    unsigned char dat = 0;
    unsigned char i;
    //DS1302_SCLK = 0;
    for(i = 0; i < 8; i++) {
        DS1302_SCLK = 1;    //sclk下降沿输出数据
       // delay_us(1);
        DS1302_SCLK = 0;
        if(DS1302_IO) {
            dat |= 0x80;
        }       
        DS1302_SCLK = 1;
        dat >>= 1;
      
    }

    return dat;
}
//Driving the CE input high initiates all data transfers. 
//The CE input serves two functions. First, CE turns on the 
// control logic that allows access to the shift register for the address/command sequence. Second, the CE signal 
// provides a method of terminating either single-byte or multiple-byte CE data transfer.
//ds1302写入字节函数,clk上升沿输入数据
// Data is input starting with bit 0.
void DS1302_WriteByte(unsigned char addr, unsigned char dat) {
    
    DS1302_SCLK = 0;
    DS1302_CE = 1;        // 使能芯片
 
    DS1302_Write8(addr);
    DS1302_Write8(dat);  
   
    DS1302_CE = 0;       // 禁用芯片
}

//ds1302读取字节函数,clk输出数据
unsigned char DS1302_ReadByte(unsigned char addr) {
      
    
    unsigned char dat = 0;
	  //DS1302_SCLK = 0;   
	
	  DS1302_CE = 1;        // 使能芯片
    //DS1302_WriteProtect(0);
    DS1302_Write8(addr);
    dat = DS1302_Read8();
   // DS1302_WriteProtect(1);
    DS1302_CE = 0;       // 禁用芯片

    return dat;
}

void DS1302_WriteTime(DS1302_Time time) {

   // DS1302_WriteProtect(0);
    DS1302_WriteByte(DS1302_SECOND_ADDR, (time.second / 10 << 4) | (time.second % 10));
    DS1302_WriteByte(DS1302_MINUTE_ADDR, (time.minute / 10 << 4) | (time.minute % 10));
    DS1302_WriteByte(DS1302_HOUR_ADDR, (time.hour / 10 << 4) | (time.hour % 10));
   // DS1302_WriteProtect(1);
}

void DS1302_ReadTime(DS1302_Time *time) {
    unsigned char sec, min, hr;

    sec = DS1302_ReadByte(DS1302_SECOND_ADDR);
    min = DS1302_ReadByte(DS1302_MINUTE_ADDR);
    hr  = DS1302_ReadByte(DS1302_HOUR_ADDR);
    //uart_send_byte(sec);
    //uart_send_byte(min);
    //uart_send_byte(hr);

    time->second = (sec >> 4) * 10 + (sec & 0x0F);
    time->minute = (min >> 4) * 10 + (min & 0x0F);
    time->hour = (hr >> 4) * 10 + (hr & 0x0F);
}

void DS1302_WriteDate(DS1302_Date date) {
    DS1302_WriteProtect(0);
    DS1302_WriteByte(DS1302_YEAR_ADDR, (date.year / 10 << 4) | (date.year % 10));
    DS1302_WriteByte(DS1302_MONTH_ADDR, (date.month / 10 << 4) | (date.month % 10));
    DS1302_WriteByte(DS1302_DAY_ADDR, (date.day / 10 << 4) | (date.day % 10));
    DS1302_WriteByte(DS1302_WEEKDAY_ADDR, date.weekday);
    DS1302_WriteProtect(1);
}

void DS1302_ReadDate(DS1302_Date *date) {
    unsigned char yr, mon, d;

    yr = DS1302_ReadByte(DS1302_YEAR_ADDR);
    mon = DS1302_ReadByte(DS1302_MONTH_ADDR);
    d = DS1302_ReadByte(DS1302_DAY_ADDR);
    date->weekday = DS1302_ReadByte(DS1302_WEEKDAY_ADDR);

    date->year = (yr >> 4) * 10 + (yr & 0x0F);
    date->month = (mon >> 4) * 10 + (mon & 0x0F);
    date->day = (d >> 4) * 10 + (d & 0x0F);
}

void DS1302_WriteClockBurst(const unsigned char *buf) {
    unsigned char i;

    DS1302_CE = 1;        // 使能芯片
   
   // DS1302_WriteProtect(0);

    DS1302_Write8(DS1302_CLOCK_BURST_WRITE);

    for(i = 0; i < 8; i++) {
        DS1302_Write8(buf[i]);
    }
   
   // DS1302_WriteProtect(1);
    
    DS1302_CE = 0;       // 禁用芯片
}

void DS1302_ReadClockBurst(unsigned char *buf) {
    unsigned char i;
 
    DS1302_CE = 1;        // 使能芯片

  //  DS1302_WriteProtect(0);

    DS1302_Write8(DS1302_CLOCK_BURST_READ);

    for(i = 0; i < 8; i++) {
        buf[i] = DS1302_Read8();
    }
   
   // DS1302_WriteProtect(1);

    DS1302_CE = 0;       // 禁用芯片
}


void DS1302_WriteBurst(unsigned char addr,unsigned char *buf,unsigned char size) {
    unsigned char i;

    DS1302_CE = 1;        // 使能芯片

    DS1302_WriteProtect(0);

    DS1302_Write8(addr);    

    for(i = 0; i < size; i++) {
        DS1302_Write8(buf[i]);
    }   
    DS1302_WriteProtect(1);    
    DS1302_CE = 0;       // 禁用芯片
}

void DS1302_ReadBurst(unsigned char addr,unsigned char *buf,unsigned char size) {
    unsigned char i;

    DS1302_CE = 1;        // 使能芯片

    DS1302_Write8(addr);

    for(i = 0; i < size; i++) {
        buf[i] = DS1302_Read8();
    }
   
    DS1302_CE = 0;       // 禁用芯片
}

void DS1302_WriteProtect(unsigned char enable) {
    DS1302_CE = 1;        // 使能芯片
    if(enable) {
        DS1302_Write8(DS1302_CONTROL_ADDR);
        DS1302_Write8(0x80);
    } else {
        DS1302_Write8(DS1302_CONTROL_ADDR);
        DS1302_Write8(0x00);
    }   
    DS1302_CE = 0;       // 禁用芯片
}


// 测试DS1302
#include "uart.h"
#include "stdio.h"   


void Test()
{
    // DS1302_Init();
    DS1302_Time time;
    DS1302_Date date;

	// uchar buf[32];   

    // DS1302_Time init_time = {12, 30, 0};
    // DS1302_Date init_date = {23, 12, 25, 5};

    //uart_init();

    //DS1302_WriteBurst(DS1302_RAM_BURST_WRITE, "init_time", 10);
    
//    DS1302_WriteTime(init_time);
//    DS1302_WriteDate(init_date);   

    // DS1302_ReadTime(&time);
    // DS1302_ReadDate(&date);
    while(1){
        //DS1302_ReadTime(&time);
        //DS1302_ReadDate(&date);     
        //sprintf(buf, "%02d:%02d:%02d %04d-%02d-%02d %02d\r\n\0", time.hour, time.minute, time.second, date.year, date.month, date.day, date.weekday);
        //uart_send_str("\n"); 
        DS1302_WriteByte(DS1302_HOUR_ADDR, 0x55);

        delay_ms(1);
    }
    
}
void main(void)
{
    Test();
}