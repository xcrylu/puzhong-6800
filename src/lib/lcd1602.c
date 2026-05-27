/**
 * @file lcd1602.c
 * @brief LCD1602显示模块驱动
 * 
 * 实现LCD初始化、命令写入、数据显示及时间/温度格式化显示功能
 */

#include "lcd1602.h"
#include "common.h"
#include <stdio.h>

/**
 * @brief 延时函数
 * @param t 延时计数值
 */
void LCD1602_Delay(unsigned int t) {
    while(t--);
}

/**
 * @brief 向LCD写入命令
 * @param cmd 命令字节
 */
void LCD1602_WriteCmd(unsigned char cmd) {
    LCD1602_RS = 0;  // 选择命令寄存器
    LCD1602_RW = 0;  // 写操作
    LCD1602_DATA = cmd;
    
    // 产生使能脉冲
    LCD1602_EN = 1;
    LCD1602_Delay(5);
    LCD1602_EN = 0;
    LCD1602_Delay(5);
}

/**
 * @brief 向LCD写入数据
 * @param dat 数据字节
 */
void LCD1602_WriteData(unsigned char dat) {
    LCD1602_RS = 1;  // 选择数据寄存器
    LCD1602_RW = 0;  // 写操作
    LCD1602_DATA = dat;
    
    // 产生使能脉冲
    LCD1602_EN = 1;
    LCD1602_Delay(5);
    LCD1602_EN = 0;
    LCD1602_Delay(5);
}

///**
// * @brief 检查LCD是否忙，忙则停止等待直到不忙
// * @param 无
// */

//void LCD1602_CheckBusy(void)
//{
//    unsigned char sta;
//    LCD1602_RW = 0;       // 读状态寄存器
//    LCD1602_EN = 1;       // 读模式
//    do
//    {
//        LCD1602_EN = 1;
//        sta = LCD1602_DATA;
//        LCD1602_EN = 0;
//    }while(sta & 0x80); // BF=1忙碌，循环等待

//}

/**
 * @brief 初始化LCD1602
 */
void LCD1602_Init(void) {
    LCD1602_WriteCmd(LCD1602_CMD_FUNCTION_SET);  // 8位数据，2行显示，5x8点阵    
    LCD1602_WriteCmd(LCD1602_CMD_DISPLAY_ON);    // 显示开，光标关，闪烁关        
    LCD1602_WriteCmd(LCD1602_CMD_ENTRY_MODE);    // 写入后光标右移        
    LCD1602_WriteCmd(LCD1602_CMD_CLEAR);         // 清屏        
    delay_ms(10);
        //LCD1602_Delay(10);
}

/**
 * @brief 设置光标位置
 * @param row 行号（0或1）
 * @param col 列号（0-15）
 */
void LCD1602_SetCursor(unsigned char row, unsigned char col) {
    unsigned char addr;
    
    // 计算DDRAM地址
    if(row == 0) addr = 0x00 + col;
    else addr = 0x40 + col;
    
    LCD1602_WriteCmd(LCD1602_CMD_SET_DDRAM | addr);  // 设置地址
}

/**
 * @brief 写入字符串
 * @param str 字符串指针
 */
void LCD1602_WriteString(unsigned char *str) {
    while(*str) {
        LCD1602_WriteData(*str++);
                
    }
}

/**
 * @brief 写入数字（定长）
 * @param num 数字值
 * @param len 显示长度
 */
void LCD1602_WriteInt( int num) {
    uchar buf[16] = {0};	
	sprintf(buf,"%d\0",num);
    LCD1602_WriteString(buf);      
}

// // 以下为测试代码
// void Test(void)
// {
//     LCD1602_Init();
//     LCD1602_WriteString("Hello, World!");
//     delay_ms(1000);
//     LCD1602_WriteString("Hello, LCD!");
//     delay_ms(1000);
//     LCD1602_WriteString("Hello, LCD!");
//     delay_ms(1000);
//     while(1);
// }

// void main()
// {
//     Test();
//     while(1);
// }


