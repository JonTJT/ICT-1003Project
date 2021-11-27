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
  punished = 1; //He will say something nice next  
  display.setFlip(gravity);    
  display.clearScreen();
  
  drawBuffer(wizard, wizard_x, wizard_y);
  delay(1000);
  display.setCursor(0,10);
  display.print("Ow! Hey!?");
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
//    max_walk = (wizard_x - 5) / 5;
//    rand_walk = random(max_walk);
//    bound = wizard_x - (rand_walk * 5);
    max_walk = wizard_x - 5;
    rand_walk = random(max_walk);
    bound = wizard_x - rand_walk;
    if (bound >= wizard_x) {
//      delay(1000);
      checkButtons(1000);
      
//      display.setCursor(20,10);
//      display.print("*zzzzzzzz*");
////      delay(2000);
//      checkButtons(2000);
//      display.clearWindow(0,0,96,30);
      return;
    }
 
    for (wizard_x; wizard_x > bound; wizard_x -= 1) {
      drawBuffer(wizard, wizard_x, wizard_y);
      
      checkButtons(1);
      
      gravity_check();
      delay(walking_delay);
    }
  } else {                                                     //Walk Right
//    max_walk = (75 - wizard_x) / 5;
//    rand_walk = random(max_walk);
//    bound = wizard_x + rand_walk * 5;
    max_walk = 75 - wizard_x;
    rand_walk = random(max_walk);
    bound = wizard_x + rand_walk;
    if (bound <= wizard_x) {
      checkButtons(1000);
      return;
    }
    for (wizard_x; wizard_x < bound; wizard_x += 1) {
      drawBuffer(wizard_flipped, wizard_x, wizard_y);
      checkButtons(1);
      
      gravity_check();
      delay(walking_delay);
      
    }
  }
  //Make him talk
    talk();
}


// Fn to make him say random things
void talk() {
  int rand_word = 0;
  // Checks if he was recently flipped(aka punished) and chooses rng based on that
  if (punished) {
    rand_word = random(8, 13);
    punished = 0;
  }
  else {
    rand_word = random(13 * 2); //There are 13 options, the *2 is to make it 50% chance that he won't say anything
  }
  //There are currently 13 lines possible
  switch(rand_word) {
    case 0:
      display.setCursor(0,10);
      display.print("I predict that...");
//      delay(1000);
      checkButtons(1000);
      display.setCursor(0,20);
      display.print("you gained weight");
//      delay(3000);
      checkButtons(3000);
      display.clearWindow(0,0,96,30);
      break;
    case 1:
      display.setCursor(0,10);
      display.print("I'm not happy...");
//      delay(1000);
      checkButtons(1000);
      display.setCursor(0,20);
      display.print("but I wanna be");
//      delay(3000);
      checkButtons(3000);
      display.clearWindow(0,0,96,30);
      break;
    case 2:
      display.setCursor(0,10);
      display.print("The stars tell me...");
//      delay(1000);
      checkButtons(1000);
      display.setCursor(0,20);
      display.print("its a good day");
//      delay(3000);
      checkButtons(3000);
      display.clearWindow(0,0,96,30);
      break;
    case 3:
      display.setCursor(0,10);
      display.print("My magic ball says..");
//      delay(1000);
      checkButtons(1000);
      display.setCursor(0,20);
      display.print("low battery? Wut");
//      delay(3000);
      checkButtons(3000);
      display.clearWindow(0,0,96,30);
      break;
    case 4:
      display.setCursor(0,10);
      display.print("Your ICT1003...");
//      delay(1000);
      checkButtons(1000);
      display.setCursor(0,20);
      display.print("GETS A+ BABY!!!");
//      delay(3000);
      checkButtons(3000);
      display.clearWindow(0,0,96,30);
      break;
    case 5:
      display.setCursor(0,10);
      display.print("Have a 'STUN SEED'");
//      delay(1000);
      checkButtons(1000);
      display.setCursor(0,20);
      display.print("backwards");
      display.setCursor(0,30);
//      delay(3000);
      checkButtons(3000);
      display.print("gottem");
//      delay(2000);
      checkButtons(2000);
      display.clearWindow(0,0,96,40);
      break;
    case 6:
      display.setCursor(0,10);
      display.print("Your face says...");
      checkButtons(1000);
      display.setCursor(0,20);
      display.print("bad fertility!");
      checkButtons(3000);
      display.clearWindow(0,0,96,30);
      break;
    case 7:
      display.setCursor(0,10);
      display.print("Luck is...");
      checkButtons(1000);
      display.setCursor(0,20);
      display.print("believing you are");
      display.setCursor(0,30);
      display.print("lucky!");
      checkButtons(2000);
      display.clearWindow(0,0,96,40);
      break;
    case 8:
      display.setCursor(0,10);
      display.print("U from tennessee?");
      checkButtons(1000);
      display.setCursor(0,20);
      display.print("Cos u the only 10");
      display.setCursor(0,30);
      display.print("i see");
      checkButtons(3000); 
      display.clearWindow(0,0,96,40);
      break;
    case 9:
      display.setCursor(0,10);
      display.print("Your palm says...");
      checkButtons(1000);
      display.setCursor(0,20);
      display.print("1 week good luck!");
      checkButtons(3000);
      display.clearWindow(0,0,96,30);
      break;
    case 10:
      display.setCursor(0,10);
      display.print("Your palm says...");
      checkButtons(1000);
      display.setCursor(0,20);
      display.print("3 weeks good luck!");
      checkButtons(3000);
      display.clearWindow(0,0,96,30);
      break;
    case 11:
      display.setCursor(0,10);
      display.print("Your palm says...");
      checkButtons(1000);
      display.setCursor(0,20);
      display.print("long life ahead!");
      checkButtons(3000);
      display.clearWindow(0,0,96,30);
      break;
    case 12:
      display.setCursor(0,10);
      display.print("Your face says...");
      checkButtons(1000);
      display.setCursor(0,20);
      display.print("good fertility!");
      checkButtons(3000);
      display.clearWindow(0,0,96,30);
      break;
    default: //He will go on to the next loop without saying anything
      break;
  }
}
