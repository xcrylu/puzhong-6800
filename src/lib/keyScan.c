// #include <reg52.h>

#include "common.h"
#include "keyScan.h"

/*************************************************
 * 函数：KeyScan
 * 功能：4×4矩阵键盘扫描
 * 返回：0~15 按键编号；无按键返回 0xff
 *************************************************/
u8 code KeyTable[4][4] = {
    {'0', '1', '2', '3'},
    {'4', '5', '6', '7'},
    {'8', '9', 'A', 'B'},
    {'C', 'D', 'E', 'F'}};

/************************* 矩阵键盘扫描（查表法） *************************
函数功能：无位定义，纯端口操作，查表返回键值
返回值：0~15 有效按键；0xFF 无按键
****************************************************************************/
u8 KeyScan(void)
{
    u8 row, col, temp,key=0xff;

    // 第一步：全行拉低，检测是否有按键按下
    KEY_PORT = 0x0F;        // 行： 0000 1111→ 低4位列=1，高4位行=0
    temp = KEY_PORT & 0x0F; // 读取列电平
    if (temp == 0x0F)
        return 0xFF; // 无按键

    // 第二步：消抖
    delay_ms(2);

    KEY_PORT = 0x0F;
    temp = KEY_PORT & 0x0F;
    if (temp == 0x0F)
        return 0xFF;

    // 第三步：逐行扫描，确定行号
    for (row = 0; row < 4; row++)
    {
        KEY_PORT = 0x0F | (0xF0 ^ (0x80 >> row)); // 逐行拉低
        temp = KEY_PORT & 0x0F;                   // 读取列

        if (temp != 0x0F) // 该行有按键
        {
            // 判断列号
            if ((temp & 0x08) == 0)
                col = 0;
            else if ((temp & 0x04) == 0)
                col = 1;
            else if ((temp & 0x02) == 0)
                col = 2;
            else if ((temp & 0x01) == 0)
                col = 3;
 
            // 查表返回键值
            key =  KeyTable[row][col];
						while((KEY_PORT&0x0f)!=0x0f);
        }
    }
		
    return key;
}

u8 KeyScan_v2(void)
{
    u8 row, col, temp,key = 0xff;
   
    // 第一步：全行拉低，检测是否有按键按下
    KEY_PORT = 0x0F;        // 行： 0000 1111→ 低4位列=1，高4位行=0
    temp = KEY_PORT & 0x0F; // 读取列电平
    if (temp == 0x0F)
        return 0xFF; // 无按键


    // 第二步：逐行扫描，确定行号
    for (row = 0; row < 4; row++)
    {
        KEY_PORT = 0x0F | (0xF0 ^ (0x80 >> row)); // 逐行拉低
        temp = KEY_PORT & 0x0F;                   // 读取列

        if (temp != 0x0F) // 该行有按键
        {
            // 判断列号
            if ((temp & 0x08) == 0)
                col = 0;
            else if ((temp & 0x04) == 0)
                col = 1;
            else if ((temp & 0x02) == 0)
                col = 2;
            else if ((temp & 0x01) == 0)
                col = 3;

            //等待按键释放
			      while((KEY_PORT&0x0f)!=0x0f);
             // 查表返回键值
            return KeyTable[row][col];
        }
    }
    return 0xFF;
}

u8 KeyScan_v3(void)
{
    u8 row = 0, col = 0, temp, key = 0xff;

    // 第一步：全行拉低，检测是否有按键按下
    KEY_PORT = 0x0F;        // 行： 0000 1111→ 低4位列=1，高4位行=0
    temp = KEY_PORT & 0x0F; // 读取列电平
    if ((temp&0x0F) == 0x0F)
        return 0xFF; // 无按键

    delay_ms(2);
    // 计算行号
    if ((temp & 0x01) == 0)
        row = 0;
    else if ((temp & 0x02) == 0)
        row = 1;
    else if ((temp & 0x04) == 0)
        row = 2;
    else if ((temp & 0x08) == 0)
        row = 3;

    delay_ms(2);
    // 计算列号
    KEY_PORT = 0xff;        //
    KEY_PORT = 0xF0;        // 行： 1111 0000 → 低4位列=0，高4位行=1
    temp = KEY_PORT & 0xF0; // 读取列电平
    if (temp == 0xF0)
        return 0xFF; // 无按键
    temp = temp >> 4;
    if ((temp & 0x01) == 0)
        col = 0;
    else if ((temp & 0x02) == 0)
        col = 1;
    else if ((temp & 0x04) == 0)
        col = 2;
    else if ((temp & 0x08) == 0)
        col = 3;

    //等待按键释放    
    while((KEY_PORT&0xF0)!=0xF0);
    return KeyTable[row][col];
}



// 以下是测试代码，使用串口发送按键值
#include "uart.h"
void Test()
{
    u8 key;

    uart_init();
    while (1)
    {
        key = KeyScan();
        //key = KeyScan_v2();
        //key = KeyScan_v3();
			 
        //delay_ms(10);
         if (key != 0xFF)
         {
           uart_send(key); // 发送按键值到串口					 
         }
				
     }

}
 
//#include "lcd1602.h"
//void test_key_lcd1602()
//{
//		u8 key;
//    uart_init();
//	  LCD1602_Init();
//   
//     while (1)
//     {
//        key = KeyScan_v2();
//			 
//        //delay_ms(10);
//         if (key != 0xFF)
//         {
//           uart_send(key); // 发送按键值到串口					 
//					 LCD1602_WriteData(key);
//         }
//				
//     }
// 
//}

#include "matrix8x8_led.h"
void test_key_matrixled(void)
{ 
	u8 key,show_digit = 0;
	uart_init();
	while(1){
		key = KeyScan_v2();
		if(key != 0xff)
		{ 
			uart_send(key); 
			show_digit = key < 'A'? key - '0': key - 'A'+10;	
			//show_digit = 1;
		}
		matrix_8x8_display_digit(show_digit);
	}

}
 
#include "seg7_mpx8.h"
void test_key_seg7()
{
	 u8 key,show_key = -1;
    
	 //LCD1602_Init();
     uart_init();
     while (1)
     {
        key = KeyScan_v2();
			 
        //delay_ms(10);
         if (key != 0xFF)
         {
           uart_send(key); // 发送按键值到串口
					 show_key = key > '9' ? key - '0': key - 'A';
					 //LCD1602_WriteData(key);
         }
				 SEG7_mpx8_display_number(show_key);
     }
 
}

void main()
{
 	 test_key_matrixled();
}


