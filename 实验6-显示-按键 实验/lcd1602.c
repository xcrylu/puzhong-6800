/**
 * @file lcd1602.c
 * @brief LCD1602液晶显示模块驱动
 *
 * @version 1.0
 * @date 2026-05-29
 * @author xcr
 *
 * @details
 * 本文件实现了LCD1602液晶显示模块的完整驱动，适用于51单片机。
 *
 * === 硬件连接 ===
 * - RS (Register Select): 寄存器选择引脚
 * - RW (Read/Write): 读写控制引脚
 * - EN (Enable): 使能引脚
 * - DATA: 8位数据端口
 *
 * === 功能描述 ===
 * - LCD1602初始化配置
 * - 命令写入（LCD1602_WriteCmd）
 * - 数据写入（LCD1602_WriteData）
 * - 字符串显示（LCD1602_WriteString）
 * - 光标设置（LCD1602_SetCursor）
 * - 延时功能
 *
 * === 使用方法 ===
 * 1. 修改引脚定义（RS、RW、EN、DATA）
 * 2. 调用 LCD1602_Init() 初始化LCD
 * 3. 调用 LCD1602_SetCursor() 设置光标位置
 * 4. 调用 LCD1602_WriteString() 显示字符串
 *
 * === 注意事项 ===
 * - LCD1602需要正确的初始化序列才能正常工作
 * - 写操作时需要满足时序要求（EN下降沿触发）
 * - 写命令和数据之间需要适当的延时
 * - DDRAM地址：第1行 0x00-0x0F，第2行 0x40-0x4F
 *
 * === 常用命令 ===
 * - 0x01: 清屏
 * - 0x02: 光标返回
 * - 0x38: 8位数据接口，2行显示，5x8点阵
 * - 0x0C: 显示开，光标关
 * - 0x80: 设置DDRAM地址
 */

#include <reg52.h>
#include <stdio.h>


// ==================== LCD1602引脚定义 ====================

sbit LCD1602_RS = _________;      // RS 引脚定义
sbit LCD1602_RW = _________;      // RW 引脚定义
sbit LCD1602_EN = _________;      // EN 引脚定义
#define LCD1602_DATA _________    // 数据口 引脚定义

// ==================== LCD1602命令定义 ====================

#define LCD1602_CMD_CLEAR       0x01    // 清屏
#define LCD1602_CMD_HOME        0x02    // 光标返回
#define LCD1602_CMD_ENTRY_MODE  0x06    // 输入模式设置
#define LCD1602_CMD_DISPLAY_ON  0x0C    // 显示开，光标关
#define LCD1602_CMD_DISPLAY_OFF 0x08    // 显示关
#define LCD1602_CMD_FUNCTION_SET 0x38   // 8位数据，2行显示，5x8点阵
#define LCD1602_CMD_SET_CGRAM   0x40    // 设置CGRAM 地址
#define LCD1602_CMD_SET_DDRAM   0x80    // 设置DDRAM地址

// ==================== 函数声明 ====================


void LCD1602_Delay(unsigned int t);
void delay_ms(unsigned int ms);

void LCD1602_Init(void);

void LCD1602_WriteCmd(unsigned char cmd);
void LCD1602_WriteData(unsigned char dat);

void LCD1602_SetCursor(unsigned char row, unsigned char col);
void LCD1602_WriteString(unsigned char *str);


// ==================== 函数实现 ====================
/**
 * @brief 延时函数
 * @param t 延时计数值
 */
void LCD1602_Delay(unsigned int t) {
    while(t--);
}
void delay_ms(unsigned int ms){
    unsigned char i;
    while(ms--)
        for(i=0;i<110;i++);
}


/**
 * @brief 向LCD写入命令
 * @param cmd 命令字节
 */
void LCD1602_WriteCmd(unsigned char cmd) {  

    //============= 补充完整程序，LCD 1602 写命令功能，==============    
   
    // 设置 RS 和 RW
    _________________;  // 选择命令寄存器
    _________________;  // 写操作
    
    // 数据置于 数据总线
    _________________;   // 准备数据
    
    // 产生使能脉冲
    _________________;          // EN 高电平
    LCD1602_Delay(5);
    _________________;          //EN 低电平
    LCD1602_Delay(5);


   //----------------------------------------------------------------------
}

/**
 * @brief 向LCD写入数据
 * @param dat 数据字节
 */
void LCD1602_WriteData(unsigned char dat) {
     //============= 补充完整程序，LCD 1602 写命令功能，==============    
   
    // 设置 RS 和 RW
    _________________;  // 选择命令寄存器
    _________________;  // 写操作
    
    // 数据置于 数据总线
    _________________;   // 准备数据
    
    // 产生使能脉冲
    _________________;          // EN 高电平
    LCD1602_Delay(5);
    _________________;          // EN 低电平
    LCD1602_Delay(5);


   //----------------------------------------------------------------------
}

///**
// * @brief 检查LCD是否忙，忙则停止等待直到不忙
// * @param 无
// */

void LCD1602_CheckBusy(void)
{
   unsigned char sta;
   LCD1602_RW = 0;       // 读状态寄存器
   LCD1602_EN = 1;       // 读模式
   do
   {
       LCD1602_EN = 1;
       sta = LCD1602_DATA;
       LCD1602_EN = 0;
   }while(sta & 0x80); // BF=1忙碌，循环等待

}

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


//===========================  测试  =============================
// 以下为测试代码
void Test(void)
{
    LCD1602_Init();
    ______________________________;       //设置光标位置为第1行第1列
    ______________________________;       //写 学号
    delay_ms(1000);

    ______________________________;      //设置光标位置为第1行3类
    ______________________________;      //写 姓名拼音
    delay_ms(1000);
    
    while(1);
}

void main()
{
    Test();
    while(1);
}


