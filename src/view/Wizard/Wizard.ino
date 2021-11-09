#include <TinyScreen.h>
#include <SPI.h>
#include <Wire.h>
#include "Bitmaps.h"

TinyScreen display = TinyScreen(TinyScreenDefault);

void setup() {
  Wire.begin();
  display.begin();
  display.setBrightness(3);
  display.clearScreen();
  display.setBitDepth(TSBitDepth16);
  display.setColorMode(TSColorModeRGB);  
  
}

void loop() {
  display.clearScreen();
  drawMap(wizardBitmap,34,54,30,10);
  delay(1000);
}

void drawMap(unsigned int* bitmap,int x_len,int y_len,int start_x,int start_y) {
  display.goTo(start_x, start_y);
  for (int i=0; i<x_len*y_len; i++){
    if (i%x_len == 0){
      display.goTo(start_x,start_y+(i/y_len));
    }
    display.writePixel(bitmap[i]);
  }
}
