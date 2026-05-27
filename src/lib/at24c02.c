/**
 * @file at24c02.c
 * @brief AT24C02 EEPROM芯片驱动
 *
 * 实现AT24C02的读写功能，支持单字节、页写、连续读等操作
 */
 
 
#include "common.h"
#include "at24c02.h"
#include "i2c.h"


// // ==================== 地址定义 ====================

// #define ADDR_TIME    0x00  // 时间数据存储地址
// #define ADDR_TEMP    0x10  // 温度数据存储地址

/**
 * @brief AT24C02初始化
 */
void AT24C02_Init(void) {
    // I2C总线初始化（引脚默认状态）
    I2C_SDA = 1;
    I2C_SCL = 1;
}

/**
 * @brief 向AT24C02指定地址写入一个字节
 * @param addr 字节地址 (0-255)
 * @param dat 要写入的数据
 */
void AT24C02_WriteByte(unsigned char addr, unsigned char dat) {
    I2C_Start();
    I2C_Write8(AT24C02_ADDR);    // 发送设备地址（写）
    I2C_Ack();

    I2C_Write8(addr);             // 发送字节地址
    I2C_Ack();

    I2C_Write8(dat);              // 发送数据
    I2C_Ack();

    I2C_Stop();

    // 等待写入完成（AT24C02写周期约5ms）
    delay_ms(AT24C02_WRITE_CYCLE);
}

/**
 * @brief 从AT24C02指定地址读取一个字节
 * @param addr 字节地址 (0-255)
 * @return 读取的数据
 */
unsigned char AT24C02_ReadByte(unsigned char addr) {
    unsigned char dat;

    I2C_Start();
    I2C_Write8(AT24C02_ADDR);    // 发送设备地址（写）
    I2C_Ack();

    I2C_Write8(addr);             // 发送字节地址
    I2C_Ack();

    I2C_Start();
    I2C_Write8(AT24C02_ADDR | 0x01);  // 发送设备地址（读）
    I2C_Ack();

    dat = I2C_Read8();           // 读取数据

    I2C_SendAck(1);                 // 发送非应答
    I2C_Stop();

    return dat;
}

/**
 * @brief 向AT24C02连续写入多个字节（自动处理跨页）
 * @param addr 起始地址
 * @param buf 数据缓冲区
 * @param len 写入长度
 */
void AT24C02_WritePage(unsigned char addr, const unsigned char *buf, unsigned char len) {
    unsigned char i;
    unsigned char pageStart;
    unsigned char pageRemain;

    while(len > 0) {
        // 计算当前页起始地址和剩余空间
        pageStart = addr & 0xF8;              // 页起始地址（AT24C02每页8字节）
        pageRemain = AT24C02_PAGE_SIZE - (addr - pageStart);

        // 限制单次写入长度
        if(len < pageRemain) {
            pageRemain = len;
        }

        // 写入一页数据
        I2C_Start();
        I2C_Write8(AT24C02_ADDR);
        I2C_Ack();

        I2C_Write8(addr);
        I2C_Ack();

        for(i = 0; i < pageRemain; i++) {
            I2C_Write8(buf[i]);
            I2C_Ack();
        }

        I2C_Stop();

        // 等待写入完成
        delay_ms(AT24C02_WRITE_CYCLE);

        // 更新指针和计数
        buf 	+= pageRemain;
        addr 	+= pageRemain;
        len 	-= pageRemain;
    }
}

/**
 * @brief 从AT24C02连续读取多个字节
 * @param addr 起始地址
 * @param buf 数据缓冲区
 * @param len 读取长度
 */
void AT24C02_Read(unsigned char addr, unsigned char *buf, unsigned int len) {
    unsigned int i;

    I2C_Start();
    I2C_Write8(AT24C02_ADDR);
    I2C_Ack();

    I2C_Write8(addr);
    I2C_Ack();

    I2C_Start();
    I2C_Write8(AT24C02_ADDR | 0x01);
    I2C_Ack();

    for(i = 0; i < len; i++) {
        buf[i] = I2C_Read8();

        if(i == len - 1) {
            I2C_SendAck(1);  // 最后一个字节发送非应答
        } else {
            I2C_SendAck(0);  // 发送应答
        }
    }

    I2C_Stop();
}


// /**
//  * @brief AT24C02读写测试
//  */

// #include "uart.h"

// void Test(void) {
//     unsigned char i;
//     unsigned char writeBuf[16];
//     unsigned char readBuf[16];

//     uart_init(); // 初始化串口用于调试输出
//     // 初始化
//     AT24C02_Init();

//     // 准备测试数据
//     for(i = 0; i < 16; i++) {
//         writeBuf[i] = 'a'+i;
//     }

//     // 写入数据
//     AT24C02_WritePage(0x00, writeBuf, 16);

//     // 读取数据
//     AT24C02_Read(0x00, readBuf, 16);

//     // 验证数据（可在调试模式下检查readBuf内容）
//     uart_send_str("Read data: ");
//     for(i = 0; i < 16; i++) {
//         uart_send_byte(readBuf[i]);
//     }
//     uart_send_str("hello world\n");
//     while(1);
// }

// void main(void) {
//     Test();   
// }
