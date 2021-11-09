#define	BLACK           0x00
//#define	BLUE            0xE0

//Extra colors if you want them.
//#define	RED             0x03
//#define	GREEN           0x1C
//#define	DGREEN          0x0C
//#define YELLOW          0x1F
#define WHITE           0xFF
//#define ALPHA           0xFE
//#define	BROWN           0x32

#include <TinyScreen.h>
#include <SPI.h>
#include <Wire.h>
#include "BMA250.h" //Accelerometer support file for Bosch MA 250 accelerometer
BMA250 accel;

// Make Serial Monitor compatible for all TinyCircuits processors
#if defined(ARDUINO_ARCH_AVR)
  #define SerialMonitorInterface Serial
#elif defined(ARDUINO_ARCH_SAMD)
  #define SerialMonitorInterface SerialUSB
#endif

//Library must be passed the board type
//TinyScreenDefault for TinyScreen shields
//TinyScreenAlternate for alternate address TinyScreen shields
//TinyScreenPlus for TinyScreen+
TinyScreen display = TinyScreen(TinyScreenDefault);
long int rando = 0;  //Setting up the random number holder

void setup(void) {
  Wire.begin();
  display.begin();
  randomSeed(analogRead(A0)); //To make our random numbers less predictable, we use randomSeed
                              //and read an unused pin for a starting value (seed)
  SerialMonitorInterface.begin(9600);
  accel.begin(BMA250_range_2g, BMA250_update_time_64ms); 
}

void loop() {
  rando = random(1, 20); //Changed to 3 for testing You need a random range 1 more than the number of statements you have
  rando = round(rando); //Round it to a whole number
  display.setFont(liberationSans_10ptFontInfo); //font size is 8
  display.fontColor(WHITE,BLACK);

  	accel.read(); //Read and manipulate accelerometer data
  	int z = accel.Z;
  	SerialMonitorInterface.println(z); //printing out Z values for debugging purposes
   
  	if (z < 0) {display.clearWindow(0,0,96,64); delay(1000);}
  	if (z > 220) {
  		switch(rando) { //A switch case for each statement
	  		case 1: 
	  			accel.read();
	  			while (accel.Z > 160){
	  				display.setCursor(3,5); //set the cursor to the top left corner
	  				display.print("It is certain"); //Here's the message that gets printed
	  				SerialMonitorInterface.println(rando);
	  				delay(2000);
	  				accel.read();}
	  			display.clearWindow(0,0,96,64); //It's important to clear the window after each graphic
	  		case 2:
	  			accel.read();
	  			while (accel.Z > 160){
	  				display.setCursor(3,5);
	  				display.print("Your dream is");
	  				display.setCursor(3,20); //With longer messages, split them up; 2d line begins at y=10
	  				display.print("achievable"); //2nd line of message
	  				SerialMonitorInterface.println(rando);
	  				delay(2000);
	  				accel.read();}
	  			display.clearWindow(0,0,96,64);
        case 3:
         accel.read();
          while (accel.Z > 160){
            display.setCursor(3,5);
            display.print("Better not tell ");
            display.setCursor(3,20); //With longer messages, split them up; 2d line begins at y=10
            display.print("you now"); //2nd line of message
            SerialMonitorInterface.println(rando);
            delay(2000);
            accel.read();}
          display.clearWindow(0,0,96,64);
        case 4: 
          accel.read();
          while (accel.Z > 160){
            display.setCursor(3,5);
            display.print("Concentrate and ");
            display.setCursor(3,20); //With longer messages, split them up; 2d line begins at y=10
            display.print("ask again"); //2nd line of message
            SerialMonitorInterface.println(rando);
            delay(2000);
            accel.read();}
          display.clearWindow(0,0,96,64);
        case 5: 
          accel.read();
          while (accel.Z > 160){
            display.setCursor(3,5);
            display.print("You may rely");
            display.setCursor(3,20);
            display.print("on it");
            SerialMonitorInterface.println(rando);
            delay(2000);
            accel.read();}
          display.clearWindow(0,0,96,64);
        case 6: 
          accel.read();
          while (accel.Z > 160){
            display.setCursor(3,5);
            display.print("As I see it yes");
            SerialMonitorInterface.println(rando);
            delay(2000);
            accel.read();}
          display.clearWindow(0,0,96,64);
        case 7: 
          accel.read();
          while (accel.Z > 160){
            display.setCursor(3,5);
            display.print("Most likely");
            SerialMonitorInterface.println(rando);
            delay(2000);
            accel.read();}
          display.clearWindow(0,0,96,64);
        case 8: 
          accel.read();
          while (accel.Z > 160){
            display.setCursor(3,5);
            display.print("Your answer is...");
            display.setCursor(3,20); //With longer messages, split them up; 2d line begins at y=10
            display.print("on your left"); //2nd line of message
            SerialMonitorInterface.println(rando);
            delay(2000);
            accel.read();}
          display.clearWindow(0,0,96,64);
        case 9: 
          accel.read();
          while (accel.Z > 160){
            display.setCursor(3,5);
            display.print("Yes!");
            SerialMonitorInterface.println(rando);
            delay(2000);
            accel.read();}
          display.clearWindow(0,0,96,64);
		 case 10: 
         accel.read();
          while (accel.Z > 160){
            display.setCursor(3,5);
            display.print("Signs point to yes");
            SerialMonitorInterface.println(rando);
            delay(2000);
            accel.read();}
          display.clearWindow(0,0,96,64);
        case 11: 
          accel.read();
          while (accel.Z > 160){
            display.setCursor(3,5);
            display.print("Don't be sad");
            display.setCursor(3,20);
            display.print("Smile:)");
            SerialMonitorInterface.println(rando);
            delay(2000);
            accel.read();}
          display.clearWindow(0,0,96,64);
        case 12: 
          accel.read();
          while (accel.Z > 160){
            display.setCursor(3,5);
            display.print("Get sushi");
            SerialMonitorInterface.println(rando);
            delay(2000);
            accel.read();}
          display.clearWindow(0,0,96,64);
        case 13: 
          accel.read();
          while (accel.Z > 160){
            display.setCursor(3,5);
            display.print("Better not");
            display.setCursor(3,20);
            display.print("tell you now");
            SerialMonitorInterface.println(rando);
            delay(2000);
            accel.read();}
          display.clearWindow(0,0,96,64);
        case 14: 
          accel.read();
          while (accel.Z > 160){
            display.setCursor(3,5);
            display.print("Cannot predict");
            display.setCursor(3,20);
            display.print("now");
            SerialMonitorInterface.println(rando);
            delay(2000);
            accel.read();}
          display.clearWindow(0,0,96,64);
        case 15: 
          accel.read();
          while (accel.Z > 160){
            display.setCursor(3,5);
            display.print("Very doubtful");
            display.setCursor(3,20);
            display.print("and ask again");
            SerialMonitorInterface.println(rando);
            delay(2000);
            accel.read();}
          display.clearWindow(0,0,96,64);
        case 16: 
          accel.read();
          while (accel.Z > 160){
            display.setCursor(3,5);
            display.print("Don't count on it");
            SerialMonitorInterface.println(rando);
            delay(2000);
            accel.read();}
          display.clearWindow(0,0,96,64);
        case 17: 
          accel.read();
          while (accel.Z > 160){
            display.setCursor(3,5);
            display.print("My reply is ");
            display.setCursor(3,20);
            display.print("go for it");
            SerialMonitorInterface.println(rando);
            delay(2000);
            accel.read();}
          display.clearWindow(0,0,96,64);
        case 18: 
          accel.read();
          while (accel.Z > 160){
            display.setCursor(3,5);
            display.print("My sources");
            display.setCursor(3,20);
            display.print("say no");
            SerialMonitorInterface.println(rando);
            delay(2000);
            accel.read();}
          display.clearWindow(0,0,96,64);
        case 19: 
          accel.read();
          while (accel.Z >160){
            display.setCursor(3,5);
            display.print("Don't give up");
            display.setCursor(3,20);
            display.print("take a break");
            SerialMonitorInterface.println(rando);
            delay(2000);
            accel.read();}
          display.clearWindow(0,0,96,64);
	  		break;
  		}
  	display.clearWindow(0,0,96,64); //Clear window after everything
  	}
  }
