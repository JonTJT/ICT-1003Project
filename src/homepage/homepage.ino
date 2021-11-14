#include <TinyScreen.h>
#include <SPI.h>
#include <Wire.h>
#include "Bitmaps.h"
#include "BMA250.h"
//#include "drawSprites.h"

TinyScreen display = TinyScreen(TinyScreenDefault);
BMA250 accel_sensor;
int wizard_x = 60; //Temporary wizard location, later to use global x coordinate var
int gravity = 1; // Determines if screen if flipped or not, 1 is flipped 0 is not flipped
int x_accel = 0; //Accelerometer value for x axis
int backgroundColor = TS_16b_Black;


void setup() {
  randomSeed(analogRead(0)); // Make it more random
  Wire.begin();
  display.begin();
  display.setBrightness(3);
  display.clearScreen();
  display.setBitDepth(TSBitDepth16);
  display.setColorMode(TSColorModeRGB);  
  display.setFlip(gravity);
  uint16_t time = millis();
  // Set up the BMA250 acccelerometer sensor
  accel_sensor.begin(BMA250_range_2g, BMA250_update_time_32ms); 
}


void loop() {
  display.clearScreen();
  drawBuffer(wizard,60,30);
  display.setFont(thinPixel7_10ptFontInfo);
  display.fontColor(TS_8b_Green,TS_8b_Black);
  display.setCursor(0,10);
  display.print("Why hello there...");
  display.setCursor(0,20);
  display.print("I'm Saicik!");
  delay(5000);
  display.clearWindow(0,0,96,30);
  
  // Here is the infinite loop for idle, change to for loop if want limit loops.
  while (1){
//    display.clearScreen();
//    display.setCursor(3,10);
//    display.print(i);
    accel_sensor.read();//This function gets new data from the acccelerometer
    x_accel = accel_sensor.X;
    gravity_check();
    idle();
  }  
}


//Currently used for walking
void idle(){  
  int max_walk = 0, rand_walk =0, bound = 0;
  
  //Get wizards coordinates, decide how far can he walk in either direction
  int walk_direction = random(2); //0 = left, 1 = right
  if (walk_direction == 0){                                    //Walk left
    max_walk = wizard_x / 5;
    rand_walk = random(max_walk);
    bound = wizard_x - (rand_walk * 5);
    //display.setCursor(30,10);
    //display.print(bound);
    if (bound >= wizard_x) {
      display.setCursor(20,10);
      display.print("*zzzzzzzz*");
      delay(5000);
      display.clearWindow(0,0,96,30);
      return;
    }
 
    for (wizard_x; wizard_x > bound; wizard_x -= 5) {
      //display.setCursor(45,10);
      //display.print(wizard_x);
      //drawMap(wizardBitmap,34,54,wizard_x,30);
      drawBuffer(wizard, wizard_x, 30);
      delay(1000);
    }
  } else {                                                     //Walk Right
    max_walk = (80 - wizard_x) / 5;
    rand_walk = random(max_walk);
    //display.setCursor(15,10);
    //display.print(rand_walk);
    bound = wizard_x + rand_walk * 5;
    //display.setCursor(30,10);
    //display.print(bound);
    if (bound <= wizard_x) {
      display.setCursor(15,10);
      display.print("*lalalalala*");
      delay(5000);
      display.clearWindow(0,0,96,30);
      return;
    }
    for (wizard_x; wizard_x < bound; wizard_x += 5) {
      //display.setCursor(45,10);
      //display.print(wizard_x);
      //drawMap(wizardBitmap_flipped,34,54,wizard_x,30);
      drawBuffer(wizard_flipped, wizard_x, 30);
      delay(1000);
    }
  }
  //Make him talk
    talk();
}

// Fn to make him say random things
void talk() {
  int rand_word = random(5);
  switch(rand_word) {
    case 0:
      display.setCursor(0,10);
      display.print("I predict that...");
      delay(1000);
      display.setCursor(0,20);
      display.print("you gained weight");
      delay(3000);
      display.clearWindow(0,0,96,30);
      break;
   case 1:
      display.setCursor(0,10);
      display.print("I'm not happy...");
      delay(1000);
      display.setCursor(0,20);
      display.print("but I wanna be");
      delay(3000);
      display.clearWindow(0,0,96,30);
      break;
    case 2:
      display.setCursor(0,10);
      display.print("The stars tell me...");
      delay(1000);
      display.setCursor(0,20);
      display.print("u gey hahahahha");
      delay(3000);
      display.clearWindow(0,0,96,30);
      break;
  case 3:
      display.setCursor(0,10);
      display.print("My magic ball says..");
      delay(1000);
      display.setCursor(0,20);
      display.print("low battery? Wut");
      delay(3000);
      display.clearWindow(0,0,96,30);
      break;
  case 4:
      display.setCursor(0,10);
      display.print("Your ICT1003...");
      delay(1000);
      display.setCursor(0,20);
      display.print("GETS A+ BABY!!!");
      delay(3000);
      display.clearWindow(0,0,96,30);
      break;
  }
}

void gravity_check() {
  // This function flips the display, and plays a falling animation before continuing idle
  if (x_accel == -1) {
    // Print error message to Serial Monitor
    display.print("ERROR! NO BMA250 DETECTED!");
  }

  if (x_accel <= -200 && gravity != 1) {
    gravity = 1;
    display.setFlip(gravity);
  } else if (x_accel >= 200 && gravity != 0) {
    gravity = 0;
    display.setFlip(gravity);
  }
  
}

//// drawBuffer function draws a bitmap at the start_x and the start_y provided.
//void drawBuffer(sprite bitmap, int start_x, int start_y) {
//  uint8_t lineBuffer[96 * 64 * 3];
//  display.startData();
//  for (int y = 0; y < 64; y++) {
//    for (int b = 0; b < 96; b++) {
//      lineBuffer[b * 2] = backgroundColor >> 8;
//      lineBuffer[b * 2 + 1] = backgroundColor;
//    }
//    if (y >= start_y && y < start_y + bitmap.height) {
//      int endX = start_x + bitmap.width;
//      if (start_x < 96 && endX > 0) {
//        int xBitmapOffset = 0;
//        int xStart = 0;
//        if (start_x < 0) xBitmapOffset -= start_x;
//        if (start_x > 0) xStart = start_x;
//        int yBitmapOffset = (y - start_y) * bitmap.width;
//        for (int x = xStart; x < endX; x++) {
//          unsigned int color = bitmap.bitmap[xBitmapOffset + yBitmapOffset++];
//          if (color != backgroundColor) {
//            lineBuffer[(x) * 2] = color >> 8;
//            lineBuffer[(x) * 2 + 1] = color;
//          }
//        }
//      }
//    }
//    display.writeBuffer(lineBuffer, 96 * 2);
//  }
//  display.endTransfer();
//}

// drawBuffer function draws a bitmap at the start_x and the start_y provided.
void drawBuffer(sprite bitmap, int start_x, int start_y) {
  uint8_t lineBuffer[96 * 64 * 1];
  display.goTo(0,0);
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
