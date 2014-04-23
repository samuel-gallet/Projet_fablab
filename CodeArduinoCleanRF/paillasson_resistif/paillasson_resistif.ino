#include <VirtualWire.h>

byte val;
int resistance= A0; // pin analog A0
const int transmit_en_pin = 12;

void setup ()
{
  Serial.begin(9600);
  pinMode(resistance, INPUT);
  // Initialise the IO and ISR
  vw_set_ptt_pin(transmit_en_pin);
  // Débit (2000b/s)
  vw_setup(2000) ;
}

void loop()
{ 
  // Détection poids entrée
  val = analogRead(resistance);
  Serial.println(val);
  
  if(val <400){
    char msg[12] = {'p','a','i','l','l','a','s','s','o','n',' ','#'};
    msg[11]=val;
    vw_send((uint8_t *)msg, 12);  
    vw_wait_tx(); // Wait until the whole message is gone
    delay(1000);
  }     
}
