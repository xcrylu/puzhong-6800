#include "seg7_mpx8.h"


//七段数码管，共阴 码表                 0      1      2      3      4      5      6      7      8      9  
unsigned char code DIGITS_CODE[] = { 0x3F,  0x06,  0x5B,  0x4F,  0x66,  0x6D,  0x7D,  0x07,  0x7F,	 0x6F, 
                                    //A      B      C      D      E      F      -     .     space
                                    0x77,	0x7C,  0x39,  0x5E,	 0x79,	0x71,  0x40,  0x80 , 0x00
                                    };

unsigned char SEG_BUF[8] = {0xff,0xff,18,0xff,0xff,0xff,0xff,0xff};

void SEG7_mpx8_display()
{
    unsigned char i;
    for(i = 0;i<8;i++)	{
        SEG7_DIGIT_PORT = i<<2;
        SEG7_DATA_PORT = 0x00; //消隐
        SEG7_DATA_PORT = DIGITS_CODE[SEG_BUF[i]];
        delay_ms(1);
    }
}

void SEG7_mpx8_set_digit(unsigned char digit, unsigned char dat)
{
    SEG_BUF[digit] = dat;
}


/**
 * @brief 设置8位数码管显示的整数
 * @details 该函数将一个整数设置到8位数码管上显示。
 * @param num 要显示的整数，范围为 -32768 到 32767
 */

void SEG7_mpx8_display_int(int num)
{
    unsigned char i=0,negtive = 0;	
   
    
    for(i=1;i<8;i++) SEG_BUF[i] = 18;  //清空显示
    
    i = 0;
    if(num<0){
        negtive = 1;  //- 负号
        num *= -1;
    }
    
    while(num>0){
        SEG_BUF[i++] = num%10;
        num /= 10;
    }
    
    if(negtive) SEG_BUF[i] = 16;

}

// // 测试七段数码管

// void Test()
// {
   
//     // SEG7_mpx8_display(buf);
//     // delay_ms(1000);
//     // SEG7_mpx8_display_number(-12345678);
//     // delay_ms(1000);
//     // SEG7_mpx8_set_digit(0,0x01);
//     // SEG7_mpx8_set_digit(1,0x02);
//     // SEG7_mpx8_set_digit(2,0x03);
//     // SEG7_mpx8_set_digit(3,0x04);
//     // SEG7_mpx8_set_digit(4,10);
//     // SEG7_mpx8_set_digit(5,11);
//     // SEG7_mpx8_set_digit(6,12);
//     // SEG7_mpx8_set_digit(7,13);
//     SEG7_mpx8_display_int(-12378);
   
//     while(1){
//         //SEG7_mpx8_display_number(34567);
//         SEG7_mpx8_display();
//     }

// }

// void main()
// {
//     Test();
// }