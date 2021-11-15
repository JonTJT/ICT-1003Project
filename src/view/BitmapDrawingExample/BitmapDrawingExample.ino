#include <TinyScreen.h>
#include <SPI.h>
#include <Wire.h>
#include "bitmaps.h"

TinyScreen display = TinyScreen(TinyScreenDefault); //Initialise the display
#include "drawSprites.h" //Put this include only after the display, will not work unless this is after

#define brightness 2

//SpriteList for your convenience, will update whenever i add a new sprite:
//format: spritename(use this in your drawBuffer function), dimensions(x,y), description(for you to identify the sprite)

//test - 10x10 filled yellow rectange
//crystalBall - 20x20 crystal ball
//eightBall - 20x20 eight ball
//heart_1 - 30x25 0% filled heart (empty)
//heart_2 - 30x25 25% filled heart
//heart_3 - 30x25 50% filled heart
//heart_4 - 30x25 75% filled heart
//heart_5 - 30x25 100% filled heart
//heart_6 - 34x29 Glowing filled heart (Note that this is bigger for the "Glowing" effect)
//wizard - 34x27 wizard/fortune teller man
//wizard_flipped - 34x27 flipped version of the wizard/fortune teller man
//bigWizard - 34x54 big wizard/fortune teller man
//bigWizard_flipped - 34x54 flipped version of the big wizard/fortune teller man

void setup() {
  Wire.begin();
  display.begin(); //Start the display
  display.setBrightness(brightness); //Set brightness of the screen
  display.setBitDepth(TSBitDepth16); //Set color bit depth to 16-bit color scheme. Bitmap colors will not display correctly unless this is set.
  display.setColorMode(TSColorModeRGB); //Set color mode to RGB. Bitmap colors will not display correctly unless this is set.
  display.setFlip(1); //Flip display
}

void loop() {
  //Use the drawBuffer function to display sprites. 
  //Parameters ==> sprite bitmap, start_x, start_y
  // bitmap - The bitmap you wish to display on the screen
  // start_x - The x starting value at which the sprite should be displayed. x=0 is the leftmost side of the screen
  // start_y - The y starting value at which the sprite should be displayed. y=0 is the topmost side of the screen
  drawBuffer(heart_1,33,20);
  delay(1000);
  drawBuffer(heart_2,33,20);
  delay(1000);
  drawBuffer(heart_3, 33,20);
  delay(1000);
  drawBuffer(heart_4, 33,20);
  delay(1000);
  drawBuffer(heart_5, 33,20);
  delay(1000);
  drawBuffer(heart_6, 31,18); // -2 on x axis and -2 on y axis due to the sprite being two pixels bigger on the left and right and two pixels bigger on top and bottom
  delay(1000);
}
