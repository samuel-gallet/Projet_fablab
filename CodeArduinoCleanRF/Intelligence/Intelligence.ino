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
    lamp();  
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;
    if (vw_get_message(buf, &buflen)) // Non-blocking
    {
      //Serial.print("commande reçu : ");
      //Serial.println((char *)buf);
      manageCommand((char*)buf);
      if (attenteCapteur)
      {
        t = millis();
        while (t + 5000 > millis())
        {
          lamp();
          if (vw_get_message(buf, &buflen))
          {
            manageCommand((char *)buf);
            if (!attenteCapteur)
              break;
          }
          
        }
        lamp();
        attenteCapteur = false;
      }
      if (attentePaillasson)
      {
        t = millis();
        while (t + 5000 > millis())
        {
          lamp();
          if (vw_get_message(buf, &buflen))
          {
            manageCommand((char *)buf);
            if (!attentePaillasson)
              break;
          }
        }
        lamp();
        attentePaillasson = false;
      }
    }   
}

void manageCommand(char * command) 
{
  if (strstr(command, "capteur;"))
  {
    if (attenteCapteur) 
    {
      Serial.print("entree, p=");
      Serial.print((uint8_t)command[11]);
      Serial.print(";");
      attenteCapteur = false;
      cptPersonne++;
      Serial.print("nb Personne=");
      Serial.print(cptPersonne);
      Serial.println(";");
    }
    else if (cptPersonne == 0) 
    {
      Serial.println("intrusion;");
    } else
    {
      Serial.println("meteo;");
      attentePaillasson = true;
    }
  }
  if (strstr(command, "paillasson "))
  {
    if (attentePaillasson) 
    {
      Serial.print("sortie, p=");
      Serial.print((uint8_t)command[11]);
      Serial.print(";");
      attentePaillasson = false;
      cptPersonne --;
      Serial.print("nb Personne=");
      Serial.print(cptPersonne);
      Serial.println(";");
    }
    else
    {
      attenteCapteur = true;
    }
  }   
}

void lamp()
{
  char command[500];
  if (Serial.readBytes(command, 100) == 0)
  {
    command == NULL;
  } else 
  {
    //Serial.println("switch on recu");
    //Serial.println(command);
    char *p = command;
    char *str;
    while ((str = strtok_r(p, ";", &p)) != NULL)
    {
      if (strstr(str, "switch on") > 0)
      {
        //Serial.println("ok");
       // active = true;
        mySwitch.switchOn(2, 2);
      } else if (strstr(str, "switch off") > 0)
      {      
        //active = false;
        mySwitch.switchOff(2, 2);

      } else if (strstr(str, "blue") > 0 || strstr(str, "yell") > 0 || strstr(str, "whit") > 0)
      {
        vw_send((uint8_t *)*p, 4);  
        vw_wait_tx(); // Wait until the whole message is gone
      }
    }
  }  
}

/*void meteo()
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
      if (strstr(str, "blue") > 0 || strstr(str, "yell") > 0 || strstr(str, "whit") > 0)
      {
        vw_send((uint8_t *)*p, 4);  
        vw_wait_tx(); // Wait until the whole message is gone
      }
    }
  }  
}
*/
