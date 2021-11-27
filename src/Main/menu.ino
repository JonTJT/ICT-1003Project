void initHomeScreen() {
  //Show wizard
  display.clearScreen();
  drawBuffer(wizard,wizard_x,wizard_y);
  delay(1000);
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

void checkButtons(int timeMS) {
  for(int i=0; i<timeMS; i++) {
    byte buttons = display.getButtons();
    if (buttons == TSButtonUpperRight || buttons == TSButtonLowerRight || buttons == TSButtonUpperLeft || buttons == TSButtonLowerLeft) {
      drawMenu();
    }
    delay(1);
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

    delay(200);
    if (display.getButtons(downButton) && currentSelection < 3) {
      currentSelection++;
      buttonsPressed = 1;
      display.clearScreen();
    }
    else if (display.getButtons(upButton) && currentSelection > 0) {
      currentSelection--;
      buttonsPressed = 1;
      display.clearScreen();
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
