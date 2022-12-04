//#define BLYNK_PRINT Serial
//#include <WiFi.h>
//#include <WiFiClient.h>
//#include <BlynkSimpleEsp32.h>
//BlynkTimer timer;
//
//// You should get Auth Token in the Blynk App.
//// Go to the Project Settings (nut icon).
//char auth[] = "_DUapHeDtLb99NDc40TnkjZPNTdbSME7";
//
//// Your WiFi credentials.
//// Set password to "" for open networks.
//char ssid[] = "kartik";
//char pass[] = "12345678";
//
//
//#define BUZZER 35 // Use buzzer for alert 
#define FlamePin 27// This is for input pin
int Flame;
boolean state = false;
 
void setup() {
    // Debug console
 Serial.begin(115200);
// Blynk.begin(auth, ssid, pass);
 pinMode(FlamePin, INPUT);
// pinMode(BUZZER, OUTPUT);
// timer.setInterval(1000L, sendUptime);
 
}
 
void sendUptime() {
  
 Flame = digitalRead(FlamePin);
 if (Flame== LOW)
  {
    Serial.println("Fire DEtected In the House!");
//    digitalWrite(BUZZER, HIGH);
     
//    Serial.print("Fire In the House!");
   
  }
 else
 {
 Serial.println("No flame");
// digitalWrite(BUZZER, LOW);
  }
}

//void loop()
//{
//  Blynk.run();
//  timer.run();
//}
 
