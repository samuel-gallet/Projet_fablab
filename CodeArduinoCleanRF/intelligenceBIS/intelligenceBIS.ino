#include <VirtualWire.h>
#include <VirtualWire_Config.h>

#include <HRCSwitch.h>


HRCSwitch mySwitch = HRCSwitch();

const int receive_pin = 4;
const int transmit_pin_meteo = 12;
const int transmit_pin_lampe = 12;
unsigned long long t_capt = 0;
unsigned long long t_pail = 0;
unsigned long long t_meteo = 0;

void setup()
{
    Serial.begin(9600);	// Debugging only
    vw_set_rx_pin(receive_pin);
    vw_setup(2000) ;
    vw_rx_start();       // Start the receiver PLL running
    mySwitch.enableTransmit(transmit_pin_lampe);
    mySwitch.switchOff(2, 2);
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
      if (strstr((char *)buf, "capteur;")){
        if (t_capt == 0 || t_capt + 3000 < millis()) {
          Serial.println("capteur;");
          t_capt = millis();
        }
      }
      if (strstr((char *)buf, "paillasson")){
        if (t_pail == 0 || t_pail + 1000 < millis()) {
          Serial.println("paillasson;");
          t_pail = millis();
        }
        //delay(5000);
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
            vw_send((uint8_t *)msg, 5);
            vw_wait_tx();
        
      } else if (strstr(str, "yell") > 0) {
            init_meteo();
            char msg2[7] = {'y','e','l','l','o','w',';'};
            vw_send((uint8_t *)msg2, 7);  
            vw_wait_tx();
      }else if (strstr(str, "whit") > 0) {
            init_meteo();
            char msg3[6] = {'w','h','i','t','e',';'};
            vw_send((uint8_t *)msg3, 6);  
            vw_wait_tx();
      }
    
  
    }

  }  
}

