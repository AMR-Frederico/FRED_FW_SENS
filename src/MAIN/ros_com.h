#include <Arduino.h>
#include "ros.h"

#include <std_msgs/Float32.h>

#include <MAIN/config.h>
#include <MAIN/led_strip.h>

// ----------> Publishers

#define sensor_ultrasonic_left "sensor/data/ultrasonic/left"
#define sensor_ultrasonic_middle "sensor/data/ultrasonic/middle"
#define sensor_ultrasonic_right "sensor/data/ultrasonic/right"

#define sensor_imu_yaw "sensor/data/imu/yaw"


// ----------> Subscriber
#define sinalization_led_strip "sensor/sinalization/led_strip"

ros::NodeHandle nh; 

// ---------> Publishers
std_msgs::Float32 ultrasonicLeftMsg; 
ros::Publisher pubUltrasonicLeft(sensor_ultrasonic_left, &ultrasonicLeftMsg); 

std_msgs::Float32 ultrasonicMiddleMsg; 
ros::Publisher pubUltrasonicMiddle(sensor_ultrasonic_middle, &ultrasonicMiddleMsg); 

std_msgs::Float32 ultrasonicRightMsg; 
ros::Publisher pubUltrasonicRight(sensor_ultrasonic_right, &ultrasonicRightMsg);

std_msgs::Float32 imuYawMsg; 
ros::Publisher pubImuYaw(sensor_imu_yaw, &imuYawMsg);

// --------> Subscribers 
ros::Subscriber<std_msgs::Float32> subLedColor(sinalization_led_strip, led_strip_controler_ros); 


bool rosConnected(ros::NodeHandle  nh,bool _connect){
    bool connected = nh.connected();
    if(_connect != connected){
        _connect = connected;
        digitalWrite(LED_BUILD_IN,!connected);
    }
    return connected;
}

void ros_init() {

    led_strip_init();

    nh.initNode(); 

    nh.advertise(pubUltrasonicLeft); 
    nh.advertise(pubUltrasonicMiddle);
    nh.advertise(pubUltrasonicRight); 

    nh.advertise(pubImuYaw); 

    nh.subscribe(subLedColor); 

    nh.getHardware()->setBaud(57600);  

}

void ros_loop(   float ultrasonic_left,    float ultrasonic_middle, 
                 float ultrasonic_right,   float imu_yaw){
    
    ultrasonicLeftMsg.data = ultrasonic_left; 
    pubUltrasonicLeft.publish(&ultrasonicLeftMsg); 

    ultrasonicMiddleMsg.data = ultrasonic_middle; 
    pubUltrasonicMiddle.publish(&ultrasonicMiddleMsg);

    ultrasonicRightMsg.data = ultrasonic_right; 
    pubUltrasonicRight.publish(&ultrasonicRightMsg);

    imuYawMsg.data = imu_yaw;
    pubImuYaw.publish(&imuYawMsg); 

}