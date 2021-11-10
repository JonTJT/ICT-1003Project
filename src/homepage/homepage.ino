#include <TinyScreen.h>
#include <SPI.h>
#include <Wire.h>
#include "Bitmaps.h"

TinyScreen display = TinyScreen(TinyScreenDefault);
int start_x, start_y; //Wizard spawn coordinates
int wizard_x = 60; //Temporary wizard location, later to use global x coordinate var

void setup() {
  randomSeed(analogRead(0)); // Make it more random
  Wire.begin();
  display.begin();
  display.setBrightness(3);
  display.clearScreen();
  display.setBitDepth(TSBitDepth16);
  display.setColorMode(TSColorModeRGB);  
  display.setFlip(1);
  
}

void loop() {
  display.clearScreen();
  drawMap(wizardBitmap,34,54,60,30);
  display.setFont(thinPixel7_10ptFontInfo);
  //display.setFont(liberationSansNarrow_8ptFontInfo);
  display.fontColor(TS_8b_Green,TS_8b_Black);
  display.setCursor(0,10);
  display.print("Why hello there...");
  display.setCursor(0,20);
  display.print("I'm Saicik!");
  delay(5000);
  display.clearWindow(0,0,96,30);
  
//  //y is vertical, x is horizontal. bottom left is origin
//  for (int i = 60; i > 10; i-=20) {
//    drawMap(wizardBitmap,34,54,i,30);
//    delay(500);
//  }
//
//  display.setCursor(3,10);
//  display.print("I predict that...");
//  delay(1000);
//  display.setCursor(3,20);
//  display.print("u gey");
//  delay(3000);
//  display.clearScreen();
//  display.setMirror(1);
//  for (int i = 60; i > 10; i-=20) {
//    drawMap(wizardBitmap,34,54,i,30);
//    delay(500);
//  }
//  display.clearScreen();
//  display.setMirror(0);

  // Here is the infinite loop for idle, change to for loop if want limit loops.
  while (1){
//    display.clearScreen();
//    display.setCursor(3,10);
//    display.print(i);
    idle();
  }

  
  
  
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

void idle(){
  //Currently used for walk, later will be converted to 
  int max_walk = 0, rand_walk =0, bound = 0;
  
  //Get wizards coordinates, decide how far can he walk in either direction
  int walk_direction = random(2); //0 = left, 1 = right
  if (walk_direction == 0){ //Walk left
    max_walk = wizard_x / 5;
    rand_walk = random(max_walk);
    bound = wizard_x - (rand_walk * 5);
    //display.setCursor(30,10);
    //display.print(bound);
    if (bound >= wizard_x) {
      display.setCursor(20,10);
      display.print("*zzzzzzzz*");
      display.clearWindow(0,0,96,30);
      delay(5000);
      return;
    }
 
    for (wizard_x; wizard_x > bound; wizard_x -= 5) {
      //display.setCursor(45,10);
      //display.print(wizard_x);
      drawMap(wizardBitmap,34,54,wizard_x,30);
      delay(500);
    }
  } else { //Walk Right
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
      drawMap(wizardBitmap_flipped,34,54,wizard_x,30);
      delay(500);
    }
  }
  //Make him talk
    talk();
}

void talk() {
  // Fn to make him say random things
  int rand_word = random(5);
//  display.setCursor(3,10);
//  display.print("NEW LOOP");
//  display.setCursor(3,20);
//  display.print(rand_word);
//  delay(1000);
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
