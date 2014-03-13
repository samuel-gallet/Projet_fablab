float val;
int led = 9;
int luminosite= 0; // pin analog A0

void setup ()
{
  pinMode(luminosite, INPUT);
  pinMode(led, OUTPUT);
}

void loop()
{
  val = analogRead(luminosite);
  analogWrite(led, manageLed(val/4));
  delay(50);
}

int manageLed(float val)
{
  if (val > 155) 
    return 0;
  else 
    return 155 - val;
}
