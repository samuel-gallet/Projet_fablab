float val;
int led = 9;
int resistance= A0; // pin analog A0


void setup ()
{
  Serial.begin(9600);
  
  pinMode(resistance, INPUT);
  pinMode(led, OUTPUT);
}

void loop()
{ 
  val = analogRead(resistance);
  
  if(val > 600) 
  {
    analogWrite(led, 0);
  } else 
  {
    delay(500);
    sendMessage("Poids=", val);
    delay(5000);
    analogWrite(led, val);
  }
  delay(100);
}


void sendMessage(char *key, int value)
{
  Serial.print(key);
  Serial.print(value);
  Serial.println(";");
  Serial.flush();
}
