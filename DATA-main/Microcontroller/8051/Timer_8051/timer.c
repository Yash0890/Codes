#include<reg51.h>
#include<lcd.h>

// Increment Switch & Led
sbit inc_sw = P1^0;
sbit inc_led = P1^4;

// Decrement Switch & Led
sbit dec_sw = P1^1; 
sbit dec_led = P1^5;

// Start Switch & Led
sbit start_sw = P1^2;
sbit start_led= P1^6;


void main()
{
/*	m_o = old mint
	s_o = old second
	s = second
	m = mint
*/
	
	unsigned int m_o,s_o,m=1,s=10;

	// lcd start function
	lcd_start();

	while(1)
	{
		// 1st line 1st position
		lcd_cmd(0x80);
		delay(2);
		lcd_string("Timer");

		// 2nd line 1st position
		lcd_cmd(0xC0);
		delay(2);
		lcd_val(m);

		lcd_string(":");

		lcd_val(s);

		if(inc_sw==0)
		{
			inc_led=1;
			s=s+10;
			if(s>60)
			{
				m++;
				s=10;	
			}
			delay(500);
		}
		else
		{
			inc_led=0;
		}

		if(dec_sw==0)
		{
			dec_led=1;
			s=s-10;
			if(s<1)
			{
				m--;
				s=60;
				lcd_cmd(0x01);
			}
			delay(500);
		}
		else
		{
			dec_led=0;
		}

		if(start_sw==0)
		{
			s_o=s;
			m_o=m;

			while(s!=0)
			{
				start_led=1;

				lcd_cmd(0x80);
				delay(2);

				lcd_string("Timer Started");

				lcd_cmd(0xC0);
				delay(2);
				lcd_val(m);

				lcd_string(":");

				lcd_val(s);

				delay(990);

				s--;

				if(m>=1&&s<=0)
				{
					m--;
					s=60;
				}

				lcd_cmd(0x01);		
			}
			m=m_o;
			s=s_o;
			delay(2);			
		}
		start_led=0;	
	}
}
