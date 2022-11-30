#include <lpc214x.h>

void delay_ms(int j)
{
	int x,i;
	for(i=0;i<j;i++)
	{
    for(x=0; x<6000; x++);    /* loop to generate 1 milisecond delay with Cclk = 60MHz */
	}
}

int main (void)
{
  int value;
	int i;
	i = 0;
	PINSEL1 = 0x00080000;	/* P0.25 as DAC output */

	while(1)
	{		
		value = 1023;
		DACR = ( (1<<16) | (value<<6) );
		delay_ms(100);
		value = 0;
		DACR = ( (1<<16) | (value<<6) );
		delay_ms(100);	
	}
}