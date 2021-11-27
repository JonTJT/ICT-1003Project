#include "magic8ball.h"//for magic 8 ball answers
BMA250 accel;

//typedef enum {INITIAL, WAIT, RESPONSE} states;
//states nextState = INITIAL;
long int rando = 0;  //Setting up the random number holder

#define brightness 10

void Ask8Ball() {
    display.clearScreen();
    accel.begin(BMA250_range_2g, BMA250_update_time_64ms); 
    display.setCursor(5,5);
    drawBuffer(eightBall,63,30);//sprite character
    display.print("Magic 8-Ball");
    delay(500);
    while (1) {
      if (display.getButtons() == TSButtonUpperLeft || display.getButtons() == TSButtonUpperRight || display.getButtons() == TSButtonLowerLeft) {
        mainLoop();
        break;
      } 
      else if (display.getButtons() == TSButtonLowerRight) {
        break;
      }
    }
    display.clearScreen();
}

void mainLoop() {
//  display.setFont(liberationSans_10ptFontInfo); //font size is 8
  //display.fontColor(WHITE,BLACK);

  display.clearWindow(0,0,96,64);
  
  while(1) {
    rando = random(0, 17); //You need a random range 1 more than the number of statements you have
    rando = round(rando); //Round it to a whole number

    accel.read(); //Read and manipulate accelerometer data
    int z = accel.Z;
    SerialMonitorInterface.println(z); //printing out Z values for debugging purposes

    if (z < 220) {
      drawBuffer(eightBall,63,30);//sprite character 
      display.setCursor(3,5);
      display.print("Shake");
      display.setCursor(3,15);
      display.print("Shake");
      display.setCursor(3,25);
      display.print("Shake"); 
      delay(500);
      display.clearWindow(0,0,96,64);
    }
    if (z > 220) {
      display.setCursor(5,5); 
      drawBuffer(wizard,63,30);//sprite character
      display.print(phrases[rando]); //Here's the message that gets printed
      SerialMonitorInterface.println(rando);
      while (accel.Z > 0) {
        for (int i = 0; i < 100; i++) {
          if (display.getButtons() == TSButtonLowerRight) {
            return;
          }
        }
        accel.read();
      }
      display.clearWindow(0,0,96,64); //It's important to clear the window after each graphic
    }
  }
}
