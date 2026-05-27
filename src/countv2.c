#include <reg52.h>

#define SEG_PORT P1      //bcd七段数码管接口
#define ON   0x00
#define OFF  0xff

unsigned long count =0;

void time_init();
void display();

int main()
{    
      time_init();     //调用time初始化函数
    while(1){
        display();     //调用显示函数，显示部分计数值
    }
}

void time_init()
{
    TMOD = 0x05; //设置time1，计数，方式1   
    TH0 = TL0 = 0;//清除定时器初值
    
    ET0 = 1;    //打开time0中断
    EA  = 1;     //允许总中断
    
    TR0 = 1;    //启动time0
}

//由于我们的显示位数有限，
// 这里只显示计数器的低8位。
void display(){
    SEG_PORT = TL0;
}

//读取计数值，计数值包括两部分，
//   一部分为：中断了多少次，就有多少个65536
//    一部分:  保存在计数器中当前的计数值
long getCount(void)
{
    return count + TH0<<8 + TL0;
}

//定时器time0中断处理函数
//当定时器计满溢出时，产生中断，计数值增加+ 65536
void time0_isr() interrupt 1
{
    count += 65536;     
}
