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

//Library must be passed the board type
//TinyScreenDefault for TinyScreen shields
//TinyScreenAlternate for alternate address TinyScreen shields
//TinyScreenPlus for TinyScreen+
TinyScreen display = TinyScreen(TinyScreenDefault);

void setup(void) {
  Wire.begin();//initialize I2C before we can initialize TinyScreen- not needed for TinyScreen+
  display.begin();
  //setBrightness(brightness);//sets main current level, valid levels are 0-15
  display.setBrightness(10);
  display.setFont(thinPixel7_10ptFontInfo);
  display.fontColor(TS_8b_White, TS_8b_Black);
}

void loop() {
  buttonLoop();
}

void buttonLoop() {
  unsigned long currentTime;
  unsigned long buttonsstartTime = 0;
  int buttonspressed = 0;
  int percentage;
  currentTime = millis();

  // Clear the screen if the buttons are not pressed.
  if (buttonpressed == 1) {
    display.clearScreen();
    buttonpressed = 0;
  }

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
    display.setCursor( 48 - (display.getPrintWidth("Calculating:    %") / 2), 32 - (display.getFontHeight() / 2));
    display.print("Calculating: " + String(percentage) + "%");
    if ( (buttonsstartTime - currentTime) > 5000 ) {
      delay(500);
      calculatelove();
      break;
    }
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
