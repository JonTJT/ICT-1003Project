  //-------------------------------------------------------------------------------
//  TinyCircuits TinyScreen/TinyScreen+ Basic Example
//  Last Updated 26 January 2016
//  
//  This example shows the basic functionality of the TinyScreen library,
//  including drawing, writing bitmaps, and printing text
//
//  Written by Ben Rose for TinyCircuits, https://tiny-circuits.com
//
//-------------------------------------------------------------------------------
//
//#include <Wire.h>
//#include <SPI.h>
//#include <TinyScreen.h>

//Library must be passed the board type
//TinyScreenDefault for TinyScreen shields
//TinyScreenAlternate for alternate address TinyScreen shields
//TinyScreenPlus for TinyScreen+
//TinyScreen display = TinyScreen(TinyScreenDefault);

//const uint8_t upButton     = TSButtonUpperRight;      (in main.ino)
//const uint8_t downButton   = TSButtonLowerRight;
const uint8_t nextButton   = TSButtonLowerLeft;

const uint8_t displayStateHoroscope = 0x01;

uint8_t currentDisplayState;
int month1 = 0;
int month2 = 0;
int day1 = 0;
int day2 = 0;
int positionOfDOB = 0;
String astro_sign = " ";
bool reset = false;
const int day1Array[4] = { 0, 1, 2, 3 };
const int day2month2Array[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
const int month1Array[2] = { 0, 1 };

void horoscope() {
  display.clearScreen();
  dobPage();
  while(1) {
     if (display.getButtons() != TSButtonLowerRight) {
        horoscopeLoop();
     } else {
        display.clearScreen();
        //Set back the fonts
        display.setFont(thinPixel7_10ptFontInfo);
        display.fontColor(TS_16b_Green,TS_16b_Black);
        break;
     }
   }
}


//void setup(void) {
//  Wire.begin();//initialize I2C before we can initialize TinyScreen- not needed for TinyScreen+
//  display.begin();
//  //setBrightness(brightness);//sets main current level, valid levels are 0-15
//  display.setBrightness(10);
//  display.setFont(thinPixel7_10ptFontInfo);
//  display.setFlip(0);
//  dobPage();
//}

void horoscopeLoop() {
  if (display.getButtons(TSButtonLowerLeft)) {
    if (positionOfDOB == 0) {
      positionOfDOB +=1;
      display.fontColor(TS_8b_Red,TS_8b_Black);
      display.setCursor(30, 30);
      display.print(month1);
      delay(500);
    } else if (positionOfDOB == 1) {
      delay(500);
      positionOfDOB +=1;
      dobPage();
      display.fontColor(TS_8b_Red,TS_8b_Black);
      display.setCursor(40, 30);
      display.print(month2);
    } else if (positionOfDOB == 2) {
      delay(500);
      positionOfDOB +=1;
      dobPage();
      display.fontColor(TS_8b_Red,TS_8b_Black);
      display.setCursor(50, 30);
      display.print(day1);
    } else if (positionOfDOB == 3) {
      delay(500);
      positionOfDOB +=1;
      dobPage();
      display.fontColor(TS_8b_Red,TS_8b_Black);
      display.setCursor(60, 30);
      display.print(day2);
    } else if (positionOfDOB == 4) {
      // upon clicking submit
      // get the month and day to compute the horoscope
      int monthDOB = concatenate(month1, month2);
      int dayDOB = concatenate(day1, day2);
      getDOB(monthDOB, dayDOB);
      if (astro_sign != " ") {
        // go to next page to print astro sign and message
        display.clearScreen();
        delay(500);
        display.fontColor(TS_8b_White,TS_8b_Black);
        display.setCursor( 48 - (display.getPrintWidth("Your Horoscope is: ")/2), 5);
        display.print("Your Horoscope is: ");
        display.setCursor(30, 15);
        display.print(astro_sign);
        // get horoscope message
        getHoroscopeMessage(astro_sign);
        reset = true;
        delay(5000);
        dobPage();
      }
     }
    }
  

  // increment
  if (display.getButtons(TSButtonUpperRight)) {
      delay(500);
      if (positionOfDOB == 1){
        int sizeOfArrayMonth1 = sizeof(month1Array)/sizeof(month1Array[0]) - 1;
        if (month1 == sizeOfArrayMonth1) {
          month1 = 0;
        } else {
          month1 += 1;  
        }
      }
      else if (positionOfDOB == 2){
        int sizeOfArrayMonth2 = sizeof(day2month2Array)/sizeof(day2month2Array[0]) - 1;
        if (month2 == sizeOfArrayMonth2) {
          month2 = 0;
        } else {
          month2 += 1;  
        }
      }
      else if (positionOfDOB == 3){
        int sizeOfArrayMonth1 = sizeof(day1Array)/sizeof(day1Array[0]) - 1;
        if (day1 == sizeOfArrayMonth1) {
          day1 = 0;
        } else {
          day1 += 1;  
        }
      }
      else if (positionOfDOB == 4){
        int sizeOfArrayMonth2 = sizeof(day2month2Array)/sizeof(day2month2Array[0]) - 1;
        if (day2 == sizeOfArrayMonth2) {
          day2 = 0;
        } else {
          day2 += 1;  
        }
      }
      dobPage();
  }
  // decrement
  if (display.getButtons(TSButtonLowerRight)) {
      delay(500);
      if (positionOfDOB == 1){
        int sizeOfArrayMonth1 = sizeof(month1Array)/sizeof(month1Array[0]) - 1;
        if (month1 == 0) {
          month1 = sizeOfArrayMonth1;
        } else {
          month1 -= 1;  
        }
      }
      else if (positionOfDOB == 2){
        int sizeOfArrayMonth2 = sizeof(day2month2Array)/sizeof(day2month2Array[0]) - 1;
        if (month2 == 0) {
          month2 = sizeOfArrayMonth2;
        } else {
          month2 -= 1;  
        }
      }
      else if (positionOfDOB == 3){
        int sizeOfArrayDay1 = sizeof(day1Array)/sizeof(day1Array[0]) - 1;
        if (day1 == 0) {
          day1 = sizeOfArrayDay1;
        } else {
          day1 -= 1;  
        }
      }
      else if (positionOfDOB == 4){
        int sizeOfArrayDay2 = sizeof(day2month2Array)/sizeof(day2month2Array[0]) - 1;
        if (day2 == 0) {
          day2 = sizeOfArrayDay2;
        } else {
          day2 -= 1;  
        }
      }
      dobPage();
  }
}

unsigned concatenate(unsigned x, unsigned y) {
  unsigned pow = 10;
  while(y >= pow)
      pow *= 10;
  return x * pow + y;        
}

void dobPage() {
  if (reset == true) {
    month1 = 0;
    month2 = 0;
    day1 = 0;
    day2 = 0;
    positionOfDOB = 0;
    astro_sign = " ";
    reset = false;
  }
  currentDisplayState = displayStateHoroscope;
  display.clearScreen();
  display.fontColor(TS_8b_White,TS_8b_Black);
  display.setCursor( 48 - (display.getPrintWidth("Enter DOB:")/2), 5);
  display.print("Enter DOB:");
  display.setCursor( 48 - (display.getPrintWidth("(MM/DD)")/2), 15);
  display.print("(MM/DD)");
  display.setCursor(30, 30);
  display.print(month1);
  display.setCursor(40, 30);
  display.print(month2);
  display.setCursor(50, 30);
  display.print(day1);
  display.setCursor(60, 30);
  display.print(day2);
  if (positionOfDOB == 4) {
    display.setCursor(0, 55);
    display.print("<Submit");
  } else {
    display.setCursor(0, 55);
    display.print("<Next");
  }

  display.setCursor(88, 0);
  display.print("+");
  display.setCursor(88, 55);
  display.print("-");
  display.fontColor(TS_8b_Red,TS_8b_Black);  
  if (positionOfDOB == 1) {
    display.setCursor(30, 30);
    display.print(month1Array[month1]);
  } else if (positionOfDOB == 2) {
    display.setCursor(40, 30);
    display.print(day2month2Array[month2]);
  } else if (positionOfDOB == 3) {
    display.setCursor(50, 30);
    display.print(day1Array[day1]);
  } else if (positionOfDOB == 4) {
    display.setCursor(60, 30);
    display.print(day2month2Array[day2]);
  }
  delay(100);
}

void getDOB(int monthDOB, int dayDOB) {
  // December
  if (monthDOB == 12){
    if (dayDOB < 22)
      astro_sign = "Sagittarius";
    else if (dayDOB >= 22 && dayDOB <= 31)
      astro_sign ="Capricorn";
  }
  // January
  else if (monthDOB == 1){
    if (dayDOB < 20)
      astro_sign = "Capricorn";
    else if (dayDOB >= 20 && dayDOB <= 31)
      astro_sign = "Aquarius";
  }
  // February 
  else if (monthDOB == 2){
    if (dayDOB < 19)
      astro_sign = "Aquarius";
    else if (dayDOB >= 19 && dayDOB <= 29)
      astro_sign = "Pisces";
  }
  // March    
  else if(monthDOB == 3){
    if (dayDOB < 21)
      astro_sign = "Pisces";
    else if (dayDOB >= 21 && dayDOB <= 31)
      astro_sign = "Aries";
  }
  // April
  else if (monthDOB == 4){
    if (dayDOB < 20)
      astro_sign = "Aries";
    else if (dayDOB >= 20 && dayDOB <= 30)
      astro_sign = "Taurus";
  }
  // May     
  else if (monthDOB == 5){
    if (dayDOB < 21)
      astro_sign = "Taurus";
    else if (dayDOB >= 21 && dayDOB <= 31)
      astro_sign = "Gemini";
  }
  // June    
  else if(monthDOB == 6){
    if (dayDOB < 21)
      astro_sign = "Gemini";
    else if (dayDOB >= 21 && dayDOB <= 30)
      astro_sign = "Cancer";
  }
  // July     
  else if (monthDOB == 7){
    if (dayDOB < 23)
      astro_sign = "Cancer";
    else if (dayDOB >= 23 && dayDOB <= 31)
      astro_sign = "Leo";
  }
  // August     
  else if(monthDOB == 8){
    if (dayDOB < 23)
      astro_sign = "Leo";
    else if (dayDOB >= 23 && dayDOB <= 31)
      astro_sign = "Virgo";
  }
  // September     
  else if (monthDOB == 9){
    if (dayDOB < 23)
      astro_sign = "Virgo";
    else if (dayDOB >= 23 && dayDOB <= 30)
      astro_sign = "Libra";
  }
  // October  
  else if (monthDOB == 10){
    if (dayDOB < 23)
      astro_sign = "Libra";
    else if (dayDOB >= 23 && dayDOB <= 31)
      astro_sign = "Scorpio";
  }
  // November     
  else if (monthDOB == 11){
    if (dayDOB < 22)
      astro_sign = "Scorpio";
    else if (dayDOB >= 22 && dayDOB <= 30)
      astro_sign = "Sagittarius";
  } else {
    display.clearScreen();
    display.fontColor(TS_8b_White,TS_8b_Black);
    display.setCursor( 48 - (display.getPrintWidth("Invalid DOB")/2), 5);
    display.print("Invalid DOB");
    delay(5000);
    reset = true;
    dobPage();
  }
}

void getHoroscopeMessage(String horoscope) {
  if (horoscope == "Aries") {
      printHoroscopeMsg("Life is like smooth", "jazz today", "");
  } else if (horoscope == "Taurus") {
      printHoroscopeMsg("Learn to trust the", "process~", "");
  } else if (horoscope == "Gemini") {
      printHoroscopeMsg("The star has given", "you an extra boost", "");
  } else if (horoscope == "Cancer") {
      printHoroscopeMsg("Neptune has pave", "a way for meaning", "-ful connections");
  } else if (horoscope == "Leo") {
      printHoroscopeMsg("Deepen your", "connections with", "key people");
  } else if (horoscope == "Virgo") {
      printHoroscopeMsg("Communication is", "key to maintaining", "trust");
  } else if (horoscope == "Libra") {
      printHoroscopeMsg("Celebrate wins", "with some bonding", "sessions");
  } else if (horoscope == "Scropio") {
      printHoroscopeMsg("Your charisma level", "are off the charts!", "");
  } else if (horoscope == "Sagittarius") {
      printHoroscopeMsg("People will be", "genuinely impressed", "by your aura");
  } else if (horoscope == "Capricorn") {
      printHoroscopeMsg("Go ahead and treat", "yourself better", "");
  }  else if (horoscope == "Aquarius") {
      printHoroscopeMsg("You will attract", "the right people", "by being yourself");
  }  else if (horoscope == "Pisces") {
      printHoroscopeMsg("Be more tender and", "loving towards your", "love interest");
  } 
}

void printHoroscopeMsg(String msg1, String msg2, String msg3) {
  display.setCursor(0, 32);
  display.print(msg1);
  display.setCursor(0, 42);
  display.print(msg2);
  display.setCursor(0, 52);
  display.print(msg3);
}
