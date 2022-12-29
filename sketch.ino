#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <Fonts/FreeSansBoldOblique9pt7b.h>
#include <Fonts/FreeSansBoldOblique12pt7b.h>
#include <Fonts/FreeSansBoldOblique18pt7b.h>
#include <Fonts/FreeSansBoldOblique24pt7b.h>

#define TFT_SCK    18
#define TFT_MOSI   23
#define TFT_MISO   19
#define TFT_CS     22
#define TFT_DC     21
#define TFT_RST  15

GFXfont OFSmall = FreeSansBoldOblique9pt7b;
GFXfont OFMedium = FreeSansBoldOblique12pt7b;
GFXfont OFLarge = FreeSansBoldOblique18pt7b;
GFXfont OFHuge = FreeSansBoldOblique24pt7b;

Adafruit_ILI9341 display(TFT_CS, TFT_DC);
GFXcanvas16 header(320, 60);
GFXcanvas16 content(320, 120);

    int ThermistorPin;
    double adcMax, Vs;

    double R1 = 10000.0;   // voltage divider resistor value
    double Beta = 3950.0;  // Beta value
    double To = 298.15;    // Temperature in Kelvin for 25 degree Celsius
    double Ro = 10000.0;   // Resistance of Thermistor at 25 degree Celsius

void setup() {
  Serial.begin(115200);

  display.begin();
  display.setRotation(1);
    display.fillScreen(ILI9341_WHITE); 
setupCanvas();
    drawHeader();

ThermistorPin = 13;
    adcMax = 4095.0; // ADC resolution 12-bit (0-4095)
    Vs = 3.3;        // supply voltage    

    pinMode(2, OUTPUT);

}


    double getTemp() {
      double Vout, Rt = 0;
      double T, Tc, Tf = 0;

      Vout = analogRead(ThermistorPin) * Vs/adcMax;
      Rt = R1 * Vout / (Vs - Vout);
      T = 1/(1/To + log(Rt/Ro)/Beta);  // Temperature in Kelvin
      Tc = T - 273.15;                 // Celsius
      Tf = Tc * 9 / 5 + 32;            // Fahrenheit
      Serial.println(Tc);

      return Tf;
    }

void setupCanvas() {
    header.fillScreen(ILI9341_WHITE); 
    header.setTextColor(ILI9341_BLACK);
    content.fillScreen(ILI9341_WHITE); 
    content.setTextColor(ILI9341_BLACK);
}

void loop() {
  content.fillScreen(ILI9341_WHITE); 

    double temp = getTemp(); 
    char buffer[8];
    char* tempString = dtostrf(temp,1,2, buffer);
    
      content.setFont(&OFMedium);

    writeText(tempString, 0, 20);

if(temp > 0) {
  digitalWrite(2, HIGH);
} else {
  digitalWrite(2, LOW);
}

  display.drawRGBBitmap(0, 60, content.getBuffer(), content.width(), content.height());

}

void drawHeader() {
  header.setFont(&OFLarge);

  header.setCursor(0, 30);
  header.print("Open Smoke");
  header.setFont(&OFSmall);
    header.setCursor(220, 30);
      header.print(" v1.0.0");
        display.drawRGBBitmap(0, 0, header.getBuffer(), header.width(), header.height());
}

void smallFont() {
  content.setFont(&OFSmall);
}

void moveTo(int16_t x, int16_t y) {
    content.setCursor(x, y);
}

void writeText(char* s, int16_t x, int16_t y) {
  content.setCursor(x, y);
  content.print(s);
}
