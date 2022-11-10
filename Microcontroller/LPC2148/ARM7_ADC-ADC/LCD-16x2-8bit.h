/*
  16x2 LCD library for LPC2148
  http://www.electronicwings.com/
*/

/* P0.8-P0.15 connected to LCD data pins */
/* P0.4,5,6 connected to RS, RW, EN */

#ifndef BIT8LCD16x2_H
#define BIT8LCD16x2_H


#include <lpc214x.h>
#include <stdint.h>

void delay_ms(uint16_t j);
void LCD_Command(char command);
void LCD_Init(void);
void LCD_Char (char msg);
void LCD_String (char* msg);
void LCD_Data(int8_t data);

#endif

