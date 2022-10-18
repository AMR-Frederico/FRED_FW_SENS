#include <MAIN/config.h>
#include <MAIN/ultrasonic.h>

float ultrasonicLeft_measure; 
float ultrasonicMiddle_measure; 
float ultrasonicRight_measure; 

void setup(){

  pinMode(LED_BUILD_IN,OUTPUT);
  digitalWrite(LED_BUILD_IN,HIGH);

  Serial.begin(9600);
}

void loop(){
  ultrasonicLeft_measure, ultrasonicMiddle_measure, ultrasonicRight_measure = ultrasound_measurments(); 

  /*
  // conversion to meters
  ultrasonicLeft_measure = ultrasonicLeft_measure/10; 
  ultrasonicMiddle_measure = ultrasonicMiddle_measure/10; 
  ultrasonicRight_measure = ultrasonicRight_measure/10; 
  */

  Serial.println(ultrasonicLeft_measure); 
}