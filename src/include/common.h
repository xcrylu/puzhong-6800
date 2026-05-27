//common.h
#ifndef _COMMON_H_
#define _COMMON_H_

#include <reg52.h>
// #include "uart.h" 

typedef unsigned char uchar;

typedef unsigned char   u8;
typedef char            s8;
typedef unsigned int    u16;
typedef int             s16;
typedef unsigned long   u32;
typedef long            s32;    

void delay_ms(unsigned int);
//延时大概位13+7*us  微秒
void delay_us(unsigned char us);

#endif 
