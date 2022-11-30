#include <lpc21xx.h>

void lcd_init(void);
void lcd_cmd(unsigned char cmd);
void lcd_data(unsigned char data);
void lcd_show(unsigned char *s);
void delay(int);

void lcd_init()
{
	lcd_cmd(0x38);
	lcd_cmd(0x0e);
	lcd_cmd(0x01);
	lcd_cmd(0x06);
	lcd_cmd(0x0c);
	lcd_cmd(0x80);
}

void lcd_cmd(unsigned char cmd)
{
	IO1CLR=0xFF070000;
	IO1SET=(cmd<<24);
	IO1CLR=(1<<16);				//rs=0
	IO1CLR=(1<<17);				//rw=0
	IO1SET=(1<<18);			  	//en=1
	delay(5);
	IO1CLR=(1<<18);			   	//en=0
}

void lcd_data(unsigned char data)
{
	IO1CLR=0xFF070000;
	IO1SET=(data<<24);
	IO1SET=(1<<16);				//rs=1
	IO1CLR=(1<<17);				//rw=0
	IO1SET=(1<<18);			   	//en=1
	delay(2);
	IO1CLR=(1<<18);			   	//en=0
}

void lcd_show(unsigned char *s)
{
	while(*s) {
		lcd_data(*s++);
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

int main()
{
	IO1DIR = 0xFFFFFFFF;
	IO0DIR = 0x00000000;
	PINSEL0 = 0X0300;
	VPBDIV = 0X02;
	lcd_init();
	lcd_show("ADC VALUE");
	while(1);
}