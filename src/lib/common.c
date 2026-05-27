// 晶振：12MHz
// void delay_ms(unsigned int n)
// {
//     unsigned char a, b;
//     while(n--){
//        for(a=2; a>0; a--)
//           for(b=239; b>0; b--);
//     }
// }


// 51单片机 1ms 延时函数
// 晶振：11.0592MHz
void delay_ms(unsigned int  ms)		
{
	unsigned char j;
	while(ms--)
		for(j = 0;j<110;j++);
}

// 延时大概位13+7*us  微秒
void delay_us(unsigned char us)
{
	while(us--);        
}
