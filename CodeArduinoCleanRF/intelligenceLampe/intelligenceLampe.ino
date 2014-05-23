#include <HRCSwitch.h>

HRCSwitch mySwitch = HRCSwitch();
const int TXpin = 12;


void setup ()
{
  Serial.begin(9600);

  mySwitch.enableTransmit(TXpin);
  mySwitch.switchOff(2, 2);

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
        mySwitch.switchOn(2, 2);
      } else if (strstr(str, "switch off") > 0)
      {      
        mySwitch.switchOff(2, 2);
      }
    }
  }
  
  }   
    

