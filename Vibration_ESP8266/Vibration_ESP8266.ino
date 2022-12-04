

#include <ESP8266WiFi.h>




 
// sensors 
int vs = D0; // vibration sensor 
int sdata = 0; // sensor data will be stored in this variable.
 

 
// This function sends Arduino's up time every second to Virtual Pin (1).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.

void setup()
{
  // Debug console
  Serial.begin(9600);
  pinMode(vs ,INPUT); 
  
 
  // Setup a function to be called every second
// 1000L
}
 

 
void sensorvalue()
{
  long measurement =vibration();
 //delay(50);
  Serial.println(measurement);
  digitalRead(vs, measurement);
  if( measurement > 1000 )
  {
    Serial.println("Vibration detected!!");
  }

}
 
long vibration(){
  long measurement=pulseIn (vs, HIGH);  //wait for the pin to get HIGH and returns measurement
  return measurement;
}
