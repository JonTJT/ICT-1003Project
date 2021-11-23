#include <TinyScreen.h>
#include <SPI.h>
#include <Wire.h>
#include "Bitmaps.h"
#include "BMA250.h"

// KEEFE PART ------------------------------------------------------------------------------------------------------
BMA250 accel_sensor;
int wizard_x = 45; //Global x coordinate for wizard
int wizard_y = 38; //Global y coordinate for wizard
int gravity = 1; // Determines if screen if flipped or not, 1 is flipped 0 is not flipped
int x_accel = 0; //Accelerometer value for x axis
int walking_delay = 150; //Delay between walking frames. Lesser is smoother animation, but he moves very fast

// MENG RONG PART --------------------------------------------------------------------------------------------------
#include <GraphicsBuffer.h>
GraphicsBuffer displayBuffer = GraphicsBuffer(96, 64, colorDepth8BPP);

TinyScreen display = TinyScreen(TinyScreenDefault);
#include "drawSprites.h"

const uint8_t upButton     = TSButtonUpperRight;
const uint8_t downButton   = TSButtonLowerRight;
const uint8_t backButton   = TSButtonUpperLeft;
const uint8_t selectButton = TSButtonLowerLeft;
const uint8_t menuButton   = TSButtonLowerLeft;

//uint8_t displayOn = 0;          
uint8_t buttonReleased = 1;     
//uint8_t rewriteMenu = false;

//Value of Page States
const uint8_t displayStateHome = 0x01;
const uint8_t displayStateMenu = 0x02;

uint8_t currentDisplayState = displayStateHome;

//DEBUG
#define menu_debug_print true // Debug messages in Serial Monitor for ease of debugging
#if defined(ARDUINO_ARCH_AVR)
  #define SerialMonitorInterface Serial
#elif defined(ARDUINO_ARCH_SAMD)
  #define SerialMonitorInterface SerialUSB
#endif

  //  DATA from menu.h file
  void (*menuHandler)(uint8_t) = NULL;
  uint8_t (*editorHandler)(uint8_t, int*, char*, void (*)()) = NULL;

  //  Declaration of variables -------------------------------------------------
  // 8 Y-axis rows each separated with a -1 pixel apart
  uint8_t menuTextY[8] = {1 * 8 - 1, 2 * 8 - 1, 3 * 8 - 1, 4 * 8 - 1, 5 * 8 - 1, 6 * 8 - 1, 7 * 8 - 3, 8 * 8 - 3};

  uint16_t defaultFontColor = TS_16b_White;
  uint16_t defaultFontBG = TS_16b_Black;
  uint16_t inactiveFontColor = TS_16b_Gray;
  uint16_t inactiveFontBG = TS_16b_Black;
  
  uint8_t menuHistory[5];
  uint8_t menuHistoryIndex = 0;
  uint8_t menuSelectionLineHistory[5];
  int currentMenu = 0;
  int currentMenuLine = 0;
  int lastMenuLine = -1;
  int currentSelectionLine = 0;
  int lastSelectionLine = -1;

  // For Rendering the Menu 
  int changeDir;
  int changeEnd;
  int changeStart;
  int yChange;

  bool needMenuDraw = true;

  //Main Menu lines selection
  void MainMenu(uint8_t selection) {
    if (menu_debug_print)SerialMonitorInterface.println("exampleMenuHandler");
    if (selection == 0) {
      //Feature 1 - Daily Lucky Number
    }
    if (selection == 1) {
      //Feature 2 - Daily Horoscope
    }
    if (selection == 2) {
      //Feature 3 - Love Calculator
    }
    if (selection == 3) {
      //Feature 4 - Ask 8 Ball
    }
  }

  // Use this struct outline to create menus
  typedef struct
  {
    const uint8_t amtLines;
    const char* const * strings;
    //Creating the Struct will call the function specified in selectionHandler
    void (*selectionHandler)(uint8_t);
  } menu_info;

  // Main Menu variables 
  static const char PROGMEM MenuStrings0[] = "Daily Lucky Number";
  static const char PROGMEM MenuStrings1[] = "Daily Horoscope";
  static const char PROGMEM MenuStrings2[] = "Love Calculator";
  static const char PROGMEM MenuStrings3[] = "Ask 8 Ball";
  
  // MenuStrings Array hold all MenuString Strings
  static const char* const PROGMEM MenuStrings[] = {MenuStrings0, MenuStrings1, MenuStrings2, MenuStrings3};
  
  // Declare MenuInfo Object
  const menu_info MenuInfo = {4, MenuStrings, MainMenu};

  // menu_info Obj Array ------------------------------------------------------
  const menu_info menuList[] = {MenuInfo};
  #define MainMenuIndex 0

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
  display.setFlip(gravity);    
  display.clearScreen();

  //Mirror x value across centre for flip
  //dist_from_centre_xaxis = 48 - wizard_x;
  //wizard_x = wizard_x - (2 * dist_from_centre_xaxis);
  
  drawBuffer(wizard, wizard_x, wizard_y);
  delay(1000);
  display.setCursor(0,10);
  display.print("Uh...RUDE!?");
  delay(3000);
  display.setCursor(0,20);
  display.print("That hurt T-T");
  delay(3000);
  display.clearWindow(0,0,96,30);
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
      delay(1000);
      display.setCursor(20,10);
      display.print("*zzzzzzzz*");
      delay(2000);
      display.clearWindow(0,0,96,30);
      return;
    }
 
    for (wizard_x; wizard_x > bound; wizard_x -= 1) {
      drawBuffer(wizard, wizard_x, wizard_y);
      gravity_check();
      delay(walking_delay);
    }
  } else {                                                     //Walk Right
    max_walk = (75 - wizard_x) / 5;
    rand_walk = random(max_walk);
    bound = wizard_x + rand_walk * 5;
    if (bound <= wizard_x) {
      delay(1000);
      display.setCursor(15,10);
      display.print("*lalalalala*");
      delay(2000);
      display.clearWindow(0,0,96,30);
      return;
    }
    for (wizard_x; wizard_x < bound; wizard_x += 1) {
      drawBuffer(wizard_flipped, wizard_x, wizard_y);
      gravity_check();
      delay(walking_delay);
      
    }
  }
  //Make him talk
    talk();
}


// Fn to make him say random things
void talk() {
  int rand_word = random(7+2); //The +2 is to add chance that he won't say anything
  // To test specific cases if need be -> rand_word = 6;
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
      display.print("its a good day");
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
    case 5:
      display.setCursor(0,10);
      display.print("Have a 'STUN SEED'");
      delay(1000);
      display.setCursor(0,20);
      display.print("backwards");
      display.setCursor(0,30);
      delay(3000);
      display.print("gottem");
      delay(2000);
      display.clearWindow(0,0,96,40);
      break;
    case 6:
      display.setCursor(0,10);
      display.print("U from tennessee?");
      delay(1000);
      display.setCursor(0,20);
      display.print("Cos u the only 10");
      display.setCursor(0,30);
      display.print("i see");
      delay(5000);
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


void checkButtons() {
  byte buttons = display.getButtons();
  if (buttonReleased && buttons) {
    buttonPress(buttons);
    buttonReleased = 0;
  }
  if (!buttonReleased && !(buttons & 0x1F)) {
    buttonReleased = 1;
  }       
}

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






// SETUP AND LOOP ------------------------------------------------------------------------------------------------------------------------------------------------------------------

void setup() {
  // put your setup code here, to run once:
  
  randomSeed(analogRead(0)); // Make it more random - Keefe's random code
  
  Wire.begin();
  SerialMonitorInterface.begin(4600); //DEBUG PURPOSE

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
  accel_sensor.begin(BMA250_range_2g, BMA250_update_time_32ms); 


//  if (displayBuffer.begin()) {
//    //memory allocation error- buffer too big
//    while (1) {
//      SerialMonitorInterface.println("Display buffer memory allocation error!");
//      delay(1000);
//    }
//  }
//  displayBuffer.setFont(thinPixel7_10ptFontInfo);
//  displayBuffer.clear();
}

void loop() {
  // put your main code here, to run repeatedly:

  display.clearScreen();
  drawBuffer(wizard,wizard_x,wizard_y);
  display.setFont(thinPixel7_10ptFontInfo);
  display.fontColor(TS_16b_Green,TS_16b_Black);
  display.setCursor(0,10);
  display.print("Why hello there...");
  display.setCursor(0,20);
  display.print("I'm Saicik!");
  delay(3000);
  
  // Here is the infinite loop for idle, change to for loop if want limit loops.
  while (1){
    if (currentDisplayState == displayStateHome) {
      gravity_check();
      idle();   //INPUT MY MENU CODE IN THE MIDDLE OF IDLE USING MENU CHECKER FUNCTION
    } else {
      drawMenu();
    }
    checkButtons();
  }  
}

//Loop menu checker
//void MenuChecker() {
//  if (currentDisplayState == displayStateHome) {
//    if ( display.getButtons(TSButtonLowerRight) || display.getButtons(TSButtonUpperRight) || display.getButtons(TSButtonLowerLeft) || display.getButtons(TSButtonUpperLeft)){
//      displayBuffer.clear();  // if a button is pressed, clear the screen
//    }
//    updateMainDisplay(); 
//    display.writeBufferDMA(displayBuffer.getBuffer(), displayBuffer.getBufferSize());
//    delay (500); // Making this smaller will make the screen more scattered, making it longer will mean you need to hold in buttons longer
//  } else {
//    drawMenu(); //Found in .h file
//  }
//  checkButtons();
//  updateMainDisplay();
//}

//Together -----------------------------------------------------------------------------------------------------
void drawMenu() {
  //for (int yChange = changeStart; yChange != changeEnd; yChange += changeDir) {
  if (needMenuDraw) {
    display.goTo(0,0);
    needMenuDraw = false;
//    display.clearScreen();
//    displayBuffer.clear();
    

    for (int i = 0; i < menuList[currentMenu].amtLines; i++) {
      // Highlight currently selected option
      if (i == currentSelectionLine) {
//        displayBuffer.fontColor(defaultFontColor, ALPHA_COLOR);
//        display.fontColor(TS_8b_Green,TS_8b_Black);
      } else {
//        displayBuffer.fontColor(inactiveFontColor, ALPHA_COLOR);
//        display.fontColor(inactiveFontColor, ALPHA_COLOR);
      }
      
      //write the selection string to display
      char buffer[20];
      if (menu_debug_print)SerialMonitorInterface.print("int currentMenu: ");
      if (menu_debug_print)SerialMonitorInterface.println(currentMenu);
      if (menu_debug_print)SerialMonitorInterface.print("int currentMenuLine: ");
      if (menu_debug_print)SerialMonitorInterface.println(currentMenuLine);
      strcpy_P(buffer, (PGM_P)pgm_read_word(&(menuList[currentMenu].strings[currentMenuLine + i])));
      
      if (menu_debug_print)SerialMonitorInterface.print("Buffer: ");
      if (menu_debug_print)SerialMonitorInterface.println(buffer);
      
//      int width = displayBuffer.getPrintWidth(buffer);      //WORKS
      int width = display.getPrintWidth(buffer);
      
      if (menu_debug_print)SerialMonitorInterface.println(width);
      
//      displayBuffer.setCursor(96 / 2 - width / 2, menuTextY[i] + 5 + yChange - (currentSelectionLine * 8) + 16);
//      display.setCursor(96 / 2 - width / 2, menuTextY[i] + 5 + yChange - (currentSelectionLine * 8) + 16);
      display.setCursor(96 / 2 - width / 2, menuTextY[i] + 5 + yChange - (currentSelectionLine * 8) + 16);
//      displayBuffer.print(buffer);
      display.print(buffer);
    }

    writeArrows();
  }
  
  //yChange and variables set by viewMenu() Function - See Function b4 understanding here
  if (yChange != changeEnd) {
    if (abs(yChange - changeEnd) > 5) {
      yChange += changeDir;
    }
    if (abs(yChange - changeEnd) > 3) {
      yChange += changeDir;
    }
    yChange += changeDir;
    needMenuDraw = true;
  }
//  displayBuffer.fontColor(0xFFFF, ALPHA_COLOR);
}


//Display Menu
void viewMenu(uint8_t button) {           //View the menu
  if (menu_debug_print)SerialMonitorInterface.print("viewMenu ");
  if (menu_debug_print)SerialMonitorInterface.println(button);            //Debugging purposes

  //Changing variable indexes after clicking the button
  if (!button) {
    //Do nothing
  } else {
    if (button == upButton) {
      if (currentSelectionLine > 0) {
        currentSelectionLine--;
      }
    } else if (button == downButton) {
      if (currentSelectionLine < menuList[currentMenu].amtLines - 1) {
        currentSelectionLine++;
      }
    } else if (button == selectButton) {
      if (menu_debug_print)SerialMonitorInterface.print("select ");
      if (menu_debug_print)SerialMonitorInterface.println(currentMenuLine + currentSelectionLine);
      menuList[currentMenu].selectionHandler(currentMenuLine + currentSelectionLine);
    } else if (button == backButton) {
      newMenu(-1);
      if (!menuHistoryIndex)
        return;
    }
  }

  // If not yet seen last line of selection -> draw the menu by setting variables yChange, ....etc.
  if (lastMenuLine != currentMenuLine || lastSelectionLine != currentSelectionLine) {
    if (menu_debug_print)SerialMonitorInterface.println(currentMenu);

    //After pressing button, currentSelectionLine will change, render based on change
    if (currentSelectionLine < lastSelectionLine) {
      changeDir = 1;
      changeEnd = 0;
      changeStart = -7;
    } else {
      changeDir = -1;
      changeEnd = 0;
      changeStart = 7;
    }
    if (lastSelectionLine == -1) {
      changeStart = changeEnd; //new menu, just draw once
    }
    yChange = changeStart;
    needMenuDraw = true;
  }
  lastMenuLine = currentMenuLine;
  lastSelectionLine = currentSelectionLine;
  menuSelectionLineHistory[menuHistoryIndex] = currentSelectionLine;

//  displayBuffer.fontColor(0xFFFF, ALPHA_COLOR);
}

// Function to create menu and store into variables if (-1) index means move back 1 page
void newMenu(int8_t newIndex) {
  currentMenuLine = 0;
  lastMenuLine = -1;
  currentSelectionLine = 0;
  lastSelectionLine = -1;
  if (newIndex >= 0) {          //Depending on either in homepage / menu state
    menuHistory[menuHistoryIndex++] = currentMenu;            //menuHistory = 5 entries - menuHistoryIndex default  = 0
    currentMenu = newIndex;                                   //update the currentMenuNode with mainmenuIndex if first pressed
  } else {
    if (currentDisplayState == displayStateMenu) { 
      menuHistoryIndex--;
      currentMenu = menuHistory[menuHistoryIndex];
    }
  }

  //MenuHistoryIndex anything other than 0, it means that it is in a menu
  if (menuHistoryIndex) {
    SerialMonitorInterface.println("menuHistoryIndex: ");
    SerialMonitorInterface.println(menuHistoryIndex);
    currentDisplayState = displayStateMenu;
    if (menu_debug_print)SerialMonitorInterface.print("New menu index ");
    if (menu_debug_print)SerialMonitorInterface.println(currentMenu);         //debug
    currentSelectionLine = menuSelectionLineHistory[menuHistoryIndex];
  } else {
    if (menu_debug_print)SerialMonitorInterface.print("New menu index ");
    if (menu_debug_print)SerialMonitorInterface.println("home");              //debug
    menuSelectionLineHistory[menuHistoryIndex + 1] = 0;       //Erase the previous history index away
    currentDisplayState = displayStateHome;
    initHomeScreen();
  }
}

//To be used in Main program to identify buttonpress
void buttonPress(uint8_t buttons) {
  if (menu_debug_print)SerialMonitorInterface.println("Buttons Pressed0");
  
  //If current display = HomePage
  if (currentDisplayState == displayStateHome) {
    
    if (buttons == TSButtonUpperRight || buttons == TSButtonLowerRight || buttons == TSButtonUpperLeft || buttons == TSButtonLowerLeft) {        //If buttons clicked is the button declared in main program
      menuHandler = viewMenu;           //Set menuHandler to reference viewMenu Function
      newMenu(MainMenuIndex);      
      menuHandler(0);                   //Call referenced function through menuHandler with argument
      if (menu_debug_print)SerialMonitorInterface.println("Buttons Pressed1");
    }
  }
  //Else if current display = in a Menu page (selecting features)
  else if (currentDisplayState == displayStateMenu) {
    if (menuHandler) {                  
      menuHandler(buttons);
      if (menu_debug_print)SerialMonitorInterface.println("Buttons Pressed2");
    }
  }
}


//--------------------------------------------------------------------------------------------------------------------






//KEEFE FUNCTIONS -------------------------------------------------------------------------------------------
