/**
 * @file leds.c
 * @brief LED流水灯控制程序
 *
 * 实现LED流水灯的上下移动点亮功能
 */

#include <reg52.h>

#include "common.h"



#define LED P2
sbit KEY0 = P3^0; 
sbit KEY1 = P3^1; 
sbit KEY2 = P3^2; 
sbit KEY3 = P3^3; 

void DelayMS(uint x);
void LED_Down(uint x);
void LED_Up(uint x);
void LED_Flash(uint x);

void main()
{
    LED_Down(3);
    LED_Up(3);
    while(1)
    {        
        LED_Flash();
    }
}

/**
 * @brief 单只LED依次灯下移点亮
 * @param x 下移点亮x圈
 */
void LED_Down(uint x)
{
    uchar i, j;
    for(i = 0; i < x; i++)
    {
        LED = 0xfe;
        for(j = 0; j < 8; j++)
        {
            DelayMS(250);
            LED = (LED << 1) | 0x01;
        }
    }
}

/**
 * @brief 单只LED依次灯上移点亮
 * @param x 上移点亮x圈
 */
void LED_Up(uint x)
{
    uchar i, j;
    for(i = 0; i < x; i++)
    {
        LED = 0x7f;
        for(j = 0; j < 8; j++)
        {
            DelayMS(250);
            LED = (LED >> 1) | 0x80;
        }
    }
}

/**
 * @brief 延时函数
 * @param x 延时毫秒数
 */
void DelayMS(uint x)
{
    uint j, k;
    for(k = 0; k < x; k++)
        for(j = 0; j < 120; j++);
}

/**
 * @brief LED闪烁函数
 * @param x 闪烁次数
 */
void LED_Flash(uint x)
{
    uchar i;
    for(i = 0; i < x; i++)
    {
        LED = 0x00;
        DelayMS(250);
        LED = 0xff;
        DelayMS(250);
    }
}