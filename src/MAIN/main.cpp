#include <MAIN/config.h>
#include <MAIN/ultrasonic.h>
#include <MAIN/ros_com.h>
#include <MAIN/imu.h>

#include <ros.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float32.h>
#include <sensor_msgs/Imu.h>

#include "filter.h"
//#include <MAIN/laser.h>

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

// ---------- imu sensor - orientation
std_msgs::Float32 imu_yaw; 
sensor_msgs::Imu imu_orientation; 

ros::Publisher pubIMUyaw("sensor/imu/yaw", &imu_yaw); 
ros::Publisher pubIMUorientation("sensor/imu/orientation", &imu_orientation); 


void imu_setupROS()
{
  imu_orientation.header.frame_id = "imu_link"; 

  imu_orientation.orientation_covariance[0] = 0;    // orientation variance in x
  imu_orientation.orientation_covariance[1] = 0;
  imu_orientation.orientation_covariance[2] = 0;
  imu_orientation.orientation_covariance[3] = 0;    
  imu_orientation.orientation_covariance[4] = 0;    // orientation variance in y
  imu_orientation.orientation_covariance[5] = 0;
  imu_orientation.orientation_covariance[6] = 0;    
  imu_orientation.orientation_covariance[7] = 0;
  imu_orientation.orientation_covariance[8] = 0;    // orientation variance in z

  imu_orientation.angular_velocity_covariance[0] = 0;   // angular velocity variance in x
  imu_orientation.angular_velocity_covariance[1] = 0;
  imu_orientation.angular_velocity_covariance[2] = 0;
  imu_orientation.angular_velocity_covariance[3] = 0;
  imu_orientation.angular_velocity_covariance[4] = 0;   // angular velocity variance in y
  imu_orientation.angular_velocity_covariance[5] = 0;
  imu_orientation.angular_velocity_covariance[6] = 0;
  imu_orientation.angular_velocity_covariance[7] = 0;
  imu_orientation.angular_velocity_covariance[8] = 0;   // angular velocity variance in z

  imu_orientation.linear_acceleration_covariance[0] = 0;   // linear acceleration variance in x
  imu_orientation.linear_acceleration_covariance[1] = 0;
  imu_orientation.linear_acceleration_covariance[2] = 0;
  imu_orientation.linear_acceleration_covariance[3] = 0;
  imu_orientation.linear_acceleration_covariance[4] = 0;   // linear acceleration variance in y
  imu_orientation.linear_acceleration_covariance[5] = 0;
  imu_orientation.linear_acceleration_covariance[6] = 0;
  imu_orientation.linear_acceleration_covariance[7] = 0;
  imu_orientation.linear_acceleration_covariance[8] = 0;   // linear acceleration variance in z

}

void setup(){

  nh.initNode(); 

  nh.advertise(pubLeftUltrasonic);
  nh.advertise(pubMiddleUltrasonic);
  nh.advertise(pubRightUltrasonic);
  nh.advertise(pubIMUyaw);
  nh.advertise(pubIMUorientation);

  pinMode(LED_BUILD_IN,OUTPUT);
  digitalWrite(LED_BUILD_IN,LOW);

  // Serial.begin(9600);

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

    float* quaternions = imu_get_quaternion(); 
    float* linaer_acceleration = imu_get_accel(); 
    float* angular_velocity = imu_get_gyro(); 

    imu_orientation.orientation.x = quaternions[0]; 
    imu_orientation.orientation.y = quaternions[1]; 
    imu_orientation.orientation.z = quaternions[2];
    imu_orientation.orientation.w = quaternions[3]; 

    imu_orientation.linear_acceleration.x = linaer_acceleration[0]; 
    imu_orientation.linear_acceleration.y = linaer_acceleration[1]; 
    imu_orientation.linear_acceleration.z = linaer_acceleration[3]; 

    imu_orientation.angular_velocity.x = angular_velocity[0]; 
    imu_orientation.angular_velocity.y = angular_velocity[1]; 
    imu_orientation.angular_velocity.z = angular_velocity[2]; 

    pubIMUorientation.publish(&imu_orientation);
  }


  nh.spinOnce();

  
}   