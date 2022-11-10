sbit RS = P3^7;   //Register select (RS) pin
sbit RW = P3^1;   //Read write (RW) pin
sbit EN = P3^5;   //Enable (EN) pin

// delay 
void delay(int ms)    // Time delay function
{
	int j,k;
	for(j=0;j<=ms;j++)
  		for(k=0;k<=300;k++);
}

void lcd_cmd(unsigned char z)   // Funtion to send command on LCD
{         
	P2 = z;
	EN = 1;
	RS = 0;
	RW = 0;
	delay(1);
	EN = 0;
	return;
}

void lcd_data(unsigned char x)
{
	P2 = x;
  	RW = 0;
  	RS = 1;
  	EN = 1;
  	delay(1);
  	EN = 0;
}

void lcd_string(unsigned char *s)
{
	while(*s != '\0'){
		lcd_data(*s);
		s++;
		delay(5);
	}
}

unsigned char c;
int num[10];	// array to save

void lcd_val(unsigned int i)     //Function to send data on LCD
{   
	int p;
	int k=0;
	//seperate digits of number and store in array 
	// 581 = 05    08    01
	while(i>0)
	{
  		num[k]=i%10;	 // save a digit into array
  		i=i/10;		 // remove same digit from number
  		k++;
	}
	k--;
	// send each digit to LCD
	for (p=k;p>=0;p--)
	{
  		c=num[p]+48;		// to get ASCII code of digit
  		P2 = c;
  		RW = 0;
  		RS = 1;
  		EN = 1;
  		delay(1);
  		EN = 0;
	}
	return;
}

	//Initialisation
void lcd_start()
{
	lcd_cmd(0x38);    // Configuring settings to 8-bit 2 row 
	delay(2);
	lcd_cmd(0x0E); 
	delay(2);
	lcd_cmd(0x06);    //Display on
	delay(2);
	lcd_cmd(0x80);    //Set cursor to blink at line 1 positon 1
	delay(2);
}

//shift coursor right
void coursor_r()
{
	lcd_cmd(0x14);	// Shift Cursor position to right
	delay(2); 
}

//shift coursor left
void coursor_l()
{
	lcd_cmd(0x10);	// Shift Cursor position to left
	delay(2);
}