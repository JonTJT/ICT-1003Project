sprite heart_list[7] = {heart_1,heart_2,heart_3,heart_4,heart_5,heart_6};

void LoveCalculator() {
  display.clearScreen();
  while(1) {
     if (display.getButtons() != TSButtonLowerRight) {
        buttonLoop();
     } else {
        display.clearScreen();
        break;
     }
   }

   //Go Back to Menu
}

void buttonLoop() {
  unsigned long currentTime;
  unsigned long buttonsstartTime = 0;
  int buttonspressed = 0;
  int percentage;
  int heart_no;
  currentTime = millis();

  // Clear the screen if the buttons are not pressed.
  
  if (display.getButtons(TSButtonUpperLeft) && display.getButtons(TSButtonUpperRight)) {
    display.clearScreen();
  }

  while (display.getButtons(TSButtonUpperLeft) && display.getButtons(TSButtonUpperRight)) {
    // Set the button pressed boolean to 1, to allow the program to know when to clear screen
    buttonspressed = 1;
    buttonsstartTime = millis();
    percentage = floor((buttonsstartTime - currentTime) / 1000) * 20;
    heart_no = floor((buttonsstartTime - currentTime)/1000);
    delay(1000);
    display.setCursor( 48 - (display.getPrintWidth("Calculating:    %") / 2), 52);
    //display.setCursor( 48 - (display.getPrintWidth("Calculating:    %") / 2), 32 - (display.getFontHeight() / 2));
    if ( !heart_no == 6){
      drawBuffer(heart_list[heart_no],30,20);
    } else{
      drawBuffer(heart_list[heart_no],28,18);
    }
    display.print("Calculating: " + String(percentage) + "%");
    if ( (buttonsstartTime - currentTime) > 5000 ) {
      delay(500);
      calculatelove();
      break;
    }
  }
  if (buttonspressed == 1) {
    display.clearScreen();
    buttonspressed = 0;
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
