#include <ros.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float32.h>
#include <sensor_msgs/Imu.h>

#include <Arduino.h>
#include <MAIN/config.h>

// -----> PUB --------------------------------------   
#define left_ultrasonic_topic "sensor/ultrasonic/left/distance"
#define middle_ultrasonic_topic "sensor/ultrasonic/middle/distance"
#define right_ultrasonic_topic "sensor/ultrasonic/right/distance"

#define imu_yaw_topic "sensor/imu/yaw"

// -----> SUBS --------------------------------------
#define cmd_led_strip_topic "cmd/led_strip/color"


// ----- PUB MSG ------------------------------------
std_msgs::Int32 leftUltrasonicMsg;
ros::Publisher pubLeftUltrasonic(left_ultrasonic_topic, &leftUltrasonicMsg); 

std_msgs::Int32 middleUltrasonicMsg; 
ros::Publisher pubMiddleUltrasonic(middle_ultrasonic_topic, &middleUltrasonicMsg); 

std_msgs::Int32 rightUltrasonicMsg; 
ros::Publisher pubRightUltrasonic(right_ultrasonic_topic, &rightUltrasonicMsg); 

std_msgs::Float32 imuYawMsg; 
ros::Publisher pubIMUyaw(imu_yaw_topic, &imuYawMsg); 

// ----- SUBS MSG ----------------------------------
ros::Subscriber<std_msgs::Float32> subLedStrip(cmd_led_strip_topic, led_strip_controler_ros );

//* <------------ ROS setup ---------->
ros::NodeHandle  nh;

bool rosConnected(ros::NodeHandle  nh,bool _connect){
    bool connected = nh.connected();
    if(_connect != connected){
        _connect = connected;
        digitalWrite(LED_BUILD_IN,!connected);
    }
    return connected;
}

void ros_init(){

    nh.initNode(); 

    nh.subscribe(subLedStrip); 

    nh.advertise(pubLeftUltrasonic); 
    nh.advertise(pubMiddleUltrasonic); 
    nh.advertise(pubRightUltrasonic);
    nh.advertise(pubIMUyaw); 

}

void ros_loop( int left_ultrasonic,   int middle_ultrasonic,
               int right_ultrasonic,  int imu_yaw) {
    
    leftUltrasonicMsg.data = left_ultrasonic; 
    pubLeftUltrasonic.publish(&leftUltrasonicMsg); 

    middleUltrasonicMsg.data = middle_ultrasonic; 
    pubMiddleUltrasonic.publish(&middleUltrasonicMsg); 

    rightUltrasonicMsg.data = right_ultrasonic; 
    pubRightUltrasonic.publish(&rightUltrasonicMsg); 

    imuYawMsg.data = imu_yaw; 
    pubIMUyaw.publish(&imuYawMsg);
}