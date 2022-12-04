#include <ESP32Servo.h>
#include "Adafruit_MQTT.h"
#include "WiFiClientSecure.h"
#include "Adafruit_MQTT_Client.h"
#include <WiFi.h>
#include <HTTPClient.h>

//#define Servo_Pin           18
//#define RedLed           D2

const char* ssid = "Nintendo";
const char* password = "11111111";

const String endpoint = "http://api.openweathermap.org/data/2.5/weather?q=Pimpri,IN&APPID=";
const String key = "b4926eeb19ee7f544ee13e00e138698f";

//#define WLAN_SSID       "Nintendo"             // Your SSID
//#define WLAN_PASS       "11111111"        // Your password

/********* Adafruit.io Setup ***********/

#define AIO_SERVER      "io.adafruit.com" //Adafruit Server
#define AIO_SERVERPORT  1883                   
#define AIO_USERNAME    "ashishhande"            // Username
#define AIO_KEY         "aio_QUnM23JBRrgjTcHtoBccZEcDMGKN"   // Auth Key

//WIFI CLIENT
WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Subscribe Servo_Deploy = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/Deploy"); // Feeds name should be same everywhere
Adafruit_MQTT_Subscribe Servo_Shut = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Shut");
Adafruit_MQTT_Publish Weather_For = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Weather");


void MQTT_connect();

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;


// Published values for SG90 servos; adjust if needed
int minUs = 1000;
int maxUs = 2000;

// These are all GPIO pins on the ESP32
// Recommended pins include 2,4,12-19,21-23,25-27,32-33
// for the ESP32-S2 the GPIO pins are 1-21,26,33-42
int servo1Pin = 13;
int servo2pin = 14;
int servo3pin = 15;
int servo4pin = 27;

int pos = 0; // position in degrees
//int anglestep = 1;
//ESP32PWM pwm;

void setup() {
  Serial.begin(115200);
    // Allow allocation of all timers
//  ESP32PWM::allocateTimer(0);
//  ESP32PWM::allocateTimer(1);
//  ESP32PWM::allocateTimer(2);
//  ESP32PWM::allocateTimer(3);
// Serial.begin(115200);
//  servo1.setPeriodHertz(30);
//  servo2.setPeriodHertz(50);
//  servo3.setPeriodHertz(30);
//  servo4.setPeriodHertz(50);// Standard 50hz servo

  servo1.attach(servo1Pin, minUs, maxUs);
  servo2.attach(servo2pin, minUs, maxUs);
  servo3.attach(servo3pin, minUs, maxUs);
  servo4.attach(servo4pin, minUs, maxUs);

//  servo1.write(0);
//  servo2.write(180);
//  servo3.write(0);
//  servo4.write(180);

  //servo.attach(Servo_Pin);
  //pinMode(RedLed, OUTPUT);

  
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
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

void loop() 
{


 
  MQTT_connect();
//    if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
// 
//    HTTPClient http;
// 
//    http.begin(endpoint + key); //Specify the URL
//    int httpCode = http.GET();  //Make the request
//    String payload = http.getString();
//
//     // Weather_For.publish(httpCode);
//     
//
// 
//    if (httpCode > 0) { //Check for the returning code
// 
//        
//        
//        Serial.println(httpCode);
//        Serial.println(payload);
//        
//      }
// 
//    else {
//      Serial.println("Error on HTTP request");
//    }
//      
//  
// 
//    http.end(); //Free the resources
//    
//
//  }


  


 
//  delay(1000);



  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription()))
 {
    if (subscription == &Servo_Deploy) 
    {
  for (pos = 0; pos <= 180; pos += 1) 
  { // sweep from 0 degrees to 180 degrees
    // in steps of 1 degree
    servo1.write(pos);
    servo3.write(pos);
    
  }
    delay(1000);
  
//  for (pos = 100; pos >= 45; pos += 1) 
//  { // sweep from 0 degrees to 180 degrees
//    // in steps of 1 degree
//    servo2.write(pos);
//    servo4.write(pos);
//    delay(100);
//  
//  
//  }
    }else
  
    
 if(subscription == &Servo_Shut)
    {
//      servo1.detach();
//      servo3.detach();
    
    
for (pos = 180; pos >= 0; pos -= 1) 
  { // sweep from 0 degrees to 180 degrees
    // in steps of 1 degree
    servo1.write(pos);
    servo3.write(pos);
    
  
  }
  delay(1000);
//  for (pos = 180; pos <= 0; pos -= 1) 
//  { // sweep from 0 degrees to 180 degrees
//    // in steps of 1 degree
//    servo1.write(pos);
//    servo3.write(pos);
//    
//
//    delay(100);
//  }
 
    }
}
}

void MQTT_connect() 
{
  int8_t ret;

  if (mqtt.connected()) 
  {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  
  while ((ret = mqtt.connect()) != 0) 
  {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000); 
    retries--;
    if (retries == 0) 
    {
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
  
}
