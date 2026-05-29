/**
 * @file dac0832.c
 * @brief DAC0832 数模转换芯片驱动实现
 */

#include "dac0832.h"
#include <absacc.h> 

#define DAC0832 XBYTE[0x7FFF] 			// DAC0832地址

void DAC0832_Init(void)
{
    DAC0832_DATA_PORT = 0x00;
    DAC0832_WR = 1;
    DAC0832_CLR = 1;
}

void DAC0832_Write(unsigned char value)
{
    DAC0832_DATA_PORT = value;
    DAC0832_CLR = 1;
    DAC0832_WR = 0;   
    DAC0832_WR = 1;
//    DAC0832_Delay();
}

#define DAC0832_InputRegister       XBYTE[0x00]
#define DAC0832_DACRegister         XBYTE[0x01]
#define DAC0832_ConvertRegister     XBYTE[0x02]



#define DAC0832_WriteIputRegister(value)     {DAC0832_InputRegister = value;}
// LE1 低电平触发转换
#define DAC0832_WriteDacRegister(value)      {DAC0832_DACRegister = value;}

// LE2 低电平触发转换
#define DAC0832_Convert()  { DAC0832_ConvertRegister = value; }    

#define DAC0832_ConvertValue(value)  {DAC0832_ConvertRegister = value;}



void DAC0832_Clear(void)
{
    DAC0832_CLR = 0;
//    DAC0832_Delay();
    DAC0832_CLR = 1;
//    DAC0832_Delay();
}


Test(void)
{  
    uchar i = 0;
    //DAC0832_Init();
    while(1)
    {
        for(i = 0;i < 256;i++)
        {
            //DAC0832_ConvertValue(i);
            DAC0832 = i; 
					  delay_us(20);
        }
        // DAC0832_Convert();
    }
}

void main()
{
   
    Test();
}
