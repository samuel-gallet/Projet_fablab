float val;
int led = 9;
int resistance= A0; // pin analog A0
int t = 10000;
int capteur = 3;
boolean peut_sortir = false;
int nbpresent = 0;

void setup ()
{
  Serial.begin(9600);
  
  pinMode(resistance, INPUT);
  pinMode(led, OUTPUT);
}

void loop()
{ 
  val = analogRead(resistance);
  
  if (val > 600) {
    analogWrite(led, 0) ;
    if (peut_sortir == true){
      if (digitalRead(capteur) == 1){
        t= 10000 + millis();   
        while (t-millis() > 0 && (t-millis())<1000000){
          Serial.println(t-millis());
          val = analogRead(resistance);
          if(val< 600){
            Serial.println("Sortie.");
            //nbpresent = nbpresent - 1;
            //Serial.println(nbpresent);
            //if (nbpresent == 0 ){
               peut_sortir = false;
            //}
          }
        }
      }
    }
  }
  else { 
    t= 10000 + millis();
    while (t-millis() > 0){
      //Serial.println(millis());
      if (digitalRead(capteur) == 1){
        /*val = analogRead(resistance);
        analogWrite(led, val) ;*/
        Serial.println("Entree.");
        peut_sortir = true;
        //nbpresent = nbpresent + 1;
      }
    }    
}
  
  delay(100);
}


void sendMessage(char *key, int value)
{
  Serial.print(key);
  Serial.print(value);
  Serial.println(";");
  Serial.flush();
}
