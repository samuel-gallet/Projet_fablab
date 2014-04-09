float val;
int led = 9;
int resistance= A0; // pin analog A0
boolean enAttente;
unsigned long curr_time, prev_time;


void setup ()
{
  Serial.begin(9600);
  pinMode(resistance, INPUT);
  pinMode(led, OUTPUT);
  enAttente = false;
  curr_time = 0;
  prev_time = 0;
}

void loop()
{ 
  // Détection poids entrée
  val = analogRead(resistance);
  
  if(val > 550) 
  {
    analogWrite(led, 0);
  } else 
  {
    delay(500);
    sendMessage("Poids=", val);
    delay(1000);
    analogWrite(led, val);
  }
  
  delay(100);
  
  // Détection poids sortie
  char command[500];
  //int i = 0;
  if (Serial.readBytes(command, 500) == 0)
  {
    command == NULL;
  } else 
  {
    char *p = command;
    char *str;
    while ((str = strtok_r(p, ";", &p)) != NULL)
    {
      if (strstr(str, "attente paillasson") > 0)
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
    while (prev_time + 5000 > curr_time)
    {
      curr_time = millis();
      val = analogRead(resistance);
      if (val < 550) {
        // personne --;
        Serial.println("sortie de la maison");
        break;
      }
    }
    enAttente = false;

  } 
  
  
  
  
}


void sendMessage(char *key, int value)
{
  Serial.print(key);
  Serial.print(value);
  Serial.println(";");
  Serial.flush();
}
