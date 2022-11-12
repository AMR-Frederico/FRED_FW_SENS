#include <MAIN/config.h>
#include <MAIN/ultrasonic.h>
#include "filter.h"
//#include <MAIN/laser.h>7

int ultrasonic_values[NUMBER_ULTRASONIC_SENSORS];  

int previousTime; 

void setup(){

  pinMode(LED_BUILD_IN,OUTPUT);
  digitalWrite(LED_BUILD_IN,HIGH);

  Serial.begin(9600);

  previousTime = millis(); 
}


void loop(){
  //int* ultrasonic_values = ultrasonic_measurments(previousTime); 
  ultrasonic_measurments(previousTime); 


}