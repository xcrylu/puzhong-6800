#include <reg52.h>
#include <intrins.h>

sbit DQ = P3^7;

void delay_us(unsigned char us);
void delay_ms(unsigned int  ms);
unsigned char ds18b20_reset();
void ds18b20_write_byte(unsigned char dat);
unsigned char ds18b20_read_byte();
void ds18b20_start_convert();
int ds18b20_read_temperature();



void main()
{
   int temp;
 
    while (1)
    {
			  ds18b20_reset();

        ds18b20_start_convert();
        delay_ms(750);                     // 最长转换时间（12位精度）
        temp = ds18b20_read_temperature(); // 单位：0.001°C
        // 分离各位数字（假设温度0~99.9°C）
       
        // 动态扫描显示
        P1 =(unsigned char)temp; // 显示整数部分 
    }
}




// DS18B20复位
unsigned char ds18b20_reset()
{
    unsigned char presence;
    DQ = 0;
    delay_us(70); // 至少480μs
    DQ = 1;       // 主机释放总线
	  delay_us(6);    // 等待从机响应 15~60us；约60us
    presence = DQ;   // 从机拉低表示存在
    delay_us(60);   // 回应周期大于480us，等420us
    return presence; // 0: 存在，1: 无设备
}
// 写一个字节
void ds18b20_write_byte(unsigned char dat)
{
    unsigned char i;
    for (i = 0; i < 8; i++)
    {
        DQ = 0;
        _nop_();
        DQ = dat & 0x01; // 先写低位
        delay_us(6);    // 位周期大于60us
        DQ = 1;
        dat >>= 1;
    }
}
// 读一个字节
unsigned char ds18b20_read_byte()
{
    unsigned char i, dat = 0;
    for (i = 0; i < 8; i++)
    {   dat >>= 1; 
        DQ = 0;
        _nop_();
        DQ = 1; // 释放总线
        _nop_();
			  _nop_();        
			if (DQ)   //保证15us内读取数据
            dat |= 0x80;
			delay_us(6); //bit周期大于60us
    }
    return dat;
}

void ds18b20_start_convert()
{
    ds18b20_reset();
    ds18b20_write_byte(0xCC); // 跳过ROM命令（单设备）
    ds18b20_write_byte(0x44); // 启动温度转换
}

int ds18b20_read_temperature()
{
    unsigned char TL, TH;
    int temp;
    ds18b20_reset();
    ds18b20_write_byte(0xCC); // 跳过ROM
    ds18b20_write_byte(0xBE); // 读温度寄存器
    TL = ds18b20_read_byte(); // 低8位
    TH = ds18b20_read_byte(); // 高8位
    temp = (TH << 8) | TL;    // 合成16位数据
    // 转换为实际温度（默认12位精度，0.0625°C/LSB）
    if (TH & 0x80)
    { // 负数（补码）
        temp = ~temp + 1;
        return -(temp >> 4); // 转为摄氏度（保留整数部分）
    }
    return (temp >> 4); // 正数（保留整数部分）
}

//us == 1 ，大概20us
//us == 2       27us
//us == 3       35us
//      4       42
//每增加1      增加大约7us
void delay_us(unsigned char us)
{
   while(us--) ;	

}

void delay_ms(unsigned int  ms)		//延时函数，时钟频率11.0592，大约延时1ms
{
	unsigned char j;
	while(ms--)
		for(j = 0;j<110;j++);
}

