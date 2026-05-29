/**
 * @file matrix8x8_led.c
 * @brief 8x8 LED点阵显示驱动
 *
 * @version 1.0
 * @date 2026-05-29
 * @author xcr
 *
 * @details
 * 本文件实现了8x8 LED点阵显示驱动，通过74HC595移位寄存器控制。
 *
 * === 硬件连接 ===
 * - DATA_PORT: 数据输入端口
 * - ST_CP: 74HC595 存储寄存器时钟（latch）
 * - SH_CP: 74HC595 移位寄存器时钟（clock）
 * - DS: 74HC595 串行数据输入
 *
 * === 功能描述 ===
 * - 74HC595移位寄存器驱动
 * - 8x8 LED点阵显示
 * - 支持数字0-9和字母A-F显示
 * - 动态扫描显示方式
 *
 * === 使用方法 ===
 * 1. 修改引脚定义（ST_CP、SH_CP、DS）
 * 2. 调用 matrix_8x8_display() 显示数组数据
 * 3. 调用 matrix_8x8_display_digit() 显示数字/字母
 *
 * === 注意事项 ===
 * - 使用74HC595时需要先发送数据，再更新latch
 * - 显示采用动态扫描方式，需要持续调用显示函数
 * - 字模数据按列编码，高位在点阵最左
 */

#include <reg52.h>


// ================  引脚定义  =========================================
#define MATRIX_DATA_PORT  _________________          // 点阵数据输入端口
sbit ST_CP =  _________________;                     // 74H595 st_cp 引脚     
sbit SH_CP =  _________________;                     // 74H595 sh_cp 引脚
sbit DS    = _________________;                      // 74H595 ds 引脚


// ================  函数定义  =========================================
void delay_us(unsigned char us);
void delay_ms(unsigned char ms);
void write_74h595(unsigned char x);
void 

void matrix_8x8_display(unsigned char buf[8]);
void matrix_8x8_display_digit(unsigned char digit);

// ================  字模数据  =========================================
// 8*8 LED矩阵取模  credit: https://xantorohara.github.io/led-matrix-editor/#
unsigned char code DigitPattern[16][8] = {      //按列编码，高位在对应点阵屏最左led

  //============= 登录 https://xantorohara.github.io/led-matrix-editor/# 
  // 获取 0~9、A~F 字符 8*8字模 



  //----------------------------------------------------------------------

};


// ================  函数实现  =========================================
/**
 * @brief 微秒延时函数
 * @param us 延时时间（微秒）
 * @note 基于软件循环延时，延时时长 ~14+7*us 微秒
 */
void delay_us(unsigned char us)
{
    while(us--);
}
void delay_ms(unsigned char ms)
{
    unsigned char i = 0;
    while(ms--) {
       for(i = 0;i < 110;i++) {
          ;
       }
    }
}

void write_74h595(unsigned char x)
{
    unsigned char i = 0;

    ST_CP = 0; //锁存低电平
    for(i = 0; i<8;i++)
    {
        SH_CP = 0;         //移位时钟低
        delay_us(1);
        DS = x&0x80;       //准备数据最高位先移入
        delay_us(1);
        SH_CP = 1;         //移入1位
        delay_us(1);
        SH_CP = 0;
        x<<= 1;

    }
    ST_CP = 0;
    delay_us(1);
    ST_CP = 1; //锁存数据
    delay_us(1);
    ST_CP = 0;
    delay_us(1);

}

// ================  显示函数  =========================================
/**
 * @brief 显示8x8 LED点阵数据（动态扫描）
 * @param buf 8字节数组，每个字节对应一列
 *
 * @details
 * 本函数采用动态扫描方式驱动8x8 LED点阵显示。
 *
 * === 工作原理 ===
 * 8x8 LED点阵由64个LED组成，采用行列扫描方式：
 * - 列：8列（通过74HC595控制，低电平选中）
 * - 行：8行（直接通过单片机IO口控制，低电平点亮）
 *
 * === 扫描流程 ===
 * 1. 消隐：先关闭所有显示，避免切换时的拖影
 * 2. 选列：通过74HC595输出列选信号（一次选通一列）
 * 3. 送行：输出该列对应的行数据（哪些行亮）
 * 4. 延时：保持该列显示约1ms（利用视觉暂留）
 * 5. 重复：对8列依次扫描，循环往复

 * === 时序 ===
 * - 先发送8位数据到74HC595
 * - 再产生锁存脉冲，更新输出
 *
 * @note 扫描频率约 1000ms/(1ms*8) = 125Hz，避免闪烁
 * @note 列选数据高电平有效（如：0xFE选通第1列，0x7F选通第8列）
 * @note 行数据低电平有效（如：0x00点亮所有行，0xFF熄灭所有行）
 */
void matrix_8x8_display(unsigned char buf[8])
{
    unsigned char i;

    MATRIX_DATA_PORT =_______________________;  // 消隐：先关闭所有显示，避免拖影

    // 依次扫描8列
    for(i = 0; i < _________________; i++) {  //依次扫描8列，每个列对应一个字节
        // 第1步：消隐（避免切换时的残影）
        MATRIX_DATA_PORT =_______________________;

        // 第2步：发送列选信号到74HC595（8位数据，低电平选通对应列）
        // 0xFE = 11111110 → 选通第1列（PENELope）
        // 0xFD = 11111101 → 选通第2列
        // 0xFB = 11111011 → 选通第3列
        // ...依次类推
        write_74h595(_______________________);   // 列选信号（高位在右）

        // 第3步：发送该列的行数据（低电平点亮对应的LED）
        MATRIX_DATA_PORT = ______________________;  // buf[i]包含该列8行的亮灭数据

        // 第4步：延时保持显示（约1ms，利用视觉暂留效应）
       ______________________;             
    }
}

void matrix_8x8_display_digit(unsigned char digit)
{    
    matrix_8x8_display(DigitPattern[digit]);
}

// ================  测试函数  =========================================
// 以下是测试代码
void Test()
{
   
    unsigned long i =0 ;
    while(1)
    {
        while(i++);{
            matrix_8x8_display_digit(i/1000 % 16);
        }           
    }
   
}

void main()n
{
    Test();
}

