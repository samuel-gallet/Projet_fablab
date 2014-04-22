String color = "green";
int led1, led2, led3, led4;
int inputPin;

void setup ()
{
  inputPin = 5;
  led1 = 1;
  led2 = 2;
  led3 = 3;
  led4 = 4;
  Serial.begin(9600);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
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
    while ((str = strtok_r(p, ";", &p)) != NULL)
    {
      if (strstr(str, "blue") > 0)
      {
        digitalWrite(led1, HIGH);
      } else if (strstr(str, "green") > 0)
      {      
        digitalWrite(led2, HIGH);
      } else if (strstr(str, "yellow") > 0)
      {      
        digitalWrite(led3, HIGH);
      } else if (strstr(str, "white") > 0)
      {      
        digitalWrite(led4, HIGH);
      }
    }
  }
}
