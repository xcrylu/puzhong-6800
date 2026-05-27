#include "tm1637.h"

// tm1637 显示按键/模块 驱动

//共阴数码管码表
unsigned char code SEG_TABLE[19] = {
      0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07, //0~7
      0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71, //8,9,A~F
      0x40,0x80,0x00                            // - , .  不显示
    };


// 短延时（适配11.0592MHz）
void TM1637_Delay(void)
{
    unsigned char i;
    for(i=0; i<10; i++);
}

// 起始信号
void TM1637_Start(void)
{
    TM1637_CLK = 1;
    TM1637_DIO = 1;
    TM1637_Delay();
    TM1637_DIO = 0;
    TM1637_Delay();
    TM1637_CLK = 0;
}

// 停止信号
void TM1637_Stop(void)
{
    TM1637_CLK = 0;
    TM1637_DIO = 0;
    TM1637_Delay();
    TM1637_CLK = 1;
    TM1637_Delay();
    TM1637_DIO = 1;
}

// 写字节（带ACK）
bit TM1637_Write(unsigned char dat)
{
    unsigned char i;
    bit ack;

    for(i=0; i<8; i++)
    {
        TM1637_CLK = 0;
        TM1637_Delay();
        TM1637_DIO = dat & 0x01;
        TM1637_Delay();
        TM1637_CLK = 1;
        TM1637_Delay();
        dat >>= 1;
    }

    // 读ACK
    TM1637_CLK = 0;
    TM1637_DIO = 1;  // 输入
    TM1637_Delay();
    ack = TM1637_DIO;
    TM1637_CLK = 1;
    TM1637_Delay();

    return ack;
}

// 读字节（读按键用）
unsigned char TM1637_ReadByte(void)
{
    unsigned char i, dat = 0;
    TM1637_DIO = 1;  // 设为输入

      //低位在前
    for(i=0; i<8; i++)
    {
        dat >>= 1;
        TM1637_CLK = 0;
        TM1637_Delay();
        if(TM1637_DIO)
            dat |= 0x80;
        TM1637_CLK = 1;
        TM1637_Delay();
    }
    return dat;
}

// 初始化
void TM1637_Init(void)
{
    TM1637_CLK = 1;
    TM1637_DIO = 1;
    TM1637_SetBright(BRIGHT_4);
        
}

// 设置亮度
void TM1637_SetBright(unsigned char bright)
{
    TM1637_Start();
    TM1637_Write(0x88 | bright);  // 显示开+亮度
    TM1637_Stop();    
}



// 显示6位数字 -32768~32767
void TM1637_DisplayInt(int num)
{
    unsigned char buf[6] = {18,18,18,18,18,18}; // 全部默认熄灭
    char i = 0;
    bit isNegative = 0;

    // 处理0
    if(num == 0)
    {
        buf[0] = 0;
    }
    else
    {
        // 处理负数
        if(num < 0)
        {
            isNegative = 1;
            num = -num;
        }

        // 拆数字
        while(num > 0 && i<6)
        {
            buf[i++] = num % 10;
            num /= 10;
        }

        // 负数加负号（紧贴数字，不悬空）
        if(isNegative && i<6)
        {
            buf[i] = 16;
        }
    }

    // 发送数据
    TM1637_Start();
    TM1637_Write(0x40);
    TM1637_Stop();

    TM1637_Start();
    TM1637_Write(0xC0);

    // 输出到6位数码管
    for(i=5; i>=0; i--)
    {
        TM1637_Write(SEG_TABLE[buf[i]]);
    }

    TM1637_Stop();
}



// 显示实数（支持负数 + 小数点）
// 显示范围：-999.99 ~ 999.99
void TM1637_DisplayFloat(float num)
{
    unsigned char buf[6] = {18,18,18,18,18,18}; // 默认全灭
    bit isNegative = 0;
    long temp;
    char i;

    // 1. 处理负数
    if(num < 0)
    {
        isNegative = 1;
        num = -num;
    }

    // 2. 放大100倍，变成整数（避免浮点误差）
    temp = (long)(num * 100 + 0.5); // 四舍五入

    // 3. 拆出最后两位 = 小数位
    buf[0] = temp % 10; temp /= 10;
    buf[1] = temp % 10; temp /= 10;

    // 4. 拆整数位
    i = 2;
        if(temp == 0)
    {
        buf[i++] = 0; // 修复BUG：强制显示整数0
    }
    while(temp > 0 && i < 5)
    {
        buf[i++] = temp % 10;
        temp /= 10;
    }

    // 5. 负数显示 '-'
    if(isNegative && i < 6)
    {
        buf[i] = 16;
    }

    // ===================== 发送显示 =====================
    TM1637_Start();
    TM1637_Write(0x40);
    TM1637_Stop();

    TM1637_Start();
    TM1637_Write(0xC0);

    // 从高位到低位输出
    for(i = 5; i >= 0; i--)
    {
        unsigned char seg = SEG_TABLE[buf[i]];

        // 在第2位（小数点位置）自动加小数点
        if(i == 2) seg |= 0x80;

        TM1637_Write(seg);
    }

    TM1637_Stop();
}


// 清屏
void TM1637_Clear(void)
{
    TM1637_Start();
    TM1637_Write(0x40);
    TM1637_Stop();

    TM1637_Start();
    TM1637_Write(0xC0);//首地址
    TM1637_Write(0x00);//数据1
    TM1637_Write(0x00);
    TM1637_Write(0x00);
    TM1637_Write(0x00);
    TM1637_Write(0x00);
    TM1637_Write(0x00);//数据6
    TM1637_Stop();
}

// --------------------------
// TM1637 按键读取（核心）
// --------------------------
// 返回值：
// 0 = 无按键
// 1~8 = K1行 SG1~SG8
// 9~16= K2行 SG1~SG8
// --------------------------
unsigned char TM1637_ReadKey(void)
{
    unsigned char keyTM1637;
    unsigned char key=0;

    TM1637_Start();
    TM1637_Write(0x42);  // 读按键命令
    keyTM1637 = TM1637_ReadByte();  // 读按键值

    TM1637_Stop();
//在有按键按下时，读键数据如下：(低位在前，高位在后) 
//    SG1       SG2      SG3       SG4       SG5       SG6       SG7       SG8 
//K1 1110_1111 0110_1111 1010_1111 0010_1111 1100_1111 0100_1111 1000_1111 0000_1111  低位在前
//   0xF7      0xF6      0xF5      0xF4      0xF3      0xF2      0xF1     0xF0         hex字节表示
//   247       246       245      244        243      242         241     240 
//K2 1111_0111 0111_0111 1011_0111 0011_0111 1101_0111 0101_0111 1001_0111 0001_0111  低位在前
//   0xEF      0xEE      0xED      0xEC      0xEB      0xEA       0xE9     0xE8      hex字节表示
//   239       238       237        236      235       234        233      232 
      
    if(keyTM1637 != 255 )  //有键按下
    {
        switch(keyTM1637)
        {
            case 0xf7: key=1; break;
            case 0xf6: key=2; break;
            case 0xf5: key=3; break;
            case 0xf4: key=4; break;
            case 0xf3: key=5; break;
            case 0xf2: key=6; break;
            case 0xf1: key=7; break;
            case 0xf0: key=8; break;                
            case 0xef: key=9; break;
            case 0xee: key=10; break;
            case 0xed: key=11; break;
            case 0xec: key=12; break;
            case 0xeb: key=13; break;
            case 0xea: key=14; break;
            case 0xe9: key=15; break;
            case 0xe8: key=16; break;
        }
    }        
//    
    return key ;

}