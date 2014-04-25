#include <VirtualWire.h>
#include <VirtualWire_Config.h>

#include <HRCSwitch.h>


HRCSwitch mySwitch = HRCSwitch();

const int receive_pin = 4;
const int transmit_pin = 9;
boolean attenteCapteur;
boolean attentePaillasson;
int cptPersonne;
unsigned long long t;

void setup()
{
    Serial.begin(9600);	// Debugging only

    // Initialise the IO and ISR
    vw_set_rx_pin(receive_pin);
    vw_setup(2000) ;
    vw_rx_start();       // Start the receiver PLL running
    mySwitch.enableTransmit(transmit_pin);
    mySwitch.switchOff(2, 2);
    attenteCapteur = false;
    attentePaillasson = false;
    cptPersonne = 0;
}

void loop()
{
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;
    if (vw_get_message(buf, &buflen)) // Non-blocking
    {
      
      manageCommand((char*)buf);
      if (attenteCapteur)
      {
        t = millis();
        while (t + 5000 > millis())
        {
          if (vw_get_message(buf, &buflen))
          {
            manageCommand((char *)buf);
            if (!attenteCapteur)
              break;
          }
        }
        attenteCapteur = false;
      }
      if (attentePaillasson)
      {
        t = millis();
        while (t + 5000 > millis())
        {
          if (vw_get_message(buf, &buflen))
          {
            manageCommand((char *)buf);
            if (!attentePaillasson)
              break;
          }
        }
        attentePaillasson = false;
      }
    }
    
}

/*
	Eventuellement, l'intelligence serait géré ici (et non dans OpenHAB)
	Grace à un switch et des variables globales.
	
*/

void manageCommand(char * command) 
{
  if (strstr(command, "capteur;"))
  {
    if (attenteCapteur) 
    {
      Serial.println("entree");
      attenteCapteur = false;
      cptPersonne++;
      Serial.print("nb Personne = ");
      Serial.print(cptPersonne);
      Serial.println();
    }
    else if (cptPersonne == 0) 
    {
      Serial.println("intrusion");
    } else
    {
      attentePaillasson = true;
    }
  }
  if (strstr(command, "paillasson "))
  {
    if (attentePaillasson) 
    {
      Serial.print("sortie ");
      Serial.print((uint8_t)command[11]);
      Serial.println();
      attentePaillasson = false;
      cptPersonne --;
      Serial.print("nb Personne = ");
      Serial.print(cptPersonne);
      Serial.println();
    }
    else
    {
      attenteCapteur = true;
    }
  }   
}


