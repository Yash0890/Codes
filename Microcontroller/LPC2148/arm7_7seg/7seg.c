#include <lpc21xx.h>
unsigned char a[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
int clk,a_clk;
void delay(unsigned int i)
{
	unsigned int j,k;
	for(j = 0; j<=i;j++)
	{
		for(k=0;k<=1275;k++);
	}
}
int main()
{
	IO0DIR |= 0x000000ff;
	IO0CLR |= 0x000000ff;
	while(1)
	{
		
		for(clk =0 ; clk <=9;clk++)
		{
			IO0SET |= a[clk];
			delay(500);
			IO0CLR |= a[clk];
		}
		
		for(a_clk =9 ; a_clk>=0;a_clk--)
		{
			IO0SET |= a[a_clk];
			delay(500);
			IO0CLR |= a[a_clk];
		}
	}
}