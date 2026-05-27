#include "uart.h"

void uart_init()
{
    SCON =  0x50;
    //TMOD = (TMOD & 0x0f) | 0x20;
      TMOD = 0x20;
    TH1 = 0xFD;           //11.059Mhz ,bps = 9600
    TL1 = 0xFD;
    TR1 = 1;
}

void uart_send_byte(unsigned char c)
{
      SBUF = c;
    while(!TI);
    TI = 0;
    
}
unsigned char uart_recv_byte()
{
    while(!RI);
    RI = 0;
    return SBUF;
}

void uart_send_str(char *str)
{
    while(*str)
    {
        uart_send_byte(*str++);
    }
}


// // 以下是测试代码

    
// void Test()
// {   
//     uart_init();
//     while(1)
//     {
//         uart_send_str("-> hello world!\n");
             
              
//         if(RI == 1) 
//         {
//             uart_send_str("<- ");
//             uart_send_byte(SBUF); // Echo received byte back
//             RI = 0; // Clear receive interrupt flag
//             uart_send_str(" \n");
//         }
//         delay_ms(1000);
//     }
    
// }


// void main()
// {
//     Test();
    
// }

