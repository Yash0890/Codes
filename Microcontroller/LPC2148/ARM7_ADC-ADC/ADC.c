/*
  ADC in LPC2148(ARM7)
  http://www.electronicwings.com/arm7/lpc2148-adc-analog-to-digital-converter
*/

#include <lpc214x.h>
#include <stdint.h>
#include "LCD-16x2-8bit.h"
#include <stdio.h>
#include <string.h>


int main(void)
{
	uint32_t result;
	float voltage;
	char volt[18];
	LCD_Init();
	PINSEL1 = 0x01000000; /* P0.28 as AD0.1 */
    AD0CR = 0x00200402; /* ADC operational, 10-bits, 11 clocks for conversion */
	while(1)
	{
		AD0CR = AD0CR | (1<<24); /* Start Conversion */
		while ( !(AD0DR1 & 0x80000000) ); /* Wait till DONE */
		result = AD0DR1;
		result = (result>>6);
		result = (result & 0x000003FF);
		voltage = ( (result/1023.0) * 3.3 ); /* Convert ADC value to equivalent voltage */
		LCD_Command(0x80);
		sprintf(volt, "Voltage=%.2f V  ", voltage);
		LCD_String(volt);
		memset(volt, 0, 18);
	}
}

