#include <MAIN/config.h>
#include <MAIN/ultrasonic.h>
#include "led_strip.h"
#include <MAIN/ros_com.h>
#include <MAIN/imu.h>

#include <ros.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float32.h>
#include <sensor_msgs/Imu.h>

#include "filter.h"
//#include <MAIN/laser.h>

#define cmd_led_strip_topic "cmd/led_strip/color"

bool _connect = false ;
bool imu_status, dmp_status; 

float quaternions[4];           //x, y, z, w
float linaer_acceleration[3];   //x, y, z
float angular_velocity[3];      //x, y, z 

int ultrasonic_range[NUMBER_ULTRASONIC_SENSORS];  
int previousTime; 

//* <------------ ROS setup ---------->
ros::NodeHandle  nh;

// ----------- ultrasonic sensor - distance
std_msgs::Int32 leftUltrasonic_distance; 
std_msgs::Int32 middleUltrasonic_distance; 
std_msgs::Int32 rightUltrasonic_distance; 

ros::Publisher pubLeftUltrasonic("sensor/ultrasonic/left/distance", &leftUltrasonic_distance); 
ros::Publisher pubMiddleUltrasonic("sensor/ultrasonic/middle/distance", &middleUltrasonic_distance); 
ros::Publisher pubRightUltrasonic("sensor/ultrasonic/right/distance", &rightUltrasonic_distance); 

//lights 
ros::Subscriber<std_msgs::Float32> subLedStrip(cmd_led_strip_topic, led_strip_controler_ros );
// ---------- imu sensor - orientation
std_msgs::Float32 imu_yaw; 

ros::Publisher pubIMUyaw("sensor/imu/yaw", &imu_yaw); 



void setup(){

  nh.initNode(); 
  led_strip_init();

  nh.subscribe(subLedStrip);
  nh.advertise(pubLeftUltrasonic);
  nh.advertise(pubMiddleUltrasonic);
  nh.advertise(pubRightUltrasonic);
  nh.advertise(pubIMUyaw);

  pinMode(LED_BUILD_IN,OUTPUT);
  digitalWrite(LED_BUILD_IN,LOW);

   Serial.begin(9600);

  previousTime = millis();

  nh.getHardware()->setBaud(57600);  
  
  // check that the IMU initializes correctly
  imu_status, dmp_status = imu_setup();

  // Serial.println(dmp_status); 

  // Serial.println("\n\n\n");


  // if (imu_status =!true){
  //   nh.logwarn("Failed to connect with mpu6050 sensor");
  // }

  // if (dmp_status =!0){
  //   nh.logwarn("DMP Initialization failed");
  // }

  if(dmp_status == 0) {
    nh.logwarn("MPU6050 connection successful");
    digitalWrite(LED_BUILD_IN, HIGH); 
  }
}


void loop(){

  int* ultrasonic_range = ultrasonic_measurments(previousTime); 
  
  leftUltrasonic_distance.data = ultrasonic_range[0]; 
  middleUltrasonic_distance.data = ultrasonic_range[1];
  rightUltrasonic_distance.data = ultrasonic_range[2];
  

  pubLeftUltrasonic.publish(&leftUltrasonic_distance);
  pubMiddleUltrasonic.publish(&middleUltrasonic_distance); 
  pubRightUltrasonic.publish(&rightUltrasonic_distance);

  //! only get the sensor data if the IMU started correctly
  if (dmp_status ==0 ){

    imu_yaw.data = imu_get_yaw(); 
    pubIMUyaw.publish(&imu_yaw); 
  }


  nh.spinOnce();

  
}   