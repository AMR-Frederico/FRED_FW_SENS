#include <Arduino.h>
#include <ros.h>

#include <std_msgs/Int16.h>
#include <std_msgs/Float32.h>

#include <MAIN/config.h>

// ----------> Publishers

// --- ultrasonic
#define sensor_ultrasonic_left "sensor/data/ultrasonic/left"
#define sensor_ultrasonic_middle "sensor/data/ultrasonic/middle"
#define sensor_ultrasonic_right "sensor/data/ultrasonic/right"

// --- imu
#define sensor_imu_yaw "sensor/data/imu/yaw"


// ----------> Subscriber
// --- led strip
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

std_msgs::Float32 ledColorMsg; 
ros::Publisher pubLedColor(sinalization_led_strip, &ledColorMsg); 


bool rosConnected(ros::NodeHandle  nh,bool _connect){
    bool connected = nh.connected();
    if(_connect != connected){
        _connect = connected;
        digitalWrite(LED_BUILD_IN,!connected);
    }
    return connected;
}

void ros_init() {

    nh.initNode(); 

    nh.advertise(pubUltrasonicLeft); 
    nh.advertise(pubUltrasonicMiddle);
    nh.advertise(pubUltrasonicRight); 

    nh.advertise(pubImuYaw); 

    nh.advertise(pubLedColor); 

    nh.getHardware()->setBaud(57600);  

}

void ros_loop(   float ultrasonic_left,    float ultrasonic_middle, 
                 float ultrasonic_right,   float imu_yaw, 
                 float led_color ){
    
    ultrasonicLeftMsg.data = ultrasonic_left; 
    pubUltrasonicLeft.publish(&ultrasonicLeftMsg); 

    ultrasonicMiddleMsg.data = ultrasonic_middle; 
    pubUltrasonicMiddle.publish(&ultrasonicMiddleMsg);

    ultrasonicRightMsg.data = ultrasonic_right; 
    pubUltrasonicRight.publish(&ultrasonicRightMsg);

}