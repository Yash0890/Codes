//Program to interface Matrix Keypad with LPC 2148
#include<lpc21xx.h>
#include "lcd4bit.h"
#define CLR 0x0003C000
Unsigned char check (int);
void delay(int);
int main(void)
{
                init_lcd();
                lcd_command(0x01);
                lcd_command(0x80);
                printlcd("4x4 KEYPAD");
                lcd_command(0xC0);
                printlcd("with LPC 2148");
                delay(2000);
                lcd_command(0x01);
                lcd_command(0x80);
                printlcd("Developed By:");
                lcd_command(0xC0);
                printlcd("Dishant Shah");
                delay(2000);
                lcd_command(0x01);
                lcd_command(0x80);
                printlcd("Enter Value");
                lcd_command(0xC0);
IO0DIR = 0X0003C000;
                while(1)
                {
                                                IO0CLR = CLR;
                                                IO0SET = O1;
                                                delay(10);
                                if(check(I1))
                                {
                                                lcd_data('0');
                                }
                                if(check(I2))
                                {
                                                lcd_data('1');
                                }
                                if(check(I3))
                                {
                                                lcd_data('2');
                                }
if(check(I4))
                                {
                                                lcd_data('3');
                                }
                                IO0CLR = CLR;
IO0SET = O2;
                                if(check(I1))
                                {
   lcd_data('4');
                                }
                                if(check(I2))
                                {
                                              lcd_data('5');
                                }
                                if(check(I3))
                                {             
                                                lcd_data('6');
                                }
 if(check(I4))
                                {             
                                              lcd_data('7');
                                }

IO0CLR = CLR;
IO0SET = O3;
if(check(I1))
                                {
                                               lcd_data('8');
                                }
if(check(I2))
                                {
                                                lcd_data('9');
                                }
if(check(I3))
                                {
                                                lcd_data('A');
                                }
if(check(I4))
                                {
                                                lcd_data('B');
                                }
IO0CLR = CLR;
IO0SET = O4;
if(check(I1))
                                {
                                                lcd_data('C');
                                }
if(check(I2))
                                {
                                                lcd_data('D');
                                }
if(check(I3))
                                {             
                                          lcd_data('E');
                                }
if(check(I4))
                                {             
                                                lcd_data('F');
                                }                                               
                }
 }
char check(int val)                             /* scanning a a key        */
  {
    while((IO0PIN & 0X00003C00)==val)
{
                                delay(50);
                               if((IO0PIN & 0X00003C00)== 0X00003C00)return(1);
                 }
                  return(0) ;
   }
void delay(int n)                                                               /* generates one milisecond delay  */
  {
   int i,j;
   for (i=1; i<=n; i++)
   for(j=0; j<=40000; j++);
   }