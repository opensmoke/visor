#include <map>
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

std::map<int, std::array<int,2>> probeDisplayOffsets;

GFXfont OFSmall = FreeSansBoldOblique9pt7b;
GFXfont OFMedium = FreeSansBoldOblique12pt7b;
GFXfont OFLarge = FreeSansBoldOblique18pt7b;
GFXfont OFHuge = FreeSansBoldOblique24pt7b;

Adafruit_ILI9341 display(TFT_CS, TFT_DC);
GFXcanvas16 header(320, 60);
GFXcanvas16 content(320, 120);

    int Probe1Pin,Probe2Pin,Probe3Pin,Probe4Pin,Probe5Pin,Probe6Pin;
    int Probe1LedPin,Probe2LedPin,Probe3LedPin,Probe4LedPin,Probe5LedPin,Probe6LedPin;

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

Probe1Pin = 13;
Probe2Pin = 12;
Probe3Pin = 14;
Probe4Pin = 27;
Probe5Pin = 26;
Probe6Pin = 25;

Probe1LedPin = 2;
Probe2LedPin = 4;
Probe3LedPin = 5;
Probe4LedPin = 33;
Probe5LedPin = 32;
Probe6LedPin = 34;

probeDisplayOffsets[Probe1LedPin] = {0,40};
probeDisplayOffsets[Probe2LedPin] = {120,40};
probeDisplayOffsets[Probe3LedPin] = {240,40};
probeDisplayOffsets[Probe4LedPin] = {0,120};
probeDisplayOffsets[Probe5LedPin] = {120,120};
probeDisplayOffsets[Probe6LedPin] = {240,120};

    adcMax = 4095.0; // ADC resolution 12-bit (0-4095)
    Vs = 3.3;        // supply voltage    

    pinMode(Probe1LedPin, OUTPUT);
    pinMode(Probe2LedPin, OUTPUT);
    pinMode(Probe3LedPin, OUTPUT);
    pinMode(Probe4LedPin, OUTPUT);
    pinMode(Probe5LedPin, OUTPUT);
    pinMode(Probe6LedPin, OUTPUT);

}


    double getTemp(int probe) {
      double Vout, Rt = 0;
      double T, Tc, Tf = 0;

      Vout = analogRead(probe) * Vs/adcMax;
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

void updateTempDisplay(double temp, char* tempDisplay, int led) {
  std::array<int,2> offset = probeDisplayOffsets.at(led);
  if(temp > 32) {
    writeText(tempDisplay, offset[0], offset[1]);
      digitalWrite(led, HIGH);
} else {
    writeText("--", offset[0], offset[1]);
      digitalWrite(led, LOW);
}
}

void loop() {
  content.fillScreen(ILI9341_WHITE); 

    double temp1 = getTemp(Probe1Pin); 
    double temp2 = getTemp(Probe2Pin); 
    double temp3 = getTemp(Probe3Pin); 
    double temp4 = getTemp(Probe4Pin); 
    double temp5 = getTemp(Probe5Pin); 
    double temp6 = getTemp(Probe6Pin); 

    char buffer1[8];
    char* temp1String = dtostrf(temp1,1,2, buffer1);

        char buffer2[8];
    char* temp2String = dtostrf(temp2,1,2, buffer2);

        char buffer3[8];
    char* temp3String = dtostrf(temp3,1,2, buffer3);

        char buffer4[8];
    char* temp4String = dtostrf(temp4,1,2, buffer4);

        char buffer5[8];
    char* temp5String = dtostrf(temp5,1,2, buffer5);

        char buffer6[8];
    char* temp6String = dtostrf(temp6,1,2, buffer6);
    
      content.setFont(&OFMedium);

updateTempDisplay(temp1, temp1String, Probe1LedPin);
updateTempDisplay(temp2, temp2String, Probe2LedPin);
updateTempDisplay(temp3, temp3String, Probe3LedPin);
updateTempDisplay(temp4, temp4String, Probe4LedPin);
updateTempDisplay(temp5, temp5String, Probe5LedPin);
updateTempDisplay(temp6, temp6String, Probe6LedPin);

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
