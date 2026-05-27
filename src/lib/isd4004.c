/**
 * @file isd4004.c
 * @brief ISD4004语音芯片驱动
 *
 * 实现语音播放、录制控制功能
 * 按照数据手册标准格式实现：24位命令 = 8位操作码 + 16位地址
 */

#include "isd4004.h"
#include "spi.h"

/**
 * @brief 毫秒级延时函数
 * @param x 延时毫秒数
 */
void ISD4004_DelayMs(unsigned int x) {
    unsigned int j;
    while(x--) {
        j = 82;  // 11.0592MHz时钟下约1ms延时
        while(j--);
    }
}

/**
 * @brief 初始化ISD4004语音芯片
 */
void ISD4004_Init(void) {
    // 初始化SPI总线 (Mode 1, LSB first)
    SPI_Init(SPI_MODE_1, SPI_LSB_FIRST);

    // 硬件复位芯片
    ISD4004_RST = 0;
    ISD4004_DelayMs(100);
    ISD4004_RST = 1;
    ISD4004_DelayMs(100);

    // 发送复位命令
    ISD4004_SendCmd(ISD4004_CMD_STOPWRD);
}

/**
 * @brief 向ISD4004发送8位命令
 * @param opcode 8位操作码
 *
 * 命令格式: [8位操作码]
 * SS下降沿开始传输，数据在SCLK上升沿移入
 */
void ISD4004_SendCmd(unsigned char opcode) {
    // 等待芯片就绪 (INT引脚变高表示空闲)
    while(ISD4004_INT == 0);

    SPI_SS = 0;              // 选中芯片，开始传输
    SPI_DelayUs(1);

    SPI_WriteByte(opcode);    // 发送8位操作码 (LSB在前)

    SPI_SS = 1;              // 取消选中，结束传输
    ISD4004_DelayMs(10);     // 等待命令执行
}

/**
 * @brief 读取ISD4004状态寄存器
 * @return 状态寄存器值
 *
 * 状态位:
 * - D0: OVF (溢出标志)
 * - D1: EOM (消息结束标志)
 * - D5-D0: 未使用
 */
unsigned char ISD4004_ReadStatus(void) {
    unsigned char status;

    while(ISD4004_INT == 0);

    SPI_SS = 0;
    SPI_DelayUs(1);

    ISD4004_SendCmd(ISD4004_CMD_RINT);  // 发送读状态命令

    status = SPI_ReadByte();              // 读取状态

    SPI_SS = 1;
    SPI_DelayUs(1);

    return status;
}

/**
 * @brief 从当前地址播放语音 (PLAY命令)
 */
void ISD4004_Play(void) {
    // 先上电
    ISD4004_SendCmd(ISD4004_CMD_PWR_UP);
    ISD4004_DelayMs(50);

    // 从当前地址播放
    ISD4004_SendCmd(ISD4004_CMD_PLAY);
}

/**
 * @brief 从指定地址播放语音 (SETPLAY命令)
 * @param addr 播放起始地址 (0-0xFFFF)
 */
void ISD4004_SetPlay(unsigned int addr) {
    // 先上电
    // ISD4004_SendCmd(ISD4004_CMD_PWR_UP);
    // ISD4004_DelayMs(50);

    // 设置播放地址并开始播放
    SPI_SS = 0;
    SPI_WriteByte(addr & 0xFF);
    SPI_WriteByte((addr >> 8) & 0xFF);
    SPI_WriteByte(ISD4004_CMD_SETPLAY);
    SPI_SS = 1;
}

/**
 * @brief 停止播放/录音 (STOP命令)
 */
void ISD4004_Stop(void) {
    ISD4004_SendCmd(ISD4004_CMD_STOP);
}

/**
 * @brief 从当前地址开始录音 (REC命令)
 */
void ISD4004_Record(void) {
    // 先上电
    // ISD4004_SendCmd(ISD4004_CMD_PWR_UP);
    // ISD4004_DelayMs(50);

    // 从当前地址录音
    ISD4004_SendCmd(ISD4004_CMD_REC);
}

/**
 * @brief 从指定地址开始录音 (SETREC命令)
 * @param addr 录音起始地址 (0-0xFFFF)
 */
void ISD4004_SetRec(unsigned int addr) {
    // 先上电
    // ISD4004_SendCmd(ISD4004_CMD_PWR_UP);
    // ISD4004_DelayMs(50);

    // 设置录音地址并开始录音
    SPI_SS = 0;
    SPI_WriteByte(addr & 0xFF);
    SPI_WriteByte((addr >> 8) & 0xFF);
    SPI_WriteByte(ISD4004_CMD_SETREC);
    SPI_SS = 1;
}

/**
 * @brief 复位ISD4004 (STOPWRD命令，进入低功耗模式)
 */
void ISD4004_Reset(void) {
    ISD4004_SendCmd(ISD4004_CMD_STOPWRD);
}

void Test_ISD4004(void) {
    ISD4004_Init();
    ISD4004_Reset();
    ISD4004_Record();
    ISD4004_Stop();
    ISD4004_Play();
}
