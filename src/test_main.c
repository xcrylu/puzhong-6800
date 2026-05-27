//#include <reg52.h>
#include <intrins.h>
#include "led_display.h"
#include "common.h"
#include "keyScan.h"
#include "lcd1602.h"
#include "i2c.h"    

void test_74h595(void);
void test_matrix_8x8_display(void);
void test_segmentled_8D_dispay(void);
void test_keyScan(void);
                                        
void main()
{    
//    char *outbuf="dad is gay";
//    char inbuf[11]={0};
//    

//    // 初始化LCD1602
//    LCD1602_Init();
//    delay_ms(10);
//    
//    // 写入数据到EEPROM
//    I2C_WriteDatas_Addr(0x00,outbuf,5);
//    delay_ms(10);
//    LCD1602_SetCursor(0,0);
//     LCD1602_WriteString(outbuf);
    //delay_ms(10);


//    // 从EEPROM读取数据
//       I2C_ReadDatas_Addr(0x00,inbuf,10);
//    inbuf[11]='\0';
//    LCD1602_SetCursor(1,0);
//    LCD1602_WriteString(inbuf);
//    
    
    while(1){
//        LCD1602_SetCursor(1,0);
//  LCD1602_WriteNum(123,3);
         test_keyScan();
        //delay_ms(1000);
        //P2=0xf0;
        //test_74h595();
        //test_matrix_8x8_display();    
        //test_segmentled_8D_dispay();    
        //    delay_ms(1);
        //SEG_8DIG_display_number(12345);
//        key = KeyScan();
//        if(key != 0xff)  P2 = key;
        //P2 = 6;
    }
    //ds1302_test();
}



