#include <WebServer.h>
#include <Adafruit_BMP085.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MLX90614.h>
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Adafruit_SSD1306.h>

#define FORCE_SENSOR_PIN 36 // ESP32 pin GIOP36 (ADC0): the FSR and 10K pulldown are connected to A0

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define FlamePin 27// This is for input pin
int Flame;
boolean state = false;

Adafruit_BMP085 bmp;
const char* ssid = "Nintendo";
const char* password = "11111111";

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

WiFiServer server(80);

String header;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
Serial.begin(9600);

if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  

  if (!mlx.begin())
  {
    Serial.println("Not connected with MLX90614 sensor, check connections ");
    while (1) {}
  }
  if (!bmp.begin())
  {
    Serial.println("Not connected with BMP180/BMP085 sensor, check connections ");
    while (1) {}
  }
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

  pinMode(FlamePin, INPUT);
}

//void sendUptime() {
  
 
//    digitalWrite(BUZZER, HIGH);
     
//    Serial.print("Fire In the House!");
   
  
// else
// {
// Serial.println("No flame");
//// digitalWrite(BUZZER, LOW);
//  }
//}


void loop(){

  Flame = digitalRead(FlamePin);
 if (Flame== LOW)
    Serial.println("Fire DEtected In the House!");

  display.setCursor(0,0);
display.println("Temp = ");
display.setCursor(0,20);
display.println(mlx.readObjectTempF());
display.display();
delay(2000);
display.clearDisplay();
display.setCursor(0,0);
display.println("Pres = ");
display.setCursor(0,20);
display.println(bmp.readPressure());
display.display();
delay(2000);
display.clearDisplay();
display.setCursor(0,0);
display.println("Sea Pr = ");
display.setCursor(0,20);
display.println(bmp.readSealevelPressure());
display.display();
delay(2000);
display.clearDisplay();
//display.setCursor(0,0);
//display.println("Force = ");
//display.setCursor(0,20);
//display.println(analogReading);
//display.display();
delay(2000);
display.clearDisplay();

  int analogReading = analogRead(FORCE_SENSOR_PIN);
    Serial.print("The force sensor value = ");
  Serial.print(analogReading); // print the raw analog reading

  if (analogReading < 10)       // from 0 to 9
    Serial.println(" -> no pressure");
  else if (analogReading < 200) // from 10 to 199
    Serial.println(" -> light touch");
  else if (analogReading < 500) // from 200 to 499
    Serial.println(" -> light squeeze");
  else if (analogReading < 800) // from 500 to 799
    Serial.println(" -> medium squeeze");
  else // from 800 to 1023
    Serial.println(" -> big squeeze");

  delay(500);

  float Env_Temp = bmp.readTemperature();
  float Pressure = bmp.readPressure();
  Pressure = (Pressure / 100.0F);
  float Altitude = bmp.readAltitude();
  float Amb_Temp = mlx.readAmbientTempF();
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the table 
            client.println("<style>body { text-align: center; font-family: \"Trebuchet MS\", Arial;}");
            client.println("table { border-collapse: collapse; width:35%; margin-left:auto; margin-right:auto; }");
            client.println("th { padding: 12px; background-color: #0043af; color: white; }");
            client.println("tr { border: 1px solid #ddd; padding: 12px; }");
            client.println("tr:hover { background-color: #bcbcbc; }");
            client.println("td { border: none; padding: 12px; }");
            client.println(".sensor { color:white; font-weight: bold; background-color: black; padding: 1px; }");
            
            // Web Page Heading
            client.println("</style></head><body><h1>Worker body health</h1>");
            client.println("<table><tr><th>MEASUREMENT</th><th>VALUE</th></tr>");
            client.println("<tr><td>Environment Temp.Celsius</td><td><span class=\"sensor\">");
            client.println(Env_Temp);
            client.println(" *C</span></td></tr>");        
            client.println("<tr><td>Pressure</td><td><span class=\"sensor\">");
            client.println(Pressure);
            client.println(" hPa</span></td></tr>");
            client.println("<tr><td>Approx. Altitude</td><td><span class=\"sensor\">");
            client.println(Altitude);
            client.println(" m</span></td></tr>"); 
            client.println("<tr><td>Body Temp</td><td><span class=\"sensor\">");
            client.println(Amb_Temp);
            client.println(" *F</span></td></tr>");
            client.println("<tr><td>Force</td><td><span class=\"sensor\">");
            client.println(analogReading);
            client.println(" units</span></td></tr>"); 
            client.println("</body></html>");
            
            
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
