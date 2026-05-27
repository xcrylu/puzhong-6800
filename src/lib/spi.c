/**
 * @file spi.c
 * @brief SPI总线驱动
 *
 * 实现通用SPI通信协议，支持模式配置和MSB/LSB选择
 */

#include "spi.h"

static unsigned char spi_mode;     // SPI模式
static unsigned char spi_lsb_first; // 位序标志

/**
 * @brief SPI微秒级延时
 * @param x 延时时间
 */
void SPI_DelayUs(unsigned char x) {
    while(x--) {
        ;
    }
}

/**
 * @brief SPI初始化
 * @param mode SPI模式 (0-3)
 * @param lsb_first 是否低位在前 (0: MSB, 1: LSB)
 */
void SPI_Init(unsigned char mode, unsigned char lsb_first) {
    spi_mode = mode;
    spi_lsb_first = lsb_first;

    // 初始化引脚状态
    SPI_SS = 1;     // 取消选中从机
    SPI_SCLK = 0;   // 根据模式设置初始时钟电平
    SPI_MOSI = 0;
}

/**
 * @brief SPI写一个字节 (Mode 1: CPOL=0, CPHA=1)
 * @param dat 要发送的数据
 * @return 从机返回的数据
 */
unsigned char SPI_WriteByte(unsigned char dat) {
    unsigned char i;
    unsigned char recv = 0;

    if(spi_lsb_first) {
        // LSB在前模式
        for(i = 0; i < 8; i++) {
            SPI_SCLK = 0;
            SPI_DelayUs(1);

            SPI_MOSI = (dat & 0x01);
            dat >>= 1;

            SPI_SCLK = 1;
            SPI_DelayUs(1);

            recv >>= 1;
            if(SPI_MISO) {
                recv |= 0x80;
            }
        }
    } else {
        // MSB在前模式
        for(i = 0; i < 8; i++) {
            SPI_SCLK = 0;
            SPI_DelayUs(1);

            SPI_MOSI = (dat & 0x80);
            dat <<= 1;

            SPI_SCLK = 1;
            SPI_DelayUs(1);

            recv <<= 1;
            if(SPI_MISO) {
                recv |= 0x01;
            }
        }
    }

    return recv;
}

/**
 * @brief SPI读取一个字节 (Mode 1: CPOL=0, CPHA=1)
 * @return 读取的数据
 */
unsigned char SPI_ReadByte(void) {
    unsigned char i;
    unsigned char dat = 0;

    if(spi_lsb_first) {
        // LSB在前模式
        for(i = 0; i < 8; i++) {
            SPI_SCLK = 1;
            SPI_DelayUs(1);

            SPI_SCLK = 0;
            SPI_DelayUs(1);

            dat >>= 1;
            if(SPI_MISO) {
                dat |= 0x80;
            }
        }
    } else {
        // MSB在前模式
        for(i = 0; i < 8; i++) {
            SPI_SCLK = 1;
            SPI_DelayUs(1);

            SPI_SCLK = 0;
            SPI_DelayUs(1);

            dat <<= 1;
            if(SPI_MISO) {
                dat |= 0x01;
            }
        }
    }

    return dat;
}

/**
 * @brief SPI同时读写一个字节 (Mode 1: CPOL=0, CPHA=1)
 * @param dat 要发送的数据
 * @return 读取到的数据
 */
unsigned char SPI_Transfer(unsigned char dat) {
    unsigned char i;
    unsigned char recv = 0;

    if(spi_lsb_first) {
        // LSB在前模式
        for(i = 0; i < 8; i++) {
            SPI_SCLK = 0;
            SPI_DelayUs(1);

            SPI_MOSI = (dat & 0x01);
            dat >>= 1;

            SPI_SCLK = 1;
            SPI_DelayUs(1);

            recv >>= 1;
            if(SPI_MISO) {
                recv |= 0x80;
            }
        }
    } else {
        // MSB在前模式
        for(i = 0; i < 8; i++) {
            SPI_SCLK = 0;
            SPI_DelayUs(1);

            SPI_MOSI = (dat & 0x80);
            dat <<= 1;

            SPI_SCLK = 1;
            SPI_DelayUs(1);

            recv <<= 1;
            if(SPI_MISO) {
                recv |= 0x01;
            }
        }
    }

    return recv;
}
