float val;
int led = 9;
int luminosite= 0; // pin analog A0
boolean donneesTraitees;
char buf[20];
int carlu;

void setup ()
{
  Serial.begin(9600);
  pinMode(luminosite, INPUT);
  pinMode(led, OUTPUT);
  donneesTraitees = true;
  carlu = 0;
}

void loop()
{
  /*if (Serial.available() > 0)
  {
    int i = 0;
    carlu = Serial.available();
    while (carlu > 0)
    {
      buf[i] = Serial.read();
      delay(2);
      carlu = Serial.available();
      i++;
    }
    if (strcmp(buf, "ok"))
      donneesTraitees = true;
    for(int i = 0; i < 20; i++)
    {
      buf[i] = '\0';
    }
  }*/
  delay(500);
  //if (donneesTraitees) 
  //{
    sendMessage("Luminosity=", manageLed(val/4));
    donneesTraitees = false;
  //}
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

void sendMessage(char *key, int value)
{
  Serial.print(key);
  Serial.print(value);
  Serial.println(";");
  Serial.flush();
}
