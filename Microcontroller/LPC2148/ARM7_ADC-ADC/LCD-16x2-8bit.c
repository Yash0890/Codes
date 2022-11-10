/*
  16x2 LCD library for LPC2148
  http://www.electronicwings.com/
*/

#include "LCD-16x2-8bit.h"


void delay_ms(uint16_t j) /* Function for delay in milliseconds  */
{
    uint16_t x,i;
	for(i=0;i<j;i++)
	{
    for(x=0; x<6000; x++);    /* loop to generate 1 millisecond delay with Cclk = 60MHz */
	}
}

void LCD_Command(char command)
{
	IO0PIN = ( (IO0PIN & 0xFFFF00FF) | (command<<8) );
	IO0SET = 0x00000040; /* EN = 1 */
	IO0CLR = 0x00000030; /* RS = 0, RW = 0 */
	delay_ms(2);
	IO0CLR = 0x00000040; /* EN = 0, RS and RW unchaned(i.e. RS = RW = 0) */
	delay_ms(5);
}

void LCD_Init(void)
{
	IO0DIR = 0x0000FFF0; /* P0.8 to P0.15 LCD Data. P0.4,5,6 as RS RW and EN */
	delay_ms(20);
	LCD_Command(0x38);  /* Initialize lcd */
	LCD_Command(0x0C);   /* Display on cursor off */
	LCD_Command(0x06);  /* Auto increment cursor */
	LCD_Command(0x01);   /* Display clear */
	LCD_Command(0x80);  /* First line first position */
}

void LCD_Char (char msg)
{
		IO0PIN = ( (IO0PIN & 0xFFFF00FF) | (msg<<8) );
		IO0SET = 0x00000050; /* RS = 1, , EN = 1 */
		IO0CLR = 0x00000020; /* RW = 0 */
		delay_ms(2);
		IO0CLR = 0x00000040; /* EN = 0, RS and RW unchanged(i.e. RS = 1, RW = 0) */
		delay_ms(5);
}

void LCD_String (char* msg)
{
	uint8_t i=0;
	while(msg[i]!=0)
	{
		LCD_Char(msg[i]);
		i++;
	}
}

void LCD_Data(int8_t data)
{
	IO0PIN = ( (IO0PIN & 0xFFFF00FF) | (data<<8) );
	IO0SET = 0x00000050; /* RS = 1, EN = 1 */
	IO0CLR = 0x00000020; /* RW = 0 */
	delay_ms(2);
	IO0CLR = 0x00000040; /* EN = 0, RS and RW unchanged(i.e. RS = 1, RW = 0) */
	delay_ms(5);
}

