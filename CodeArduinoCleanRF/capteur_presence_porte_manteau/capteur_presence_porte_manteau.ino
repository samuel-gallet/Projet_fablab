#include <VirtualWire.h>
#include <VirtualWire_Config.h>

int val;
int capteur = 3;
//const int transmit_pin = 12;
const int transmit_en_pin = 12;
const int receive_pin = 9;
int ledSoleil = 6;
int ledNuage = 7;
int ledPluie = 8;

void setup ()
{
  Serial.begin(9600);
  pinMode(capteur, INPUT);
    pinMode(ledSoleil, OUTPUT);
    pinMode(ledPluie, OUTPUT);
    pinMode(ledNuage, OUTPUT);
  val = 0;
  // Initialise the IO and ISR
  //vw_set_tx_pin(transmit_pin);
  vw_set_ptt_pin(transmit_en_pin);
  // Débit (2000b/s)

  vw_set_rx_pin(receive_pin);
  vw_setup(2000) ;
  vw_rx_start();       // Start the receiver PLL running
}

/*

Fonction qui envoie la chaine de caractère "capteur" quand quelqu'un est sur le
paillasson. (Que la personne entre ou sort)

*/

void loop()
{
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  
  vw_wait_rx();
  if (vw_get_message(buf, &buflen)) // Non-blocking
    {
      if(!vw_have_message())
        Serial.println("le message n'est pas recu");
      Serial.println((uint8_t)*buf);
      manageCommand((char *)buf);
    }
  else {
    digitalWrite(ledPluie, LOW);
    digitalWrite(ledSoleil, LOW);
    digitalWrite(ledNuage, LOW);
  }  
  
  
  val = digitalRead(capteur);
  if (val == 1) {
    char msg[8] = {'c','a','p','t','e','u','r',';'};
    Serial.println(msg);
    vw_send((uint8_t *)msg, 8);  
    vw_wait_tx(); // Wait until the whole message is gone
    delay(1000);
  }
}

void manageCommand(char * command) 
{
  if (strstr(command, "blue")) 
  {
    Serial.println("pluie");
    digitalWrite(ledPluie, HIGH);
  }
  else if (strstr(command, "yellow"))
  {
     Serial.println("soleil");
    digitalWrite(ledSoleil, HIGH);
  }
  else if (strstr(command, "white"))
  {
     Serial.println("nuage");
    digitalWrite(ledNuage, HIGH);
  }
}
