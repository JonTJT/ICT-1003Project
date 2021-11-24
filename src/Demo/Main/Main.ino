#include <TinyScreen.h>
#include <SPI.h>
#include <Wire.h>

TinyScreen display = TinyScreen(TinyScreenDefault);
#include "Bitmaps.h"
#include "drawSprites.h"
#include "BMA250.h"
BMA250 accel_sensor;
int wizard_x = 45; //Global x coordinate for wizard
int wizard_y = 38; //Global y coordinate for wizard
int gravity = 1; // Determines if screen if flipped or not, 1 is flipped 0 is not flipped
int x_accel = 0; //Accelerometer value for x axis
int walking_delay = 150; //Delay between walking frames. Lesser is smoother animation, but he moves very fast
int bad_check = 1; //Temp var, so he doesnt repeat the bad line.
int good_check = 0; //Same as bad_check but for good line
int punish_check = 0;

const uint8_t upButton     = TSButtonUpperRight;
const uint8_t downButton   = TSButtonLowerRight;
const uint8_t backButton   = TSButtonUpperLeft;
const uint8_t selectButton = TSButtonLowerLeft;
const uint8_t menuButton   = TSButtonLowerLeft;

//DEBUG
#define menu_debug_print true // Debug messages in Serial Monitor for ease of debugging

#if defined(ARDUINO_ARCH_AVR)
  #define SerialMonitorInterface Serial
#elif defined(ARDUINO_ARCH_SAMD)
  #define SerialMonitorInterface SerialUSB
#endif


// KEEFE FUNCTIONS ------------------------------------------------------------------------------------------------------------------------------------------------------------------
void gravity_check() {
  // This function flips the display, and plays a falling animation before continuing idle
  int dist_from_centre_xaxis = 0;
  accel_sensor.read();//This function gets new data from the acccelerometer
  x_accel = accel_sensor.X;
  if (x_accel == -1) {
    // Print error message to Serial Monitor
    display.print("ERROR! NO BMA250 DETECTED!");
  }

  // Check if flipped already
  if (x_accel <= -200 && gravity != 1) {
    gravity = 1;
  } else if (x_accel >= 200 && gravity != 0) {
    gravity = 0;
  } else {
    return;
  }

  falling();
  display.clearScreen();    
  display.setFlip(gravity);    
  

  //Mirror x value across centre for flip
  //dist_from_centre_xaxis = 48 - wizard_x;
  //wizard_x = wizard_x - (2 * dist_from_centre_xaxis);
  
  drawBuffer(wizard, wizard_x, wizard_y);
  delay(1000);
  display.setCursor(0,10);
  display.print("Ow! Hey!");
  delay(1500);
  display.setCursor(0,20);
  display.print("That hurt T-T");
  delay(3000);
  display.clearWindow(0,0,96,30);
  punish_check = 1;
}

// This functions animates the wiz to be falling
void falling() {
  for (int i = 30; i >= -15; i -= 5) {
    drawBuffer(wizard, wizard_x, i);
    delay(100);
  }
  delay(1000);
  drawBuffer(wizard, wizard_x + 3, -15);
  delay(500);
  drawBuffer(wizard, wizard_x - 3, -15);
  delay(500);
  drawBuffer(wizard, wizard_x + 3, -15);
  delay(500);
  drawBuffer(wizard, wizard_x - 3, -15);
  
  delay(3000);
}

//Currently used for walking
void idle(){  
  int max_walk = 0, rand_walk =0, bound = 0;
  
  //Get wizards coordinates, decide how far can he walk in either direction
  int walk_direction = random(2); //0 = left, 1 = right
  if (walk_direction == 0){                                    //Walk left
    max_walk = (wizard_x - 5) / 5;
    rand_walk = random(max_walk);
    bound = wizard_x - (rand_walk * 5);
    if (bound >= wizard_x) {
//      delay(1000);
      for (int i = 0; i < 100; i++) {
        checkButtons();
      }
      display.clearWindow(0,0,96,30);
      return;
    }
 
    for (wizard_x; wizard_x > bound; wizard_x -= 1) {
      drawBuffer(wizard, wizard_x, wizard_y);
      
      checkButtons();
      
      gravity_check();
      delay(walking_delay);
    }
  } else {                                                     //Walk Right
    max_walk = (75 - wizard_x) / 5;
    rand_walk = random(max_walk);
    bound = wizard_x + rand_walk * 5;
    if (bound <= wizard_x) {
//      delay(1000);
      for (int i = 0; i < 100; i++) {
        checkButtons();
      }
      for (int i = 0; i < 200; i++) {
        checkButtons();
      }
      display.clearWindow(0,0,96,30);
      return;
    }
    for (wizard_x; wizard_x < bound; wizard_x += 1) {
      drawBuffer(wizard_flipped, wizard_x, wizard_y);

      checkButtons();
      
      gravity_check();
      delay(walking_delay);
      
    }
  }
  //Make him talk
  //talk();
}

// Fn to make him say random things
void talk_bad() {
  int rand_word; //The +2 is to add chance that he won't say anything
  rand_word = 0;
  switch(rand_word) {
    case 0:
      display.setCursor(0,10);
      display.print("Your palm says...");
      delay(1000);
      display.setCursor(0,20);
      display.print("you gained weight");
      delay(3000);
      display.clearWindow(0,0,96,30);
      break;
    default:
      break;
  }
}

void talk_good() {
  int rand_word;; //The +2 is to add chance that he won't say anything
  rand_word = 0;
  switch(rand_word) {
    case 0:
      display.setCursor(0,10);
      display.print("T-the stars align!");
      delay(2000);
      display.setCursor(0,20);
      display.print("You will meet your");
      display.setCursor(0,30);
      display.print("uh...");
      delay(2000);
      display.setCursor(0,30);
      display.print("s-soulmate soon!");
      delay(3000);
      display.clearWindow(0,0,96,40);
      break;
    default:
      break;
  }
}

// Fn to make him say random things
void talk() {
  int rand_word = random(7+2); //The +2 is to add chance that he won't say anything
  // To test specific cases if need be -> rand_word = 6;
  switch(rand_word) {
    case 0:
      display.setCursor(0,10);
      display.print("I predict that...");
//      delay(1000);
      for (int i = 0; i < 100; i++){
        checkButtons();
      }
      display.setCursor(0,20);
      display.print("you gained weight");
//      delay(3000);
      for (int i = 0; i < 300; i++){
        checkButtons();
      }
      display.clearWindow(0,0,96,30);
      break;
    case 1:
      display.setCursor(0,10);
      display.print("I'm not happy...");
//      delay(1000);
      for (int i = 0; i < 100; i++){
        checkButtons();
      }
      display.setCursor(0,20);
      display.print("but I wanna be");
//      delay(3000);
      for (int i = 0; i < 300; i++){
        checkButtons();
      }
      display.clearWindow(0,0,96,30);
      break;
    case 2:
      display.setCursor(0,10);
      display.print("The stars tell me...");
//      delay(1000);
      for (int i = 0; i < 100; i++){
        checkButtons();
      }
      display.setCursor(0,20);
      display.print("its a good day");
//      delay(3000);
      for (int i = 0; i < 300; i++){
        checkButtons();
      }
      display.clearWindow(0,0,96,30);
      break;
    case 3:
      display.setCursor(0,10);
      display.print("My magic ball says..");
//      delay(1000);
      for (int i = 0; i < 100; i++){
        checkButtons();
      }
      display.setCursor(0,20);
      display.print("low battery? Wut");
//      delay(3000);
      for (int i = 0; i < 300; i++){
        checkButtons();
      }
      display.clearWindow(0,0,96,30);
      break;
    case 4:
      display.setCursor(0,10);
      display.print("Your ICT1003...");
//      delay(1000);
      for (int i = 0; i < 100; i++){
        checkButtons();
      }
      display.setCursor(0,20);
      display.print("GETS A+ BABY!!!");
//      delay(3000);
      for (int i = 0; i < 300; i++){
        checkButtons();
      }
      display.clearWindow(0,0,96,30);
      break;
    case 5:
      display.setCursor(0,10);
      display.print("Have a 'STUN SEED'");
//      delay(1000);
      for (int i = 0; i < 100; i++){
        checkButtons();
      }
      display.setCursor(0,20);
      display.print("backwards");
      display.setCursor(0,30);
//      delay(3000);
      for (int i = 0; i < 300; i++){
        checkButtons();
      }
      display.print("gottem");
//      delay(2000);
      for (int i = 0; i < 200; i++){
        checkButtons();
      }
      display.clearWindow(0,0,96,40);
      break;
    case 6:
      display.setCursor(0,10);
      display.print("U from tennessee?");
//      delay(1000);
      for (int i = 0; i < 100; i++){
        checkButtons();
      }
      display.setCursor(0,20);
      display.print("Cos u the only 10");
      display.setCursor(0,30);
      display.print("i see");
//      delay(5000);
      for (int i = 0; i < 500; i++){
        checkButtons();
      }
      display.clearWindow(0,0,96,40);
      break;
    default:
      break;
  }
}

// MR FUNCTIONS ------------------------------------------------------------------------------------------------------------------------------------------------------------------

void initHomeScreen() {
//  displayBuffer.clearWindow(0, 0, 96, 5);
  //Show wizard
  display.clearScreen();
  drawBuffer(wizard,wizard_x,wizard_y);
  delay(1000);
}

//void checkButtons() {
//  byte buttons = display.getButtons();
//  if (buttonReleased && buttons) {
//    buttonPress(buttons);
//    buttonReleased = 0;
//  }
//  if (!buttonReleased && !(buttons & 0x1F)) {
//    buttonReleased = 1;
//  }
//}

void writeArrows() {
  leftArrow(0, 15 + 2);
  rightArrow(0, 45 + 5);

  upArrow(90, 15 + 2);
  downArrow(90, 45 + 4);
}

//Function for drawing the arrows based on X and Y axis of screen
void leftArrow(int x, int y) {
//  displayBuffer.drawLine(x + 2, y - 2, x + 2, y + 2, 0xFFFF);
//  displayBuffer.drawLine(x + 1, y - 1, x + 1, y + 1, 0xFFFF);
//  displayBuffer.drawLine(x + 0, y - 0, x + 0, y + 0, 0xFFFF);
  display.drawLine(x + 2, y - 2, x + 2, y + 2, 0xFFFF);
  display.drawLine(x + 1, y - 1, x + 1, y + 1, 0xFFFF);
  display.drawLine(x + 0, y - 0, x + 0, y + 0, 0xFFFF);
}

void rightArrow(int x, int y) {
//  displayBuffer.drawLine(x + 0, y - 2, x + 0, y + 2, 0xFFFF);
//  displayBuffer.drawLine(x + 1, y - 1, x + 1, y + 1, 0xFFFF);
//  displayBuffer.drawLine(x + 2, y - 0, x + 2, y + 0, 0xFFFF);
  display.drawLine(x + 0, y - 2, x + 0, y + 2, 0xFFFF);
  display.drawLine(x + 1, y - 1, x + 1, y + 1, 0xFFFF);
  display.drawLine(x + 2, y - 0, x + 2, y + 0, 0xFFFF);
}

void upArrow(int x, int y) {
//  displayBuffer.drawLine(x + 0, y - 0, x + 4, y - 0, 0xFFFF);
//  displayBuffer.drawLine(x + 1, y - 1, x + 3, y - 1, 0xFFFF);
//  displayBuffer.drawLine(x + 2, y - 2, x + 2, y - 2, 0xFFFF);
  display.drawLine(x + 0, y - 0, x + 4, y - 0, 0xFFFF);
  display.drawLine(x + 1, y - 1, x + 3, y - 1, 0xFFFF);
  display.drawLine(x + 2, y - 2, x + 2, y - 2, 0xFFFF);
}
void downArrow(int x, int y) {
//  displayBuffer.drawLine(x + 0, y + 0, x + 4, y + 0, 0xFFFF);
//  displayBuffer.drawLine(x + 1, y + 1, x + 3, y + 1, 0xFFFF);
//  displayBuffer.drawLine(x + 2, y + 2, x + 2, y + 2, 0xFFFF);
  display.drawLine(x + 0, y + 0, x + 4, y + 0, 0xFFFF);
  display.drawLine(x + 1, y + 1, x + 3, y + 1, 0xFFFF);
  display.drawLine(x + 2, y + 2, x + 2, y + 2, 0xFFFF);
}

// Wesley FUNCTIONS -------------------------------------------------------------------------------------------------------------------------------------------------------------
void checkButtons() {
  byte buttons = display.getButtons();
  if (buttons == TSButtonUpperRight || buttons == TSButtonLowerRight || buttons == TSButtonUpperLeft || buttons == TSButtonLowerLeft) {
    drawMenu();
  }
}

void drawMenu() {
  display.clearScreen();
  
  int currentSelection = 0;
  int buttonsPressed = 0;

  while (1) {
    switch (currentSelection) {
    case 0:
      display.setCursor( 48 - (display.getPrintWidth(">Lucky 4D<") / 2), 15);
      display.print(">Lucky 4D<"); 
      display.setCursor( 48 - (display.getPrintWidth("Horoscope") / 2), 25);
      display.print("Horoscope");
      display.setCursor( 48 - (display.getPrintWidth("Love Calculator") / 2), 35);
      display.print("Love Calculator");
      display.setCursor( 48 - (display.getPrintWidth("Ask 8 Ball") / 2), 45);
      display.print("Ask 8 Ball");
      break;
    case 1:
      display.setCursor( 48 - (display.getPrintWidth("Lucky 4D") / 2), 10);
      display.print("Lucky 4D"); 
      display.setCursor( 48 - (display.getPrintWidth(">Horoscope<") / 2), 20);
      display.print(">Horoscope<");
      display.setCursor( 48 - (display.getPrintWidth("Love Calculator") / 2), 30);
      display.print("Love Calculator");
      display.setCursor( 48 - (display.getPrintWidth("Ask 8 Ball") / 2), 40);
      display.print("Ask 8 Ball");
      break;
    case 2:
      display.setCursor( 48 - (display.getPrintWidth("Lucky 4D") / 2), 5);
      display.print("Lucky 4D"); 
      display.setCursor( 48 - (display.getPrintWidth("Horoscope") / 2), 15);
      display.print("Horoscope");
      display.setCursor( 48 - (display.getPrintWidth(">Love Calculator<") / 2), 25);
      display.print(">Love Calculator<");
      display.setCursor( 48 - (display.getPrintWidth("Ask 8 Ball") / 2), 35);
      display.print("Ask 8 Ball");
      break;
    case 3:
      display.setCursor( 48 - (display.getPrintWidth("Lucky 4D") / 2), 0);
      display.print("Lucky 4D"); 
      display.setCursor( 48 - (display.getPrintWidth("Horoscope") / 2), 10);
      display.print("Horoscope");
      display.setCursor( 48 - (display.getPrintWidth("Love Calculator") / 2), 20);
      display.print("Love Calculator");
      display.setCursor( 48 - (display.getPrintWidth(">Ask 8 Ball<") / 2), 30);
      display.print(">Ask 8 Ball<");
      break;
    default:
      break;
    }
  
    writeArrows();
    
    if (display.getButtons(downButton) && currentSelection < 3) {
      currentSelection++;
      buttonsPressed = 1;
      display.clearScreen();
      delay(300);
    }
    else if (display.getButtons(upButton) && currentSelection > 0) {
      currentSelection--;
      buttonsPressed = 1;
      display.clearScreen();
      delay(300);
    }
    else if (display.getButtons(selectButton)) {
      switch(currentSelection){
        case 0:
        SerialMonitorInterface.println("Do 4dnumber");
        number4d();
        break;
        case 1:
        SerialMonitorInterface.println("Do horoscope");
        horoscope();
        break;
        case 2:
        SerialMonitorInterface.println("Do LoveCalculator");
        LoveCalculator();
        break;
        case 3:
        SerialMonitorInterface.println("Do Ask8Ball");
        Ask8Ball();
        break;
      }
    }
    else if (display.getButtons(backButton)) {
      initHomeScreen();
      break;
    }
  }
}

// SETUP AND LOOP ------------------------------------------------------------------------------------------------------------------------------------------------------------------

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
  display.print("I'm Saicik!");
  delay(5000);
  
//  attachInterrupt(TSP_PIN_BT1, drawMenu, FALLING);
//  attachInterrupt(TSP_PIN_BT2, drawMenu, FALLING);
//  attachInterrupt(TSP_PIN_BT3, drawMenu, FALLING);
//  attachInterrupt(TSP_PIN_BT4, drawMenu, FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:
  checkButtons();
  gravity_check();
  idle();   //INPUT MY MENU CODE IN THE MIDDLE OF IDLE USING MENU CHECKER FUNCTION
  if (good_check == 1 && punish_check == 1) {
    good_check = 0;
    talk_good();
  }
  if (bad_check == 1) {
    bad_check = 0;
    good_check = 1;
    talk_bad();
  }
  
  checkButtons();
}
