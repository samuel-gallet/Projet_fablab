#include <VirtualWire.h>
#include <VirtualWire_Config.h>

#include <HRCSwitch.h>


HRCSwitch mySwitch = HRCSwitch();

const int receive_pin = 4;
const int transmit_pin_meteo = 12;
const int transmit_pin_lampe = 12;
boolean attenteCapteur;
boolean attentePaillasson;
int cptPersonne;
unsigned long long t;
unsigned long long t2=0;

void setup()
{
    Serial.begin(9600);	// Debugging only
    vw_set_rx_pin(receive_pin);
    vw_setup(2000) ;
    vw_rx_start();       // Start the receiver PLL running
    // Initialise the IO and ISR
    mySwitch.enableTransmit(transmit_pin_lampe);
    mySwitch.switchOff(2, 2);
    attenteCapteur = false;
    attentePaillasson = false;
    cptPersonne = 0;
}

void init_lampe() {
  mySwitch.enableTransmit(transmit_pin_lampe);
}

void init_meteo() {
  vw_set_tx_pin(transmit_pin_meteo);
  vw_setup(2000) ;

}

void loop()
{
  init_meteo();
  lamp();  
   uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;
    if (vw_get_message(buf, &buflen)) // Non-blocking
    {
      manageCommand((char*)buf);
      if (attenteCapteur)
      {
        t = millis();
        while (t + 7000 > millis())
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
        while (t + 7000 > millis())
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
  void init_meteo();
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
    else if ( (cptPersonne == 0 && t2+60000<millis()) || t2==0) 
    {
      Serial.println("intrusion;");
      t2= millis();
    } else
    {
      if (!attentePaillasson && cptPersonne > 0){ 
        Serial.println("meteo;");
      }
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
    char *p = command;
    char *str;
    while ((str = strtok_r(p, ";", &p)) != NULL)
    {
      if (strstr(str, "switch on") > 0)
      {
        init_lampe();
        mySwitch.switchOn(2, 2);
        init_meteo();
      } else if (strstr(str, "switch off") > 0)
      { 
         init_lampe();     
         mySwitch.switchOff(2, 2);
         init_meteo();

      } else if (strstr(str, "blue") > 0)  {
            init_meteo();
            char msg[5] = {'b','l','u','e',';'};
            Serial.println(msg);
            vw_send((uint8_t *)msg, 5);
            vw_wait_tx();
        
      } else if (strstr(str, "yell") > 0) {
            init_meteo();
            char msg2[7] = {'y','e','l','l','o','w',';'};
            Serial.println(msg2);
            vw_send((uint8_t *)msg2, 7);  
            vw_wait_tx();
      }else if (strstr(str, "whit") > 0) {
            init_meteo();
            char msg3[6] = {'w','h','i','t','e',';'};
            Serial.println(msg3);
            vw_send((uint8_t *)msg3, 6);  
            vw_wait_tx();
      }
    
  
    }

  }  
}

