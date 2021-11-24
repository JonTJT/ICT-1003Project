#define  BLACK           0x00
#define WHITE           0xFF
//
//#include <TinyScreen.h>
//#include <SPI.h>
//#include <Wire.h>
//#include "Bitmaps.h" //wizard.ino module for sprites
#include "magic8ball.h"//for magic 8 ball answers
//#include "BMA250.h" //Accelerometer support file for Bosch MA 250 accelerometer
BMA250 accel;

// Make Serial Monitor compatible for all TinyCircuits processors
//#if defined(ARDUINO_ARCH_AVR)
//#define SerialMonitorInterface Serial
//#elif defined(ARDUINO_ARCH_SAMD)
//#define SerialMonitorInterface SerialUSB
//#endif

typedef enum {INITIAL, WAIT, RESPONSE} states;
states nextState = INITIAL;

//TinyScreen display = TinyScreen(TinyScreenDefault);
//#include "drawSprites.h" //Put this include only after the display, will not work unless this is after

#define brightness 10

//void setup() {
//  Wire.begin();
//  display.begin();
//  display.setBrightness(brightness); //spirites module
//  display.setBitDepth(TSBitDepth16); //spirites module
//  display.setColorMode(TSColorModeRGB); //spirites module
//  randomSeed(analogRead(1)); //To make our random numbers less predictable, we use randomSeed
//                              //and read an unused pin for a starting value (seed)
//  SerialMonitorInterface.begin(9600);
//  accel.begin(BMA250_range_2g, BMA250_update_time_64ms); 
//  display.setFlip(1); //Flip display
//}

void Ask8Ball() {
  display.clearScreen();
  while(1) {
     if (display.getButtons() != TSButtonLowerRight) {
        mainLoop();
     } else {
        display.clearScreen();
        //Set back the fonts
        display.setFont(thinPixel7_10ptFontInfo);
        display.fontColor(TS_16b_Green,TS_16b_Black);
        break;
     }
   }
}

void mainLoop() {
  display.setFont(liberationSans_10ptFontInfo); //font size is 8
  //display.fontColor(WHITE,BLACK);

    accel.read(); //Read and manipulate accelerometer data
    int z = accel.Z;
    SerialMonitorInterface.println(z); //printing out Z values for debugging purposes
   
    if (z < 0) {display.clearWindow(0,0,96,64); delay(1000);}
    if (z > 220) {


   // switch statement state machine
  switch (nextState) {
  
  // intial state, print magic 8-ball
  case INITIAL:
    display.clearWindow(0,0,96,64);
    display.setCursor(3,5);
    drawBuffer(eightBall,63,30);//sprite character
    display.print("Magic 8-Ball");
    delay(2000);
    nextState = WAIT;
    break;
  
  // do nothing state, wait for interrupt
  case WAIT:
    display.setCursor(9,5);
    magic8ball();
    break;

  // reponse state, determine/print response
  //case RESPONSE:
  //accel.read();
          //while (accel.Z > 160){
            //display.setCursor(12,5); //set the cursor to the top left corner
            //magic8ball();
            //nextState = INITIAL;
           //break;
  //}}
}}}

/* randomly selects from responses and prints it */
void magic8ball() {
  // generate random number between 0 and 19 to select and print random response
  oneLineScroll(0,phrases[random(RESPONSES)]);
  delay(2000);
  display.clearWindow(0,0,96,64); //It's important to clear the window after each graphic
  delay(2000);
}

/* interrupt service routine triggered by button press */
void trigger() {
  nextState = RESPONSE;
}

/*
If string is too large to print, scrolls text on single line.
0 = first line; 1 = second line
Reuse for other projects! 
*/
void oneLineScroll (int line, String text) {
  // set cursor on intended line and print first 16 chars
  display.clearWindow(0,0,96,64);
  drawBuffer(wizard,63,30);//sprite character
  display.setCursor(0, 0);
  // print first 16 characters
  display.print(text.substring(0,15));
  // for every additional letter, shift in from right
  if (text.length()>16) {
    for (int i = 0; i <= (text.length()-16); i++) {
      display.setCursor(0, line);
      display.print(text.substring(i,i+16));
    }
  }
   delay(1500);
}
