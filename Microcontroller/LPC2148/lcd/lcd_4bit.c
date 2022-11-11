#include <lpc21xx.h>

#define LCD_PINS (0xff<<16)
#define LCD_RS (1<<13)
#define LCD_RW (1<<14)
#define LCD_EN (1<<15)

void delay (unsigned int delay)
{
	unsigned int i,j;
	for(i=0;i<=delay;i++)
	{
		for(j=0;j<=1275;j++);
	}
}

void lcd_init(void);
void lcd_show(char *s);
void lcd_cmd(unsigned char cmd);
void lcd_data(unsigned char dat);

void lcd_init(void)
{
	delay(5);
	lcd_cmd(0x38); // 2 lines 
	lcd_cmd(0x0e); // display on cursor blinking
	
	lcd_cmd(0x01); // clear display
	lcd_cmd(0x06); // shift to right
	
	lcd_cmd(0x80); // force cursor to start form beginning of 1st line
}	

void lcd_cmd(unsigned char cmd)
{
	IO0CLR |= (LCD_RS|LCD_RW|LCD_EN|LCD_PINS);
	
	
	
	IO0SET |= ((cmd & 0XF0)<<16);
	
	IO0CLR |= LCD_RS;
	IO0CLR |= LCD_RW;
	IO0SET |= LCD_EN;
	
	delay(2);
	IO0CLR |= LCD_EN;
	delay(3);
	
	IO0CLR |= (LCD_RS|LCD_RW|LCD_EN|LCD_PINS);
	IO0SET |= (((cmd & 0X0F)<<4)<<16);
	
	IO0CLR |= LCD_RS;
	IO0CLR |= LCD_RW;
	IO0SET |= LCD_EN;
	
	delay(2);
	IO0CLR |= LCD_EN;
	delay(3);
}

void lcd_data(unsigned char dat)
{
	IO0CLR |= (LCD_RS|LCD_RW|LCD_EN|LCD_PINS);
	
	
	
	IO0SET |= (( dat & 0XF0)<<16);
	
	IO0SET |= LCD_RS;
	IO0CLR |= LCD_RW;
	IO0SET |= LCD_EN;
	
	delay(2);
	IO0CLR |= LCD_EN;
	delay(3);
	
	IO0CLR |= (LCD_RS|LCD_RW|LCD_EN|LCD_PINS);
	IO0SET |= ((( dat & 0X0F)<<4)<<16);
	
	IO0SET |= LCD_RS;
	IO0CLR |= LCD_RW;
	IO0SET |= LCD_EN;
	
	delay(2);
	IO0CLR |= LCD_EN;
	delay(3);
}

void lcd_show(char *s)
{
	while(*s!='\0')
	{
		lcd_data(*s);
		s++;
	}
}

int main()
{
	IO0DIR = 0XFFFFFFFF;
	lcd_init();
	lcd_show("Hello ARM-7");
	delay(5000);
}