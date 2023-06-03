#include <ros.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float32.h>

#include <sensor_msgs/Imu.h>
#include <sensor_msgs/Range.h>

#include <Arduino.h>
#include <MAIN/config.h>

// -----> PUB --------------------------------------   

#define left_ultrasonic_range_topic "sensor/range/ultrasonic/left"
#define right_ultrasonic_range_topic "sensor/range/ultrasonic/right"
#define back_ultrasonic_range_topic "sensor/range/ultrasonic/back"

#define imu_topic "sensor/orientation/imu"

// -----> SUBS --------------------------------------
#define cmd_led_strip_topic "cmd/led_strip/color"


// ----- PUB MSG ------------------------------------

sensor_msgs::Range leftUltrasonicRangeMsg;
ros::Publisher pubLeftUltrasonicRange(left_ultrasonic_range_topic, &leftUltrasonicRangeMsg); 

sensor_msgs::Range rightUltrasonicRangeMsg;
ros::Publisher pubRightUltrasonicRange(right_ultrasonic_range_topic, &rightUltrasonicRangeMsg); 

sensor_msgs::Range backUltrasonicRangeMsg;
ros::Publisher pubBackUltrasonicRange(back_ultrasonic_range_topic, &backUltrasonicRangeMsg); 

sensor_msgs::Imu imuMsg; 
ros::Publisher pubImu(imu_topic, &imuMsg);

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

    nh.advertise(pubLeftUltrasonicRange);
    nh.advertise(pubRightUltrasonicRange);
    nh.advertise(pubBackUltrasonicRange);

    nh.advertise(pubImu);

    // -------------------------------------------------------------//

    leftUltrasonicRangeMsg.header.frame_id = "left_ultrasonic_link";
    leftUltrasonicRangeMsg.radiation_type = sensor_msgs::Range::ULTRASOUND; 
    leftUltrasonicRangeMsg.field_of_view = 0.26; 
    leftUltrasonicRangeMsg.min_range = 0.05; 
    leftUltrasonicRangeMsg.max_range = 3.5; 

    rightUltrasonicRangeMsg.header.frame_id = "right_ultrasonic_link";
    rightUltrasonicRangeMsg.radiation_type = sensor_msgs::Range::ULTRASOUND; 
    rightUltrasonicRangeMsg.field_of_view = 0.26; 
    rightUltrasonicRangeMsg.min_range = 5; 
    rightUltrasonicRangeMsg.max_range = 350; 

    backUltrasonicRangeMsg.header.frame_id = "back_ultrasonic_link";
    backUltrasonicRangeMsg.radiation_type = sensor_msgs::Range::ULTRASOUND; 
    backUltrasonicRangeMsg.field_of_view = 0.26; 
    backUltrasonicRangeMsg.min_range = 5; 
    backUltrasonicRangeMsg.max_range = 350; 

}

void ros_ultrasonic(int *ultrasonic_range ) {

    // ------------------------------------------- //

    leftUltrasonicRangeMsg.header.stamp = nh.now(); 
    leftUltrasonicRangeMsg.range = ultrasonic_range[0];
    pubLeftUltrasonicRange.publish(&leftUltrasonicRangeMsg); 

    backUltrasonicRangeMsg.header.stamp = nh.now(); 
    backUltrasonicRangeMsg.range = ultrasonic_range[1];
    pubRightUltrasonicRange.publish(&rightUltrasonicRangeMsg); 

    rightUltrasonicRangeMsg.header.stamp = nh.now(); 
    rightUltrasonicRangeMsg.range = ultrasonic_range[2];
    pubBackUltrasonicRange.publish(&backUltrasonicRangeMsg); 

}

void ros_imu (float *orientation, float *orientation_covariance, 
              float *angular_velocity, float *angular_velocity_covariance,
              float *linear_acceleration, float *linear_acceleration_covariance) {

    imuMsg.header.frame_id = "imu_link"; 
    imuMsg.header.stamp = nh.now(); 

    imuMsg.orientation.x = orientation[0];
    imuMsg.orientation.y = orientation[1];
    imuMsg.orientation.z = orientation[2];
    imuMsg.orientation.w = orientation[3]; 

    imuMsg.angular_velocity.x = angular_velocity[0];
    imuMsg.angular_velocity.y = angular_velocity[1];
    imuMsg.angular_velocity.z = angular_velocity[2];

    imuMsg.linear_acceleration.x = linear_acceleration[0]; 
    imuMsg.linear_acceleration.y = linear_acceleration[1]; 
    imuMsg.linear_acceleration.z = linear_acceleration[2]; 

    for (int i = 0; i < 9; i ++) {
        imuMsg.orientation_covariance[i] = orientation_covariance[i];
        imuMsg.angular_velocity_covariance[i] = angular_velocity_covariance[i]; 
        imuMsg.linear_acceleration_covariance[i] = linear_acceleration_covariance[i];
    } 

    pubImu.publish(&imuMsg); 
}