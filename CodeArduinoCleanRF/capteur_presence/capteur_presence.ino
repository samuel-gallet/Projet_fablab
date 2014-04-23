#include <VirtualWire.h>

int val;
int capteur = 3;
//const int transmit_pin = 12;
const int transmit_en_pin = 12;

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

}

/*

Fonction qui envoie la chaine de caractère "capteur" quand quelqu'un est sur le
paillasson. (Que la personne entre ou sort)

*/

void loop()
{
  val = digitalRead(capteur);
  if (val == 1) {
    char msg[7] = {'c','a','p','t','e','u','r'};
    vw_send((uint8_t *)msg, 7);  
    vw_wait_tx(); // Wait until the whole message is gone
    break;
  }  
}    


/* Note à enlever à la fin du projet :
   
   C'est OpenHab qui gère les données recues. (grace au recepteur branché sur l'ordi)
   Grace au bundle, OpenHAB saura quelle donnée il a recu en premier
   (paillasson puis Capteur ou l'inverse)
   Les cartes arduino ne sont plus que des emetteurs.
   
*/
