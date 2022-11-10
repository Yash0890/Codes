#include <util/delay.h>
#include <avr/io.h>
#define F_CPU 16000000
int main(){
  DDRD=0xFF;
  PORTD=0xFF;
  while(1)
  {
    int i;
    unsigned char seg[ i ]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0XF8,0X80,0x90};
    for(i=0; i<10; i++)
    {
      PORTD=seg[i];
      _delay_ms(700);
    }
  }
}