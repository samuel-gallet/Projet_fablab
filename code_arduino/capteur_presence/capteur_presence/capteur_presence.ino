#include <VirtualWire.h>

int val;
int capteur = 3;
boolean enAttente;
unsigned long curr_time, prev_time;
//const int transmit_pin = 12;
const int transmit_en_pin = 12;

void setup ()
{
  Serial.begin(9600);
  pinMode(capteur, INPUT);
  enAttente = false;
  curr_time = 0;
  prev_time = 0;
  val = 0;
 // Initialise the IO and ISR
    //vw_set_tx_pin(transmit_pin);
    vw_set_ptt_pin(transmit_en_pin);
    // Débit
    vw_setup(2000) ;

}

void loop()
{
  // Capteur entrée
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
      }
    }
  }
 // delay(500);
  curr_time = millis(); 
  if (true)//enAttente)
  {
    prev_time = curr_time;
    curr_time = millis();
    while (prev_time + 5000 > curr_time)
    {
      curr_time = millis();
      val = digitalRead(capteur);
      if (val == 1) {
        char msg[11] = {'p','e','r','s','o','n','n','e',' ','+','1'};
        vw_send((uint8_t *)msg, 11);  
        vw_wait_tx(); // Wait until the whole message is gone
        break;
      }
    }
    enAttente = false;
  } 
  
  // Capteur sortie
  val = digitalRead(capteur);
  if(val == 1) 
  {
    delay(500);
    Serial.println("attente sortie");
    delay(5000);
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
