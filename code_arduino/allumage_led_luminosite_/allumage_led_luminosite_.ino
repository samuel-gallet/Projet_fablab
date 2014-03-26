float val;
int led = 9;
int luminosite= 0; // pin analog A0
boolean active;


void setup ()
{
  Serial.begin(9600);
  pinMode(luminosite, INPUT);
  pinMode(led, OUTPUT);
  active = false;
}

void loop()
{
  char command[500];
  
  if (Serial.readBytes(command, 500) == 0)
  {
    command == NULL;
  } else 
  {
    char *p = command;
    char *str;
  
    while ((str = strtok_r(p, ";", &p)) != NULL)
    {
      if (strstr(str, "switch on") > 0)
      {
        active = true;
      } else if (strstr(str, "switch off") > 0)
      {
        active = false;
        analogWrite(led, 0);
      }
    }
  }
  
  if (active)
  {
    delay(200);
    sendMessage("Luminosity=", manageLed(val/4));
    val = analogRead(luminosite);
    analogWrite(led, manageLed(val/4));
  }
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
