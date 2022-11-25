#include <lpc21xx.h>
void delay(int i);

unsigned char clockwise[4] = {0x3,0x6,0xC,0x9};
unsigned char anti_clockwise[4] = {0x9,0xC,0x6,0x3};

int step_number = 5;

int ck,ack,j =0 ;

int main()
{
	PINSEL0 = 0x00000000;
	IO0DIR |= 0x00000780;
	IO0CLR = 0x00000780;
	
	while(1)
	{
		for(ck=0; ck<step_number ;ck++)
		{
			for(j=0;j<4;j++)
			{
				IOPIN0 = clockwise[j]<<7;
				delay(2000);
			}
		}
		
		for(ack=0;ack<step_number;ack++)
		{
			for(j=0;j<4;j++)
			{
				IOPIN0 = anti_clockwise[j]<<7;
				delay(2000);
			}
		}
		
	}
}

void delay(int i)
{
	unsigned int j,k;
	for(j=0;j<=i;j++)
	{
		for(k=0;k<=1275;k++);
	}
}
