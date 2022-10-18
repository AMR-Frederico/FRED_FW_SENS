#include <ros.h>

#include <sensor_msgs/Range.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Float32.h>
#include <ros/time.h>

#include <MAIN/config.h>

//------------ Publishers -----------------

// Measurement of the vl53l0x sensors
#define left_laser_measure_topic "sensor_readings/distance/laser/left"
#define right_laser_measure_topic "sensor_readings/distance/laser/right"

// Measurement of the hc-sr04 sensors
#define left_ultrasonic_measure_topic "sensor_readings/distance/ultrasonic/left"
#define middle_ultrasonic_measure_topic "sensor_readings/distance/ultrasonic/middle"
#define right_ultrasonic_measure_topic "sensor_readings/distance/ultrasonic/right"

ros::NodeHandle  nh;

// ------ ultrasonic sensor 
sensor_msgs::Range ultrasonicLeftMsg;
ros::Publisher pub_ultrasonicLeft(left_ultrasonic_measure_topic, &ultrasonicLeftMsg);

sensor_msgs::Range ultrasonicMiddleMsg;
ros::Publisher pub_ultrasonicMiddle(middle_ultrasonic_measure_topic, &ultrasonicMiddleMsg);

sensor_msgs::Range ultrasonicRightMsg;
ros::Publisher pub_ultrasonicRight(right_ultrasonic_measure_topic, &ultrasonicRightMsg);

// ------ laser sensor 
sensor_msgs::Range laserLeftMsg; 
ros::Publisher pub_laserLeft(left_laser_measure_topic, &laserLeftMsg); 

sensor_msgs::Range laserRightMsg; 
ros::Publisher pub_laserRight(right_laser_measure_topic, &laserRightMsg); 

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

    nh.advertise(pub_laserLeft); 
    nh.advertise(pub_laserRight); 

    nh.advertise(pub_ultrasonicLeft); 
    nh.advertise(pub_ultrasonicMiddle); 
    nh.advertise(pub_ultrasonicRight); 

    // -------- static fields of the ULTRASONIC SENSOR messages 
    ultrasonicLeftMsg.radiation_type = sensor_msgs::Range::ULTRASOUND; 
    ultrasonicLeftMsg.header.frame_id = "map"; 
    ultrasonicLeftMsg.field_of_view = 0.26;        // field of view -> 15° or 0.26 radians 
    ultrasonicLeftMsg.min_range = 0.05;            // minimum  distance of detection in meters
    ultrasonicLeftMsg.max_range = 4;               // maximum  distance of detection in meters 

    ultrasonicMiddleMsg.radiation_type = sensor_msgs::Range::ULTRASOUND; 
    ultrasonicMiddleMsg.header.frame_id = "map"; 
    ultrasonicMiddleMsg.field_of_view = 0.26; 
    ultrasonicMiddleMsg.min_range = 0.05;
    ultrasonicMiddleMsg.max_range = 4; 

    ultrasonicRightMsg.radiation_type = sensor_msgs::Range::ULTRASOUND; 
    ultrasonicRightMsg.header.frame_id = "map"; 
    ultrasonicRightMsg.field_of_view = 0.26; 
    ultrasonicRightMsg.min_range = 0.05; 
    ultrasonicRightMsg.max_range = 4; 

    // -------- static fields of the LASER SENSOR messages 
    laserLeftMsg.radiation_type = sensor_msgs::Range::INFRARED; 
    laserLeftMsg.header.frame_id = "map"; 
    laserLeftMsg.field_of_view = 0.44;        // field of view -> 25° or 0.44 radians  
    laserLeftMsg.min_range = 0.01; 
    laserLeftMsg.max_range = 2; 

    laserRightMsg.radiation_type = sensor_msgs::Range::INFRARED; 
    laserRightMsg.header.frame_id = "map"; 
    laserRightMsg.field_of_view = 0.44; 
    laserRightMsg.min_range = 0.01; 
    laserRightMsg.max_range = 2; 

}

void ros_loop(){

    // ULTRASOUND SENSOR 
    ultrasonicLeftMsg.header.stamp = nh.now(); 
    ultrasonicLeftMsg.range = ultrasonicLeftMeasure;
    pub_ultrasonicLeft.publish(&ultrasonicLeftMsg); 

    ultrasonicMiddleMsg.header.stamp = nh.now(); 
    ultrasonicMiddleMsg.range = ultrasonicMiddleMeasure;
    pub_ultrasonicMiddle.publish(&ultrasonicMiddleMsg);  

    ultrasonicRightMsg.header.stamp = nh.now(); 
    ultrasonicRightMsg.range = ultrasonicRightMeasure;
    pub_ultrasonicRight.publish(&ultrasonicRightMsg);  

    // LASER SENSOR
    laserLeftMsg.header.stamp = nh.now(); 
    laserLeftMsg.range = laserLeftMeasure;
    pub_laserLeft.publish(&laserLeftMsg); 

    laserRightMsg.header.stamp = nh.now();
    laserRightMsg.range = laserRightMeasure; 
    pub_laserRight.publish(&laserRightMsg); 

    nh.spinOnce();

}