#include <HRCSwitch.h>

HRCSwitch mySwitch = HRCSwitch();

float val;
int led = 9;
int luminosite= 0; // pin analog A0
boolean active;
unsigned long curr_time, prev_time;
const int TXpin = 10;


void setup ()
{
  Serial.begin(9600);
  pinMode(luminosite, INPUT);
  pinMode(led, OUTPUT);
  active = false;
  curr_time = 0;
  prev_time = 0;
  mySwitch.enableTransmit(TXpin);
}

void loop()
{
  char command[500];
  int i = 0;
  if (Serial.readBytes(command, 500) == 0)
  {
    command == NULL;
  } else 
  {
    char *p = command;
    char *str;
  /*if (Serial.available())
  {  
    while (Serial.available())
    {
      command[i] = Serial.read();
      i++;
      delay(5);
    }
    char *p = command;
    char *str;*/
    Serial.println(p);
    while ((str = strtok_r(p, ";", &p)) != NULL)
    {
      if (strstr(str, "switch on") > 0)
      {
        active = true;
        mySwitch.switchOn(2, 2);
      } else if (strstr(str, "switch off") > 0)
      {      
        active = false;
        mySwitch.switchOff(2, 2);
        analogWrite(led, 0);
      }
    }
  }
  
  if (active)
  {
    prev_time = curr_time;
    curr_time = millis();
    if (prev_time + 300 < curr_time)
    {
      sendMessage("Luminosity=", manageLed(val/4));
    }
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
