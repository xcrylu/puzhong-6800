#include <reg52.h>
#include "tm1637.h"

unsigned int ms = 0;    // 毫秒
unsigned char sec = 0;   // 秒
unsigned char min = 0;   // 分

bit timeUpdate = 1;      // 更新标志
bit displayUpdate = 1;

void Timer0_Init(void)  // 1ms@12MHz
{
    TMOD |= 0x01;
    TH0 = (65535 - 1000) / 255;// 0xFC;
    TL0 = (65535 - 1000) % 10;//0x67;
    ET0 = 1;
    //TR0 = 1;
    EA  = 1;
}


void TM1637_DisplayStopwatch(void)
{
    unsigned char buf[6] = {18,18,1,18,2,18};
        char i;


    // 显示格式：00.00.00
    // 第5位：分十位
    // 第4位：分个位
    // 第3位：秒十位
    // 第2位：秒个位
    // 第1位：毫秒十位
    // 第0位：毫秒个位

  
    // 格式 00.00.00
    buf[5] = min / 10;
    buf[4] = min % 10;
    buf[3] = sec / 10;
    buf[2] = sec % 10;
    buf[1] = (ms / 100);
    buf[0] = (ms / 10) % 10;

     //写数据到显示寄存器
    TM1637_Start();
    TM1637_Write(0x40);
    TM1637_Stop();

        //发送初始地址
    TM1637_Start();
    TM1637_Write(0xC0);
    
    // 输出到6位数码管
    for(i=5; i>=0; i--)
    {                    
        unsigned char seg = SEG_TABLE[buf[i]];
        if(i == 4 || i == 2) seg |= 0x80; // 两个小数点
        TM1637_Write(seg);            
    }

    TM1637_Stop();
}
bit start = 0;

void main(void)
{
     TM1637_Init();
     Timer0_Init();  // 初始化定时器

    while(1)
    {
            char key = TM1637_ReadKey();
            switch (key)
            {
                case 1: //启动
                            if(start == 0 ){
                                start = 1;
                                TR0 = 1;
                                displayUpdate = 1;
                            }            
                    
                    break;
                 
              case 2: //停止
                    start = 0;
                    TR0 = 0;
                  TM1637_DisplayStopwatch();
                    break;
                case 3: //清零
                    TR0=0;
                  min = 0;
                  sec = 0;
                  ms  = 0;
                  TH0 = (65535 - 1000) / 255;// 0xFC;
                    TL0 = (65535 - 1000) % 10;//0x67;
                  TM1637_DisplayStopwatch();
                 // timeUpdate = 1;
                  break;
            }
            
            
            if(displayUpdate)  // 1ms刷新一次
        {
         // timeUpdate = 0;
          TM1637_DisplayStopwatch();                
        }
    }
//    unsigned char key;
//    TM1637_Init();

//    while(1)
//    {
//        key = TM1637_ReadKey();
//        if(key != 0)
//        {
//           TM1637_DisplayInt(key);  // 显示键值1~16
//        }
//            TM1637_DisplayFloat(0.12);
//    }
}

void Timer0_ISR(void) interrupt 1
{
//    TH0 = 0xFC;
//    TL0 = 0x67;

      TH0 = (65535 - 1000) / 255;// 0xFC;
    TL0 = (65535 - 1000) % 10;//0x67;
    ms++;
    
      //if(ms%100 == 0) {timeUpdate=1;}
        
    if(ms >= 1000)
    {              
        ms = 0;
        sec++;
        if(sec >= 60)
        {
            sec = 0;
            min++;
            if(min >= 100) min = 0;
        }
    }
        
    //timeUpdate = 1;
        
}