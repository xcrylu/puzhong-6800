/**
 * @file ds18b20.c
 * @brief DS18B20温度传感器驱动
 * 
 * 严格按照DS18B20数据手册时序要求实现
 * 时钟频率: 11.0592MHz, 机器周期≈1.085μs
 */

#include "ds18b20.h"
#include "lcd1602.h"
#include <intrins.h>




// 11.0592MHz
// us == 1 ，~20us
// us == 2   ~27us
// us == 3   ~35us
//       4   ~42
// 每增加1   增加约7us
void DS18B20_DelayUs(unsigned char us)
{
  while(us--);
}

// 初始化 成功返回0 失败1
unsigned char DS18B20_Reset(void)
{
    unsigned char ack;
    DS18B20_DQ = 1;
    _nop_();  // 确保总线稳定
    
    //拉低dq大于480us，发复位信号
    DS18B20_DQ = 0;
    DS18B20_DelayUs(70);  // 拉低>=480us  14us+70*7us = 520us
    
    //读ds1820应答
    DS18B20_DQ = 1;
    DS18B20_DelayUs(6);   // 等待应答窗口15~60us ,60us
    ack = DS18B20_DQ;        //读应答脉冲宽度60~240us
    DS18B20_DelayUs(60); // 应当周期大于480us，这里约440us 
		
    return ack;
}

// 写字节 低位先发
void DS18B20_WriteByte(unsigned char dat)
{
    unsigned char i;
	DS18B20_DQ = 1;
	_nop_();
    //低位在前
    for(i=0;i<8;i++)
    {   
        DS18B20_DQ = 0;           // ① 拉低总线，启动写时隙
        _nop_(); _nop_();         // ② 保持低电平 ≥1μs
        
        if(dat & 0x01)
            DS18B20_DQ = 1;       // ③ 写1: 立即释放总线为高
        else
            DS18B20_DQ = 0;       // ③ 写0: 继续保持低电平
        
        DS18B20_DelayUs(6);       // ④ 保持整个时隙 约60us
                                   //    DS18B20在15~60μs窗口采样
        
        DS18B20_DQ = 1;           // ⑤ 释放总线
        //_nop_();      // ⑥ 等待恢复，保证Tx时间大于60μs，DS18B20采样窗口结束，准备下一位
				                            //写0，Tx低电平时间小于120us
        
        dat >>= 1;                // ⑦ 准备下一位(LSB先发)
    }
}

// 读字节 低位先收
// 1~15us期间读取数据线电平，15~60us期间保持低电平，60~120us期间保持高电平
unsigned char DS18B20_ReadByte(void)
{
    unsigned char i,dat=0;
	  DS18B20_DQ = 1;
    for(i=0;i<8;i++)
    {
        dat >>= 1;
        DS18B20_DQ = 0;
        _nop_();
        //_nop_();
        DS18B20_DQ = 1;
        _nop_();
        _nop_();
        if(DS18B20_DQ)  //请保证发起读数据位后15us内读取
            dat |= 0x80;
        DS18B20_DelayUs(6); //保证DS18B20 bit 时间大于60μs ~60us
    }
    return dat;
}

void DS18B20_GetTemp(TempStruct *temp)
{
    unsigned char tl,th;
    int tempval;

    if(DS18B20_Reset()) { /* no device present */
       temp->temp_integer = 0;
       temp->temp_fractional = 0;
       return;
    }
    DS18B20_WriteByte(DS18B20_CMD_SKIP_ROM);    // 跳过ROM
    DS18B20_WriteByte(DS18B20_CMD_CONVERT);     // 启动转换
    

    // 等待转换完成
	delay_ms(750);

    if(DS18B20_Reset()) { /* no device present */
        temp->temp_integer = 0;
        temp->temp_fractional = 0;
        return;
    }
    DS18B20_WriteByte(DS18B20_CMD_SKIP_ROM);	
    DS18B20_WriteByte(DS18B20_CMD_READ_SCRATCH);    // 读取暂存器
	
    tl = DS18B20_ReadByte();
    th = DS18B20_ReadByte();
    
    tempval = (th<<8)|tl;
    if(tempval & 0x8000)
    {
        tempval = (~tempval)+1;
        temp->temp_integer = -(tempval/16);
    }
    else
    {
        temp->temp_integer = tempval/16;
    }
    temp->temp_fractional = (tempval&0x0F)*625/100;
		//return temp->temp_integer
}


// #include "uart.h"
// #include "stdio.h"  
// void Test(void){
//     TempStruct temp;
//     char buf[16];
//     uart_init();
//     while(1)
//     {
//         DS18B20_GetTemp(&temp);
//         sprintf(buf, "%2d.%2d C\r\n\0", temp.temp_integer, temp.temp_fractional);
//         uart_send_str(buf);  
        
//     }

// }

// void main(void)
// {
//     Test();
// }
