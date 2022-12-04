#include <WiFi.h>
#include <ESP32Servo.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

//#define YellowLed           D1
//#define RedLed           D2


#define WLAN_SSID       "Nintendo"             // Your SSID
#define WLAN_PASS       "11111111"        // Your password

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com" //Adafruit Server
#define AIO_SERVERPORT  1883                   
#define AIO_USERNAME    "Ninad_Shahgadkar"            // Username
#define AIO_KEY         "aio_YpnI28iAoh6kdYHGQnXxIOwZE4Ce"   // Auth Key

//WIFI CLIENT
WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Subscribe Servo_Deploy = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/Deploy"); // Feeds name should be same everywhere
Adafruit_MQTT_Subscribe Servo_Shut = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Shut");


void MQTT_connect();

Servo servo1;
//Servo servo2;
//Servo servo3;
//Servo servo4;

// Published values for SG90 servos; adjust if needed
//int minUs = 1000;
//int maxUs = 2000;

// These are all GPIO pins on the ESP32
// Recommended pins include 2,4,12-19,21-23,25-27,32-33
// for the ESP32-S2 the GPIO pins are 1-21,26,33-42
int servo1Pin = 13;
//int servo2pin = 14;
//int servo3pin = 15;
//int servo4pin = 27;

int pos = 0; // position in degrees
//ESP32PWM pwm;

void setup() {
  Serial.begin(115200);

      // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
 //Serial.begin(115200);
  servo1.setPeriodHertz(50);
//  servo2.setPeriodHertz(50);
//  servo3.setPeriodHertz(50);
//  servo4.setPeriodHertz(50);// Standard 50hz servo

  servo1.attach(servo1Pin, 500, 2400);
//  servo2.attach(servo2pin, 500, 2400);
//  servo3.attach(servo3pin, 500, 2400);
//  servo4.attach(servo4pin, 500, 2400);

//  pinMode(YellowLed, OUTPUT);
//  pinMode(RedLed, OUTPUT);

  
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP());
 
  mqtt.subscribe(&Servo_Deploy);
  mqtt.subscribe(&Servo_Shut);

}

void loop() {
 
  MQTT_connect();
  

  Adafruit_MQTT_Subscribe *subscription;
 while ((subscription = mqtt.readSubscription(20000))) {
    if (subscription == &Servo_Deploy) {
     for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    servo1.write(pos);
//    servo3.write(pos);// tell servo to go to position in variable 'pos'
    delay(15);             // waits 15ms for the servo to reach the position
  } 
    }
    if (subscription == &Servo_Shut) {
    for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    servo1.write(pos); 
//    servo3.write(pos);// tell servo to go to position in variable 'pos'
    delay(15);             // waits 15ms for the servo to reach the position
  }
  }

  
}
}


void MQTT_connect() {
  int8_t ret;

  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  
  while ((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000); 
    retries--;
    if (retries == 0) {
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
  
}
