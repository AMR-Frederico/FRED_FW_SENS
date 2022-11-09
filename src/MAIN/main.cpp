#include <MAIN/config.h>
//#include <MAIN/ultrasonic.h>
#include "filter.h"
#include <MAIN/laser.h>

float ultrasonicLeft_measure; 
float ultrasonicMiddle_measure; 
float ultrasonicRight_measure; 

void setup(){

  pinMode(LED_BUILD_IN,OUTPUT);
  digitalWrite(LED_BUILD_IN,HIGH);

  Serial.begin(9600);

  //ultrasonicLeftArray[0] = ultrasonicLeft.read(CM); 
  //ultrasonicMiddleArray[0] = ultrasonicMiddle.read(CM); 
  //ultrasonicRightArray[0] = ultrasonicRight.read(CM);

  laser_init(); 
}

void loop(){

  //ultrasonicLeft_measure, ultrasonicMiddle_measure, ultrasonicRight_measure = ultrasonic_measurments(); 

  //LaserLeft_measure, LaserRight_measure = laser_measurments();

  /*
  // conversion to meters
  ultrasonicLeft_measure = ultrasonicLeft_measure/10; 
  ultrasonicMiddle_measure = ultrasonicMiddle_measure/10; 
  ultrasonicRight_measure = ultrasonicRight_measure/10; 
  */

  //Serial.println(ultrasonicLeft_measure); 

  laser_measurments();
}