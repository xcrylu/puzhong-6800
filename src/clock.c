// clock.c
// 演示时钟程序
// by xcr
// 2026-4-8

#include<reg52.h>						/*包含头文件REG51.H*/
#include <type_traits>
#include "common.h"
#include "seg7_mpx8.h"


void Timer0_Init();
void display();



struct _time{
	uchar secend;
	uchar minute;
	uchar hour;
}time={00,34,12};				

void main(){
	Timer0_Init();
	while(1){
		display();
	}
}												

//初始化定时器中断
#define MS50CNT 50000
void Timer0_Init()
{
	//设置定时器工作方式
	TMOD 	= 0x01; //定时器0 定时 工作方式1
	//设置定时器初值，设置每50ms发生一次定时器溢出中断
	TH0 	= (65535-MS50CNT)/255;  
	TL0 	= (65535-MS50CNT)%255;
	
	//配置中断	
	ET0 = 1;  //打开定时器0 中断
	EA  = 1;  //打开总中断
	
	//启动定时器0	
  	TR0	= 1;
}

void display()
{
	uchar i= 0;

	uchar tmp[8] ={0} ;
	tmp[2] = tmp[5]=16;
	tmp[0] = time.hour/10;
	tmp[1] = time.hour%10; //时
	tmp[3] = time.minute/10;
	tmp[4] = time.minute%10;  //分
	tmp[6] = time.secend/10;
  	tmp[7] = time.secend%10;  //秒		
									 
	SEG7_mpx8_display(tmp);	
}
														
//延时函数
void DelayMS(u16 x)					//延时函数
{
	u8 i;								//定义无符号字符型变量i
	while(x--)							//双重for循环语句实现软件延时 
		for(i=0;i<110;i++);			    
}

uchar cnt50ms = 0;

void t0_isr(void) interrupt 1
{
	//50ms
	//重置定时器初值
	TH0 	= (65535-MS50CNT)/255;  
	TL0 	= (65535-MS50CNT)%255;
	
	if(++cnt50ms ==20){  //1m
		cnt50ms = 0;	 
		if(++time.secend == 60){ //1分钟
			time.secend = 0;
			if(++time.minute == 60){ //1小时
				time.minute =0;
				if(++time.hour==24){  //1天
					time.hour=0;
				}
			}
		}	
	}
}