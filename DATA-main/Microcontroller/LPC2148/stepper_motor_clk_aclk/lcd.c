#include <lpc214x.h>

void delay();
void lcdcmd(unsigned char);
void lcddata(unsigned char);
void lcdint();


void delay()
{
	unsigned int i = 0;
	for(i=0;i<=1000;i++);
}

void lcdcmd(unsigned char cmd)
{
	IO0PIN &= 0x00;
	IO0PIN |= cmd;
	IO0CLR |=1<<8;
	IO0SET |=1<<10;
	delay();
	IO0CLR |=1<<10;
}


void lcddata(unsigned char data)
{
	IO0PIN &= 0x00;
	IO0PIN |= (data<<0);
	IO0SET |= 1<<8;
	IO0SET |= 1<<10;
	delay();
	IO0CLR |=1<<10;
}

void lcdint()
{
	IO0CLR |=1<<10;
	lcdcmd(0x30);
	lcdcmd(0x0C);
	lcdcmd(0x01);
	lcdcmd(0x06);
	lcdcmd(0x80);
}

void string(unsigned char *p)
{
	while(*p !='\0')
	{
		lcddata(*p++);
		
	}
}



int main()
{
	IO0DIR |= 0xFF;
	
	lcdint();
	lcdcmd(0x8A);
	string("ARYAN");
	while(1)
		
	{
		
		delay();
	}
	
}
