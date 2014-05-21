#include <VirtualWire.h>

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
  val = 0;
  // Initialise the IO and ISR
  //vw_set_tx_pin(transmit_pin);
  vw_set_ptt_pin(transmit_en_pin);
  // Débit (2000b/s)
  vw_setup(2000) ;
  vw_set_rx_pin(receive_pin);
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
  
  val = digitalRead(capteur);
  if (val == 1) {
    char msg[8] = {'c','a','p','t','e','u','r',';'};
    vw_send((uint8_t *)msg, 8);  
    vw_wait_tx(); // Wait until the whole message is gone
    delay(1000);
    if (vw_get_message(buf, &buflen)) // Non-blocking
    {
      manageCommand((char *)buf);
    }
  } else {
    digitalWrite(ledPluie, LOW);
    digitalWrite(ledSoleil, LOW);
    digitalWrite(ledNuage, LOW);
  }  
}

void manageCommand(char * command) 
{
  if (strstr(command, "blue")) 
  {
    digitalWrite(ledPluie, HIGH);
  }
  else if (strstr(command, "yell"))
  {
    digitalWrite(ledSoleil, HIGH);
  }
  else if (strstr(command, "whit"))
  {
    digitalWrite(ledNuage, HIGH);
  }
}