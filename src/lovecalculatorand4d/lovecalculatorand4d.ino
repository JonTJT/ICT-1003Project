//-------------------------------------------------------------------------------
//  TinyCircuits TinyScreen/TinyScreen+ Basic Example
//  Last Updated 26 January 2016
//
//  This example shows the basic functionality of the TinyScreen library,
//  including drawing, writing bitmaps, and printing text
//
//  Written by Ben Rose for TinyCircuits, https://tiny-circuits.com
//
//-------------------------------------------------------------------------------

#include <Wire.h>
#include <SPI.h>
#include <TinyScreen.h>
#include "bitmaps.h"

//Library must be passed the board type
//TinyScreenDefault for TinyScreen shields
//TinyScreenAlternate for alternate address TinyScreen shields
//TinyScreenPlus for TinyScreen+
TinyScreen display = TinyScreen(TinyScreenDefault);
#include "drawSprites.h"
sprite heart_list[7] = {heart_1,heart_2,heart_3,heart_4,heart_5,heart_6};

void setup(void) {
  Wire.begin();//initialize I2C before we can initialize TinyScreen- not needed for TinyScreen+
  display.begin();
  //setBrightness(brightness);//sets main current level, valid levels are 0-15
  display.setBrightness(2);
  display.setFont(thinPixel7_10ptFontInfo);
  display.setBitDepth(TSBitDepth16); //Set color bit depth to 16-bit color scheme. Bitmap colors will not display correctly unless this is set.
  display.setColorMode(TSColorModeRGB); //Set color mode to RGB. Bitmap colors will not display correctly unless this is set.
  display.fontColor(TS_16b_White, TS_16b_Black);
  display.setFlip(1);
}

void loop() {
  buttonLoop();
}

void buttonLoop() {
  unsigned long currentTime;
  unsigned long buttonsstartTime = 0;
  int buttonspressed = 0;
  int percentage;
  int heart_no;
  currentTime = millis();

  // Clear the screen if the buttons are not pressed.
  

  if (display.getButtons(TSButtonLowerLeft)) {
    number4d();
  }

  if (display.getButtons(TSButtonUpperLeft) && display.getButtons(TSButtonUpperRight)) {
    display.clearScreen();
  }

  while (display.getButtons(TSButtonUpperLeft) && display.getButtons(TSButtonUpperRight)) {
    // Set the button pressed boolean to 1, to allow the program to know when to clear screen
    buttonspressed = 1;
    buttonsstartTime = millis();
    percentage = floor((buttonsstartTime - currentTime) / 1000) * 20;
    heart_no = floor((buttonsstartTime - currentTime)/1000);
    delay(1000);
    display.setCursor( 48 - (display.getPrintWidth("Calculating:    %") / 2), 52);
    //display.setCursor( 48 - (display.getPrintWidth("Calculating:    %") / 2), 32 - (display.getFontHeight() / 2));
    if ( !heart_no == 6){
      drawBuffer(heart_list[heart_no],30,20);
    } else{
      drawBuffer(heart_list[heart_no],28,18);
    }
    display.print("Calculating: " + String(percentage) + "%");
    if ( (buttonsstartTime - currentTime) > 5000 ) {
      delay(500);
      calculatelove();
      break;
    }
  }
  if (buttonspressed == 1) {
    display.clearScreen();
    buttonspressed = 0;
  }
  display.setCursor( 48 - (display.getPrintWidth("Press both the top") / 2), 0);
  display.print("Press both the top"); 
  display.setCursor( 48 - (display.getPrintWidth("right and left") / 2), 10);
  display.print("right and left");
  display.setCursor( 48 - (display.getPrintWidth("buttons to obtain") / 2), 20);
  display.print("buttons to obtain");
  display.setCursor( 48 - (display.getPrintWidth("your love score!") / 2), 30);
  display.print("your love score!");
}

void calculatelove() {
  int loveScore = rand() % 100 + 1;
  display.clearScreen();
  display.setCursor( 48 - (display.getPrintWidth("The love score is:") / 2), 0);
  display.print("The love score is:");
  display.setCursor( 48 - (display.getPrintWidth("  ") / 2), 20);
  display.print(loveScore);
  delay(10000);
  display.clearScreen();
}

void number4d() {
  int randomnumber1 = rand() % 10;
  int randomnumber2 = rand() % 10;
  int randomnumber3 = rand() % 10;
  int randomnumber4 = rand() % 10;
  display.clearScreen();
  display.setCursor( 48 - (display.getPrintWidth("Your 4d number is:") / 2), 0);
  display.print("Your 4d number is:");
  display.setCursor( 48 - (display.getPrintWidth("    ") / 2), 20);
  display.print(String(randomnumber1) + String(randomnumber2) + String(randomnumber3) + String(randomnumber4));
  delay(10000);
  display.clearScreen();
}
