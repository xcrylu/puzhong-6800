#include "Led_display.h"
#include "keyScan.h"
#include "common.h"
#include "ds1302.h"
#include "LCD1602.H"



void test_keyScan()
{

    unsigned char key;
    static unsigned  char displaykey=0;
    key = KeyScan_v2();
   if(key != 0xFF)
   {
     displaykey = key;
         // 这里可以加处理：串口发送、数码管显示等
            // 例: if(key==1) ...
   }
    SEG_8DIG_display_number(displaykey);  

}

void test_74h595(){
    
    unsigned char i =0 ;
    for(i =0 ;i<8;i++){        
        write_74h595(1<<i);
        delay_ms(1000);
    }
}



void test_segmentled_8D_dispay(void)
{    
    unsigned char buf[8] ={0,1,2,3,4,5,6,7};
    SEG_8DIG_display(buf);
}


unsigned char i = 0;
unsigned int x = 0;
void test_matrix_8x8_display(void)
{
  //x++;
    matrix_8x8_display(DigitPattern[x++/100%10]);
    //matrix_8x8_display(DigitPattern[1]);

}

extern void delay_ms(uint ms);

void ds1302_test(void) {
    DS1302_Time time;
    DS1302_Date date;

    LCD1602_Init();
    DS1302_Init();

    time.hour = 14;
    time.minute = 30;
    time.second = 0;
    DS1302_WriteTime(time);

    date.year = 26;
    date.month = 5;
    date.day = 17;
    date.weekday = 7;
    DS1302_WriteDate(date);

    LCD1602_SetCursor(0, 0);
    LCD1602_WriteString("DS1302 Test");
        DS1302_ReadTime(&time);

        LCD1602_SetCursor(1, 0);
        LCD1602_WriteNum(time.hour, 2);
        LCD1602_WriteData(':');
        LCD1602_WriteNum(time.minute, 2);
        LCD1602_WriteData(':');
        LCD1602_WriteNum(time.second, 2);
        LCD1602_WriteData(' ');

        delay_ms(500);
    }
}

void main()
{
   while(1)
   {
    test_keyScan();
    test_74h595();
    test_segmentled_8D_dispay();
    test_matrix_8x8_display();
    ds1302_test();
   }

}