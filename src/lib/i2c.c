/**
 * @file i2c.c
 * @brief I2C总线驱动
 * 
 * 实现I2C总线操作及时间/温度数据的读写功能
 */

#include "i2c.h"
#include "common.h"

/**
 * @brief I2C总线延时函数
 */
void I2C_Delay(void) {
    unsigned char i = 1;
    while(i--);
}

/**
 * @brief 发送I2C起始信号
 */
void I2C_Start(void) {
    I2C_SDA = 1;
    I2C_SCL = 1;
    I2C_Delay();
    
    // SDA从高变低，产生起始信号
    I2C_SDA = 0;
    I2C_Delay();
    
    I2C_SCL = 0;
}

/**
 * @brief 发送I2C停止信号
 */
void I2C_Stop(void) {
    I2C_SDA = 0;
    I2C_SCL = 1;
    I2C_Delay();
    
    // SDA从低变高，产生停止信号
    I2C_SDA = 1;
    I2C_Delay();
}

/**
 * @brief 等待从设备应答
 * @return 0表示收到应答，1表示无应答
 */
unsigned char I2C_Ack(void) {
    unsigned char ack;
    
    I2C_SDA = 1;
    I2C_SCL = 1;
    I2C_Delay();
    
    // 读取应答信号
    ack = I2C_SDA;
    
    I2C_SCL = 0;
    I2C_Delay();
    
    return ack;
}

/**
 * @brief 发送应答信号
 * @param ack 0表示发送应答，1表示发送非应答
 */
void I2C_SendAck(unsigned char ack) {
    I2C_SDA = ack;
    I2C_SCL = 1;
    I2C_Delay();
    
    I2C_SCL = 0;
    I2C_Delay();
}

/**
 * @brief 向I2C总线写入一个字节
 * @param dat 要写入的数据
 */
void I2C_Write8(unsigned char dat) {
    unsigned char i;
    
    for(i = 0; i < 8; i++) {
        // 发送最高位
        I2C_SDA = dat & 0x80;
        dat <<= 1;
        
        // 时钟上升沿锁存数据
        I2C_SCL = 1;
        I2C_Delay();
        
        I2C_SCL = 0;
        I2C_Delay();
    }
}

/**
 * @brief 从I2C总线读取一个字节
 * @return 读取的数据
 */
unsigned char I2C_Read8(void) {
    unsigned char i, dat = 0;
    
    I2C_SDA = 1;
    
    for(i = 0; i < 8; i++) {
        dat <<= 1;
        
        // 时钟上升沿读取数据
        I2C_SCL = 1;
        I2C_Delay();
        
        dat |= I2C_SDA;
        
        I2C_SCL = 0;
        I2C_Delay();
    }
    
    return dat;
}


/**
 * @brief 写入数据到数据到EEPROM
 * @param slave     I2C设备地址
 * @param start_addr 起始地址
 * @param buf     写入得数据缓冲区
 * @param count   写入数据大小
 * @return 非0 表示成功写入的数据数量，-1 表示失败
 */
int I2C_Write(unsigned char slave, unsigned char start_addr, char *buf, int count)
{
    int i,ack;
    // 地址越界保护
    int cnt = 0;
    //if(start_addr + count > I2C_MAX_ADDR)int
    //    count = I2C_MAX_ADDR - start_addr;  

    I2C_Start();                  // I2C起始
    I2C_Write8(slave);       // 设备地址+写
    I2C_Ack();                    // 等待应答
    I2C_Write8(start_addr);  // 写入起始地址

    ack = I2C_Ack();
    if(ack == 1)
        return -1;

    // 连续写入多字节
    for(i = 0; i < count; i++)
    {
        I2C_Write8(buf[i]);
        ack = I2C_Ack();
        if(ack == 1)
            return -1;
        cnt++;
    }
    I2C_Stop();

    return cnt;
}

/**
 * @brief 从EEPROM读取数据
 * @param slave     I2C设备地址
 * @param start_addr 起始地址
 * @param buf     存储读取数据的缓冲区
 * @param count   要读取的数据大小  
 * @return n 读取的数据大小，-1 表示失败
 */
int I2C_Read(unsigned char slave, unsigned char start_addr, char *buf, int count)
{
//    char ack;
    int i,cnt=0;

    // 1. 设置读取地址
    I2C_Start();

    I2C_Write8(slave); // 设备地址+写
    //ack = I2C_Ack();   
    if(I2C_Ack() == 1)
        return -1;
    // 2. 写入起始地址
    I2C_Write8(start_addr);
    // ack = I2C_Ack();
    if(I2C_Ack() == 1)
        return -1;

    // 3. 重启 + 读命令
    I2C_Start();
    I2C_Write8(slave | 0x01); // 设备地址+读
	
    if(I2C_Ack() == 1)
       return -1;   

    // 4. 连续读取多字节
    for(i = 0; i < count; i++)
    {
        buf[i] = I2C_Read8();

        // 最后一个字节 发送NACK，其余发送ACK
        if(i == count - 1)
            I2C_SendAck(1);     // 非应答
        else
            I2C_SendAck(0);     // 应答
        cnt++;
    }
    // 5. 停止信号
    I2C_Stop();   

    return cnt;
}

// 以下是测试代码
// #include "uart.h"
// #define AT24C02_ADDR 0xA0
// void Test()
// {
//     char writeBuf[16] = "Hello, I2C!";
//     char readBuf[16] = {0};

//     uart_init();
// //    I2C_Init();
//     I2C_Write(AT24C02_ADDR, 0, writeBuf, 16);
//     delay_ms(10);
//     // delay_ms(1000);
//     I2C_Read(AT24C02_ADDR, 0, readBuf, 16);
//     uart_send_str("Read from I2C: ");
//     uart_send_str(readBuf);
//     //delay_ms(1000);
//     while(1);
// }

// void main(void)
// {
//    Test();
// }