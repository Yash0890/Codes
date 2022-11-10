#include <lpc214x.h>
#include <stdint.h>

int main(void)
{	
	IO0DIR = 0x000000FF;		/* Make P0.0 to P0.7 bit as output bit, P0.8 bit as an input pin  */
	while(1)
	{		
		if ( IO0PIN & (1<<8) )	/* If switch is open, pin is HIGH */
		{
			IO0SET = 0x00000055;	
			IO0CLR = 0x000000AA;  /* Turn on LED */
		}
		else /* If switch is closed, pin is LOW */
		{
			IO0SET = 0x000000AA;	/* Turn off LED */
			IO0CLR = 0x00000055;
		}		
	}	
}