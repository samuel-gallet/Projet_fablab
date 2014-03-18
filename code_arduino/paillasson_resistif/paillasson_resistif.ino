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
  
  if (val > 1000) 
    analogWrite(led, 0) ;
  else 
    analogWrite(led, val) ;

  Serial.println(val);
  delay(8);
}




// manageLed(float val)
//{
//  if (val > 1000) 
//    digitalWrite(led, LOW) ;
   // return 0;
//  else 
//    digitalWrite(led, HIGH) ;
   // return val;
//}
