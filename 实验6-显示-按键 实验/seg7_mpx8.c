

/**
 * @file seg7_mpx8.c
 * @brief 8位七段数码管动态扫描驱动
 *
 * @version 1.0
 * @date 2026-05-29
 * @author xcr
 *
 * @details
 * 本文件实现了8位七段数码管的动态扫描显示驱动，适用于51单片机。
 *
 * === 硬件连接 ===
 * - 段选信号: P0口（或其他GPIO，需修改 SEG7_DATA_PORT）
 * - 位选信号: P2口（通过3-8译码器，需修改 SEG7_DIGIT_PORT）
 *
 * === 功能描述 ===
 * - 支持8位数码管同时显示
 * - 支持0-9数字、A-F字母、负号、小数点、空白显示
 * - 支持整数显示（正负均可）
 * - 采用动态扫描方式，利用视觉暂留效应
 *
 * === 使用方法 ===
 * 1. 修改引脚定义（SEG7_DATA_PORT、SEG7_DIGIT_PORT）
 * 2. 补充数码管码表（DIGITS_CODE）
 * 3. 调用 SEG7_mpx8_display_int() 设置要显示的数字
 * 4. 在主循环中调用 SEG7_mpx8_display() 刷新显示
 *
 * === 注意事项 ===
 * - 动态扫描需要在主循环中持续调用显示函数
 * - 扫描周期约1ms*8=8ms，频率约125Hz，无闪烁
 * - SEG_BUF[0]对应最右边（个位），SEG_BUF[7]对应最左边
 *
 * === 数码管码表说明 ===
 * 共阴极数码管编码：
 * - 0-9: 数字0-9
 * - 10-15: 字母A-F
 * - 16: 负号'-' (0x40)
 * - 17: 小数点'.' (0x80)
 * - 18: 空白熄灭 (0x00)
 */

#include <reg52.h>

// ==================== 数码管引脚定义 ====================

#define SEG7_DATA_PORT  _______________________   // 请补充数码管数据引脚定义
#define SEG7_DIGIT_PORT _______________________   // 请补充数码管位选引脚定义

// ==================== 函数声明 ====================
/*  
 * @brief 显示整数到数码管
 * @details 该函数将指定的整数显示到8位数码管上。
 * @param num 要显示的整数，范围为 -32768 到 32767
 */
void SEG7_mpx8_display_int(int num);

/*  
 * @brief 设置8位数码管的位显示
 * @details 该函数将指定的位号和显示数据设置到8位数码管上。
 * @param digit 数码管的位号，0-7
 * @param dat 数码管显示的数字，0-18，分别对应 0-9、A-F、-、 . 和 空白
 */
void SEG7_mpx8_set_digit(unsigned char digit, unsigned char dat);

/*  
 * @brief 扫描显示8位数码管
 * @details 该函数将8位数码管显示的数字刷新到数码管上。
 */
void SEG7_mpx8_display();

//七段数码管，共阴 码表                 0      1      2      3      4      5      6      7      8      9  
unsigned char code DIGITS_CODE[19] = {  _______________________/* 请补充数码管码0~9编码 */, 
                                    //A      B      C      D      E      F      -     .     space
                                     _______________________/* 请补充数码管码A~F编码 */,  0x40,  0x80 , 0x00
                                    };

                                    

// 八位数码管显示缓冲区
// SEG_BUF[0]: 最右边（个位）
// SEG_BUF[7]: 最左边（千万位）
// 0-9: 数字0-9, 10-15: A-F, 16: '-', 17: '.', 18: 空白
unsigned char SEG_BUF[8] = {18, 18, 18, 18, 18, 18, 18, 18};


void delay_ms(unsigned int ms);

void delay_ms(unsigned int ms){
    unsigned char i;
    while(ms--){
        for(i = 0;i < 112;i++);
    }
}

/**
 * @brief 扫描显示8位数码管（动态扫描）
 * @details 该函数采用动态扫描方式依次点亮8位数码管，利用人眼的视觉暂留效应。
 *          扫描频率约为 1000ms / (1ms * 8) = 125Hz，可避免闪烁。
 * 
 * 工作原理：
 * 1. 循环点亮每一位数码管
 * 2. 先输出段选信号（显示数据）
 * 3. 再输出位选信号（选择哪一位亮）
 * 4. 延时约1ms
 * 5. 消隐，避免拖影
 * 
 * @note 位选信号 i&lt;&lt;2 取决于硬件连接，请根据实际电路调整
 */
void SEG7_mpx8_display()
{
    //============= 补充完整程序，扫描显示8个七段数码管=====================    
    unsigned char i,tmpCode;    
    for(i = 0; i <___________; i++)	{            //八个数码管依次显示
        SEG7_DATA_PORT = ___________;           // 消隐：先关闭段选
        SEG7_DIGIT_PORT = i << 2;              // 位选：选择第i位数码管,我们使用了P2.2~P2.4,通过3-8译码器转换成8选1位
        tmpCode = ________________;            //获取第i个的数码管码
        SEG7_DATA_PORT = ________________;     // 段选：输出显示数据
        delay_ms(1);                     // 延时，保持该位点亮约1ms
    }
    //------------------------------------------------------------------
}

/**
 * @brief 设置8位数码管的某一位显示内容
 * @details 该函数将指定的位号和显示数据设置到显示缓冲区中。
 * 
 * @param digit 数码管的位号，范围 0-7
 *              - 0: 最右边（最低位）
 *              - 7: 最左边（最高位）
 * @param dat 数码管显示的数据，范围 0-18
 *            - 0-9: 显示数字0-9
 *            - 10-15: 显示字母A-F
 *            - 16: 显示负号'-'
 *            - 17: 显示小数点'.'
 *            - 18: 显示空白（熄灭）
 */
void SEG7_mpx8_set_digit(unsigned char digit, unsigned char dat)
{
    if(digit<19) {  // 防止越界
        SEG_BUF[digit] = dat;
    }
}

/**
 * @brief 设置8位数码管显示整数
 * @details 该函数将一个整数分解并设置到显示缓冲区中。
 *          支持负数显示，自动处理前导零。
 * 
 * @param num 要显示的整数，范围为 -32768 到 32767
 * 
 * 显示格式：
 * - 正数：如 1234 → 显示 "  1234"
 * - 负数：如 -1234 → 显示 " -1234"
 * - 低位在右边，高位在左边
 * 
 * 算法说明：
 * 1. 清空显示缓冲区（设置为空白）
 * 2. 处理负数（记录负号标志）
 * 3. 从低位到高位提取数字，存入缓冲区
 * 4. 如果是负数，在最高位显示负号
 * 
 * @note 显示顺序：SEG_BUF[0]是最右边（个位），SEG_BUF[7]是最左边（千万位）
 */
void SEG7_mpx8_display_int(int num)
{
    unsigned char i = 0, negtive = 0;	
   
    // 清空显示缓冲区（全部设置为空白）
    for(i = 0; i <  8; i++) {
        SEG_BUF[i] = 18;  // 18表示空白
    }
    
    i = 0;
    // 处理负数
    if(num < 0) {
        negtive = 1;  // 记录负号标志
        num = -num;   // 取绝对值
    }
    
    // 从低位到高位提取数字
    while(num > 0 && i <  8) {
        SEG_BUF[i++] = num % 10;  // 取出当前位（个位、十位、百位...）
        num /= 10;                 // 去掉当前位
    }
    
    // 如果是负数，在最高有效位左边显示负号
    if(negtive && i < 8) {
        SEG_BUF[i] = 16;  // 16表示负号
    }
}

// // 测试七段数码管

void Test()
{
   
    // SEG7_mpx8_display(buf);
    // delay_ms(1000);
    // SEG7_mpx8_display_number(-12345678);
    // delay_ms(1000);
    // SEG7_mpx8_set_digit(0,0x01);
    // SEG7_mpx8_set_digit(1,0x02);
    // SEG7_mpx8_set_digit(2,0x03);
    // SEG7_mpx8_set_digit(3,0x04);
    // SEG7_mpx8_set_digit(4,10);
    // SEG7_mpx8_set_digit(5,11);
    // SEG7_mpx8_set_digit(6,12);
    // SEG7_mpx8_set_digit(7,13);
    SEG7_mpx8_display_int(-12378);
   
    while(1){
        
        ___________________________;  // 调用显示函数 显示当前缓冲区数据
        //delay_ms(1000);
    }

}

void main()
{
    Test();
}