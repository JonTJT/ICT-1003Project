#include <TinyScreen.h>
#include <SPI.h>
#include <Wire.h>
#include "Bitmaps.h"

#define brightness 10

TinyScreen display = TinyScreen(TinyScreenDefault);

typedef struct {
  int width;
  int height;
  const  unsigned int *bitmap;
} sprite;

int backgroundColor = TS_16b_Black;
//Sprite initialisation
sprite wizard = {34,54,wizardBitmap};
sprite wizard_flipped = {34,54,wizardBitmap_flipped};
sprite crystal_ball = {20,20,crystalBallBitmap};

void setup() {
  Wire.begin();
  display.begin();
  display.setBrightness(2);
  display.clearScreen();
  display.setBitDepth(TSBitDepth16);
  display.setColorMode(TSColorModeRGB);
 
  
  //drawMap(wizardBitmap,34,54,31,5);
}

void loop() {
  drawBuffer(wizard_flipped,30,10);
  delay(1000);
  drawBuffer(wizard,30,10);
  delay(1000);
  
  
  drawBuffer(crystal_ball, 30,30);
  delay(1000);
  // drawMap(wiz
}

// Decommissioned, prints line by line, can be used for cool appearances
void drawMap(sprite bitmap,int start_x,int start_y) {
  display.goTo(start_x, start_y);
  for (int i=0; i<bitmap.width*bitmap.height; i++){
    if (i%bitmap.width == 0){
      display.goTo(start_x,start_y+(i/bitmap.height));
    }
    display.writePixel(bitmap.bitmap[i]);
  }
}

// drawBuffer function draws a bitmap at the start_x and the start_y provided.
void drawBuffer(sprite bitmap, int start_x, int start_y) {
  uint8_t lineBuffer[96 * 64 * 3];
  display.startData();
  for (int y = 0; y < 64; y++) {
    for (int b = 0; b < 96; b++) {
      lineBuffer[b * 2] = backgroundColor >> 8;
      lineBuffer[b * 2 + 1] = backgroundColor;
    }
    if (y >= start_y && y < start_y + bitmap.height) {
      int endX = start_x + bitmap.width;
      if (start_x < 96 && endX > 0) {
        int xBitmapOffset = 0;
        int xStart = 0;
        if (start_x < 0) xBitmapOffset -= start_x;
        if (start_x > 0) xStart = start_x;
        int yBitmapOffset = (y - start_y) * bitmap.width;
        for (int x = xStart; x < endX; x++) {
          unsigned int color = bitmap.bitmap[xBitmapOffset + yBitmapOffset++];
          if (color != backgroundColor) {
            lineBuffer[(x) * 2] = color >> 8;
            lineBuffer[(x) * 2 + 1] = color;
          }
        }
      }
    }
    display.writeBuffer(lineBuffer, 96 * 2);
  }
  display.endTransfer();
}
