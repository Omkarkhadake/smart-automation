#include <Adafruit_BMP085.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MLX90614.h>


Adafruit_MLX90614 mlx = Adafruit_MLX90614();
Adafruit_BMP085 bmp;
  

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);
  //Serial.begin(9600);
  mlx.begin();
  bmp.begin();

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  

}

void loop() 
{
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

  //mlx.update();
//  bmp.update();
    // Display static text
  

  
  
  
}
