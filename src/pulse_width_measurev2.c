// time.c
// 演示延时
// by xcr
// 2026-4-8

#include<reg52.h>        /*包含头文件REG51.H*/

//========数据类型定义===========
#define uchar unsigned char                /*宏定义后方便书写*/
#define uint unsigned int                    /*宏定义后方便书写*/

//===========宏定义===============
#define DISPLAY_DATA_PORT      P1    //数码管数值端口
#define DISPLAY_CTL_PORT         P2    //数码管选择端口

#define ON          0x00   
#define OFF            0xFF

#define KEY_ON  0
#define KEY_OFF 1

//======sfr bit定义==============
//sbit KEY1=P3^0;
//sbit KEY2=P3^1;
sbit Pulse_In = P3^2;

//七段数码管，共阴 码表
//                             0   1    2    3     4    5    6   7   8      9   
uchar code DIGITS_TABLE[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,
//                               A   B     C    D     E     F    -      .
                             0x77,0x7C,0x39, 0x5E, 0x79, 0x71,0x40, 0x80};
                                                                    




long count = 0;
//显示内容缓存
uchar displayContent[8]={0};

                            //======函数声明==================
void init();
void display(long x);
void DelayMS(unsigned x);


void main(){

    Pulse_In = 0;   //放在这是为了避免在INT0中断打开后产生INT0中断
    
    init();                  //初始化：配置定时器0和外部外部中断
    
    //我们要测量一个脉冲（高电平）信号的宽度
    //这里在P3.2引脚(外部中断0引脚）输出一个3ms 脉冲
    //
    //  此输出脉冲波形：__|‾‾~3ms‾‾‾|___
    // 
    Pulse_In = 0;    
    DelayMS(1);
    
    Pulse_In = 1;
    DelayMS(3);
    
    Pulse_In = 0;    
    
    while(1){
        display(count);
    }
    
}                                        

void init()
{
    //设置中断
    TMOD=0x09;    //T0 定时 方式1，GATE=1
    
    TH0=0x00;   //设初值
    TL0=0x00;
      
    
    //配置中断
    ET0 = 1;   //允许定时器1中断
    IT0 = 1;   //外部中断0，下降沿触发
    EX0 = 1;   //允许int0 中断     
    EA  = 1;   //打开中断中开关
    PX0 = 1;   //int0 高优先级
      
    //启动定时器
    TR0 = 1;         //启动定时器0    

}


//定时器0中断，根据我们的配置，
//每产生一次time0中断，表示计数值达到65536，
//定时器将从零开始从新计数
//计数值应该增加65536
void t0_isr(void) interrupt 1   
{    
    count += 65536;    
}


//外部中断0中断产生，
//即P3.2发生下降沿，脉冲结束
void int0_isr(void) interrupt 0
{    
    count += ((TH0<<8)+TL0);        
}



