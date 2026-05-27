// time.c
// 演示延时
// by xcr
// 2026-4-8

#include<reg52.h>        /*包含头文件REG51.H*/
#include "common.h"
#include "seg7_mpx8.h"

//===========宏定义===============
#define ON      0x00   
#define OFF        0xFF

#define KEY_ON  0
#define KEY_OFF 1

//======sfr bit定义==============
//sbit KEY1=P3^0;
//sbit KEY2=P3^1;
sbit Pulse_In = P3^2;


unsigned long count = 0;


//======函数声明==================
void init();

void main(){

    init();                  //配置定时器0工作模式
    
    //我们要测量一个脉冲（高电平）信号的宽度
    //这里在P3.2引脚(外部中断0引脚）输出一个3ms 脉冲
    //默认情况下P3.2引脚为高电平，因此输出脉冲如右图：‾‾|__|‾‾3ms‾‾‾|___
    // 即：     先输出0，延时一段时间，在输出1，延时3毫秒，再输出0
//    Pulse_In = 0;    
//    DelayMS(1);
//    
//    Pulse_In = 1;
//    DelayMS(3);
//    
//    Pulse_In = 0;    
    
    while(1){
        
    }
    
}                                        

void init()
{
    // 定时器0配置
    TMOD=0x09;    //T0 定时 方式1，GATE=1    
    TH0=0x00;   //设初值
    TL0=0x00;       
    TR0 = 1;         //启动定时器0

    // 定时器0中断配置
    ET0 = 1;   //允许定时器1中断
    IT0 = 1;   //外部中断0，下降沿触发
    EX0 = 1;   //允许int0 中断
    PX0 = 1;   //int0 高优先级
    EA     = 1;   //打开中断中开关
}


void display()
{
    uchar i= 0;
//    uchar tmp[8] ={0} ;        
                                     
    for(i= 0;i<8;i++){
        SEG_DATA_PORT = 0x00;           // 消隐（关键！）        
        SEG_DIGIT_SEL_PORT     = ~(1<<i);          // 字选择
        SEG_DATA_PORT = DIGITS_TABLE[displayContent[i]];    //输出字    符码
        delay_ms(5);
    }

}

//定时器0中断，根据我们的配置，
//每产生一次time0中断，表示计数值达到65536，
//定时器将从零开始从新计数
//计数值应该增加65536
void t0_isr(void) interrupt 1   
{
    
    count += 65536;
    ;;
}

bit end_flag = 0;

void int0_isr(void) interrupt 0
{
    uchar i=7;
    
  if(end_flag == 1){     
        count += ((TH0<<8)+TL0);                
        //设置显示数据
        while(i--) displayContent[i] = 0;  //清空显示数据
      i=7;
        while(count>0){
            displayContent[i--] = count%10;
            count /=10;
        }
    }
    else{
        count = 0;
    }
    end_flag = !end_flag;
    TH0=TL0=0;
    
}


