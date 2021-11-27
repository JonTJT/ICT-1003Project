#include <TinyScreen.h>
#include <SPI.h>
#include <Wire.h>

TinyScreen display = TinyScreen(TinyScreenDefault);
#include "Bitmaps.h"
#include "drawSprites.h"
#include "BMA250.h"
BMA250 accel_sensor;

int wizard_x = 45;          //Global x coordinate for wizard
int wizard_y = 38;          //Global y coordinate for wizard
int gravity = 1;            // Determines if screen if flipped or not, 1 is flipped 0 is not flipped
int x_accel = 0;            //Accelerometer value for x axis
int walking_delay = 150;    //Delay between walking frames. Lesser is smoother animation, but he moves very fast
int punished = 0;           //If 1 he is punished, and will say something nice next.

const uint8_t upButton     = TSButtonUpperRight;
const uint8_t downButton   = TSButtonLowerRight;
const uint8_t backButton   = TSButtonUpperLeft;
const uint8_t selectButton = TSButtonLowerLeft;
const uint8_t menuButton   = TSButtonLowerLeft;

#if defined(ARDUINO_ARCH_AVR)
  #define SerialMonitorInterface Serial
#elif defined(ARDUINO_ARCH_SAMD)
  #define SerialMonitorInterface SerialUSB
#endif

//DEBUG
#define menu_debug_print true // Debug messages in Serial Monitor for ease of debugging

void setup() {
  // put your setup code here, to run once:
  
  randomSeed(analogRead(0)); // Make it more random - Keefe's random code
  
  Wire.begin();
  SerialMonitorInterface.begin(9600); //DEBUG PURPOSE

  //Keefe part
  display.begin();
  display.setBrightness(8);
  display.clearScreen();
  display.setBitDepth(TSBitDepth16);
  display.setColorMode(TSColorModeRGB);   
  display.setFlip(gravity);
  display.initDMA();
  uint16_t time = millis();
  // Set up the BMA250 acccelerometer sensor
  accel_sensor.begin(BMA250_range_2g, BMA250_update_time_64ms); 

  display.clearScreen();
  drawBuffer(wizard,wizard_x,wizard_y);
  display.setFont(thinPixel7_10ptFontInfo);
  display.fontColor(TS_16b_Green,TS_16b_Black);
  display.setCursor(0,10);
  display.print("Why hello there...");
  display.setCursor(0,20);
  display.print("I'm Ah Huat!");
  delay(5000);
}

void loop() {
  // put your main code here, to run repeatedly:
  checkButtons(500);
  gravity_check();
  idle();   //INPUT MY MENU CODE IN THE MIDDLE OF IDLE USING MENU CHECKER FUNCTION
  checkButtons(500);
}
