#include <VirtualWire.h>

const int receive_pin = 4;

void setup()
{
    Serial.begin(9600);	// Debugging only

    // Initialise the IO and ISR
    vw_set_rx_pin(receive_pin);
    vw_setup(2000) ;
    vw_rx_start();       // Start the receiver PLL running
}

void loop()
{
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;

    if (vw_get_message(buf, &buflen)) // Non-blocking
    {
	int i;
	for (i = 0; i < buflen; i++)
	{
	    Serial.print((char)buf[i]);
	}
	Serial.println();
    }
}

/*
	Eventuellement, l'intelligence serait géré ici (et non dans OpenHAB)
	Grace à un switch et des variables globales.
	
*/
