const int s0 = D4;
const int s1 = D5;
const int s2 = D6;
const int s3 = D7;
const int out = D8;  
int red = 0; 
int green = 0;
int blue = 0;  
void setup() 
{    
  Serial.begin(9600);  
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(out, INPUT);
  digitalWrite(s0, HIGH);
  digitalWrite(s1, HIGH);
}   
void loop()
{   
  color();
  Serial.print("\n\n\n\n\n"); 
  delay(3000);     
}
 
void nitrogen()
{
  /////// NITROGEN ///////////
  if(445 < blue < 490)
  {
    Serial.print("Nitrogen = "); 
    Serial.print(blue,DEC);
    
    if(445 <= blue <455)
    {
      Serial.println(" - Low");
    }
    else if (456 <= blue <=474 )
    {
      Serial.println(" - Normal");
    }
    else if (475 <= blue <= 490)
    {
      Serial.println(" - High");
    }
  }
  else
  {
    Serial.println();
  }
}

void potassium()
{
  ////////// Potassium //////////
  if(620< red < 700)
  {
    Serial.print("Potassium = "); 
    Serial.print(red,DEC);
      
    if(620 <= red <= 640)
    {
      Serial.println(" - Low");
    }
    else if (641 <= red <= 675)
    {
      Serial.println(" - Normal");
    }
    else if (676 <= red <= 700)
    {
      Serial.println(" - High");
    }
  }
  else
  {
    Serial.println();
  }
}

void phosphorus()
{
  ///////// Phosphorus //////////
  if(500 < green < 570)
  {
    Serial.print("Phosphorus = "); 
    Serial.print(green,DEC);
    
    if(500 <= green <= 525)
    {
      Serial.println(" - Low");
    }
    else if (526 <= green <= 550)
    {
      Serial.println(" - Normal");
    }
    else if (551 <= green <= 570)
    {
      Serial.println(" - High");
    }
  }
  else
  {
    Serial.println();
  }
}

void color() 
{     
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);    //count OUT, pRed, RED
  red = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  potassium();
  delay(50);
    
  digitalWrite(s3, HIGH);  //count OUT, pBLUE, BLUE   
  blue = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  nitrogen();
  delay(50);
  
  digitalWrite(s2, HIGH);    //count OUT, pGreen, GREEN   
  green = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  phosphorus(); 
  delay(50);
}
