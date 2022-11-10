#include <util/delay.h>
#include <avr/io.h>
#define F_CPU 16000000
int main(){
  DDRD=0xFF;
  PORTD=0x3F;
  while(1)
  {
    int i;
    unsigned char seg[ i ]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0X07,0X7F,0x6F};
    for(i=0; i<10; i++)
    {
      PORTD=seg[ i ];
      _delay_ms(700);
    }
  }
}