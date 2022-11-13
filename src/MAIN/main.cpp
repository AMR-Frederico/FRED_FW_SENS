#include <MAIN/config.h>
#include <MAIN/ultrasonic.h>
#include <MAIN/ros_com.h>

#include <ros.h>
#include <std_msgs/Int32.h>

#include "filter.h"
//#include <MAIN/laser.h>

bool _connect = false ;

ros::NodeHandle  nh;

std_msgs::Int32 leftUltrasonic_distance; 
std_msgs::Int32 middleUltrasonic_distance; 
std_msgs::Int32 rightUltrasonic_distance; 

ros::Publisher pubLeftUltrasonic("sensor/ultrasonic/left/distance", &leftUltrasonic_distance); 
ros::Publisher pubMiddleUltrasonic("sensor/ultrasonic/middle/distance", &middleUltrasonic_distance); 
ros::Publisher pubRightUltrasonic("sensor/ultrasonic/right/distance", &rightUltrasonic_distance); 


int ultrasonic_range[NUMBER_ULTRASONIC_SENSORS];  
int previousTime; 

void setup(){

  nh.initNode(); 

  nh.advertise(pubLeftUltrasonic);
  nh.advertise(pubMiddleUltrasonic);
  nh.advertise(pubRightUltrasonic); 

  // pinMode(LED_BUILD_IN,OUTPUT);
  // digitalWrite(LED_BUILD_IN,HIGH);

  //Serial.begin(9600);

  previousTime = millis();

  nh.getHardware()->setBaud(57600);  
  
}


void loop(){

  int* ultrasonic_range = ultrasonic_measurments(previousTime); 
  
  leftUltrasonic_distance.data = ultrasonic_range[0]; 
  middleUltrasonic_distance.data = ultrasonic_range[1];
  rightUltrasonic_distance.data = ultrasonic_range[2];

  pubLeftUltrasonic.publish(&leftUltrasonic_distance);
  pubMiddleUltrasonic.publish(&middleUltrasonic_distance); 
  pubRightUltrasonic.publish(&rightUltrasonic_distance);

  nh.spinOnce();

  
}   