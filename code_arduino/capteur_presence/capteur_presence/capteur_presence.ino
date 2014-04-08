int val;
int capteur = 3;
boolean enAttente;
unsigned long curr_time, prev_time;
int personne;


void setup ()
{
  Serial.begin(9600);
  pinMode(capteur, INPUT);
  enAttente = false;
  curr_time = 0;
  prev_time = 0;
  val = 0;
  personne = 0;

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
      if (strstr(str, "attente capteur") > 0)
      {
        enAttente = true;
      }/* else
      {      
        enAttente = false;
      }*/
    }
  }
 // delay(500);
  curr_time = millis(); 
  if (enAttente)
  {
    prev_time = curr_time;
    curr_time = millis();
    while (prev_time + 10000 > curr_time)
    {
      curr_time = millis();
      val = digitalRead(capteur);
      if (val == 1) {
        personne ++;
        for(int i = 0; i < 5; i++)
          sendMessage("Personne=", personne);
        break;
      }
    }
    enAttente = false;

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
