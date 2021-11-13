#define menu_debug_print true // Debug messages in Serial Monitor for ease of debugging


// Declaration of variables -------------------------------------------------
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

//Prototype Functions -------------------------------------------------------
void (*menuHandler)(uint8_t) = NULL;
uint8_t (*editorHandler)(uint8_t, int*, char*, void (*)()) = NULL;


// Struct -------------------------------------------------------------------
// Use this struct outline to create menus
typedef struct
{
  const uint8_t amtLines;
  const char* const * strings;
  //Creating the Struct will call the function specified in selectionHandler
  void (*selectionHandler)(uint8_t);
} menu_info;


/*  Step 1: Create string variables of menu linues 
 *  
 *  Step 2: Create an array to store all the strings in step 1
 *  
 *  Step 3: Declare menu_info Obj
 *  
 *  Step 4: Make a menu_info Obj Array to hold all objs in Step3 if more than 1 menu
 *  const menu_info menuList[] = {exampleMenuInfo, dateTimeMenuInfo, secondExampleMenuInfo};
 *  #define exampleMenuIndex 0
 *  #define dateTimeMenuIndex 1
 *  #define secondExampleMenuIndex 2
  */
// Main Menu Data -----------------------------------------------------------
/*OG SOURCE:
static const char PROGMEM exampleMenuStrings0[] = "Time & Date";
static const char PROGMEM exampleMenuStrings1[] = "Brightness";
static const char PROGMEM exampleMenuStrings2[] = "Menu 2";
static const char PROGMEM exampleMenuStrings3[] = "Option 4";
static const char PROGMEM exampleMenuStrings4[] = "Option 5"; */
// Main Menu variables 
static const char PROGMEM MenuStrings0[] = "Daily Lucky Number";
static const char PROGMEM MenuStrings1[] = "Daily Horoscope";
static const char PROGMEM MenuStrings2[] = "Love Calculator";
static const char PROGMEM MenuStrings3[] = "Ask 8 Ball";

// MenuStrings Array hold all MenuString Strings
static const char* const PROGMEM MenuStrings[] =
{
  MenuStrings0,
  MenuStrings1,
  MenuStrings2,
  MenuStrings3,
};

// Declare MenuInfo Object
const menu_info MenuInfo = {4, MenuStrings, MainMenu};


// menu_info Obj Array ------------------------------------------------------
const menu_info menuList[] = {MenuInfo};
#define MainMenuIndex 0


// Functions ----------------------------------------------------------------

bool needMenuDraw = true;

//To be used in Main program to identify buttonpress
void buttonPress(uint8_t buttons) {
  //If current display = HomePage
  if (currentDisplayState == displayStateHome) {
    if (buttons == viewButton) {        //If buttons clicked is the viewbutton declared in main program
      menuHandler = viewMenu;           //Set menuHandler to reference viewMenu Function
      newMenu(MainMenuIndex);        
      menuHandler(0);                   //Call referenced function through menuHandler with argument
    }
    else if (buttons == menuButton) {   //If buttons clicked is the menuButton declared in main program
      menuHandler = viewMenu;
      newMenu(MainMenuIndex);
      menuHandler(0);
    }
  } 
  //Else if current display = in a Menu page (selecting features)
  else if (currentDisplayState == displayStateMenu || currentDisplayState == displayStateCalibration) {
    if (menuHandler) {                  
      menuHandler(buttons);             //Buttons argument = display.getButtons() from Main program -> checkButtons() function
    }
  }
  /*
  //Else if current page = editor page (not needed as we wont be doing any editing)
  else if (currentDisplayState == displayStateEditor) {
    if (editorHandler) {
      editorHandler(buttons, 0, 0, NULL);
    }
  }
  */
}


// Function to keep track of Menu like a linked list index
void newMenu(int8_t newIndex) {
  currentMenuLine = 0;
  lastMenuLine = -1;
  currentSelectionLine = 0;
  lastSelectionLine = -1;
  if (newIndex >= 0) {
    menuHistory[menuHistoryIndex++] = currentMenu;
    currentMenu = newIndex;
  } else {
    if (currentDisplayState == displayStateMenu) {    //If it is already in a menu, take -1 menuHistoryIndex before and use as currentMenu
      menuHistoryIndex--;
      currentMenu = menuHistory[menuHistoryIndex];
    }
  }

  //MenuHistoryIndex anything other than 0, it means that it is in a menu
  if (menuHistoryIndex) {
    currentDisplayState = displayStateMenu;
    if (menu_debug_print)SerialMonitorInterface.print("New menu index ");
    if (menu_debug_print)SerialMonitorInterface.println(currentMenu);
    currentSelectionLine = menuSelectionLineHistory[menuHistoryIndex];
  } else {
    if (menu_debug_print)SerialMonitorInterface.print("New menu index ");
    if (menu_debug_print)SerialMonitorInterface.println("home");
    menuSelectionLineHistory[menuHistoryIndex + 1] = 0;       //Erase the previous history index away
    currentDisplayState = displayStateHome;
    initHomeScreen();
  }
}

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


// Rendering the Menu 
int changeDir;
int changeEnd;
int changeStart;
int yChange;

void drawMenu() {
  //for (int yChange = changeStart; yChange != changeEnd; yChange += changeDir) {
  if (needMenuDraw) {
    needMenuDraw = false;
    displayBuffer.clearWindow(0, 7, 96, 56);
    for (int i = 0; i < menuList[currentMenu].amtLines; i++) {
      // Highlight currently selected option
      if (i == currentSelectionLine) {
        displayBuffer.fontColor(defaultFontColor, ALPHA_COLOR);
      } else {
        displayBuffer.fontColor(inactiveFontColor, ALPHA_COLOR);
      }
      
      //write the selection string to display
      char buffer[20];
      strcpy_P(buffer, (PGM_P)pgm_read_word(&(menuList[currentMenu].strings[currentMenuLine + i])));
      int width = displayBuffer.getPrintWidth(buffer);
      displayBuffer.setCursor(96 / 2 - width / 2, menuTextY[i] + 5 + yChange - (currentSelectionLine * 8) + 16);
      displayBuffer.print(buffer);
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
  displayBuffer.fontColor(0xFFFF, ALPHA_COLOR);
}

//Display Menu
void viewMenu(uint8_t button) {
  if (menu_debug_print)SerialMonitorInterface.print("viewMenu ");
  if (menu_debug_print)SerialMonitorInterface.println(button);
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
    if (menu_debug_print)SerialMonitorInterface.println("drawing menu ");
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

  displayBuffer.fontColor(0xFFFF, ALPHA_COLOR);
}

//Functions for buttons functionality of menu using the help of arrows
void writeArrows() {
  leftArrow(0, 15 + 2);
  rightArrow(0, 45 + 5);

  upArrow(90, 15 + 2);
  downArrow(90, 45 + 4);
}

//Function for drawing the arrows based on X and Y axis of screen
void leftArrow(int x, int y) {
  displayBuffer.drawLine(x + 2, y - 2, x + 2, y + 2, 0xFFFF);
  displayBuffer.drawLine(x + 1, y - 1, x + 1, y + 1, 0xFFFF);
  displayBuffer.drawLine(x + 0, y - 0, x + 0, y + 0, 0xFFFF);
}

void rightArrow(int x, int y) {
  displayBuffer.drawLine(x + 0, y - 2, x + 0, y + 2, 0xFFFF);
  displayBuffer.drawLine(x + 1, y - 1, x + 1, y + 1, 0xFFFF);
  displayBuffer.drawLine(x + 2, y - 0, x + 2, y + 0, 0xFFFF);
}

void upArrow(int x, int y) {
  displayBuffer.drawLine(x + 0, y - 0, x + 4, y - 0, 0xFFFF);
  displayBuffer.drawLine(x + 1, y - 1, x + 3, y - 1, 0xFFFF);
  displayBuffer.drawLine(x + 2, y - 2, x + 2, y - 2, 0xFFFF);
}
void downArrow(int x, int y) {
  displayBuffer.drawLine(x + 0, y + 0, x + 4, y + 0, 0xFFFF);
  displayBuffer.drawLine(x + 1, y + 1, x + 3, y + 1, 0xFFFF);
  displayBuffer.drawLine(x + 2, y + 2, x + 2, y + 2, 0xFFFF);
}
