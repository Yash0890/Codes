#include<lpc214x.h>

unsigned int adc_value;
unsigned int adc(int);

void lcd_init(void);
void lcd_cmd(unsigned char cmd);
void lcd_data(unsigned char data);
void lcd_show(unsigned char *s);
void delay(int);

unsigned int adc(int ch)
{
	AD0CR=0x00200600|(1<<ch);		  //select channel
	AD0CR|=(1<<24);				  //start conversion
	while((AD0GDR& (1<<31))==0);
	adc_value =AD0GDR;
	adc_value =(adc_value >> 6) & 0x03FF; 					 // bit 6:15 is 10 bit AD value
	return adc_value;
}

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
	while(1)
	{
		lcd_cmd(0x8b);
		adc_value=adc(6);
		lcd_data((adc_value/1000)+48);
		lcd_data((adc_value/100)%10+48);
		lcd_data((adc_value/10)%10+48);
		lcd_data((adc_value%10)+48);
	}
}