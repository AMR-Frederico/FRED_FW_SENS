#include <ros.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Float32.h>
#include <Arduino.h>
#include <MAIN/config.h>

bool rosConnected(ros::NodeHandle  nh,bool _connect){
    bool connected = nh.connected();
    if(_connect != connected){
        _connect = connected;
        digitalWrite(LED_BUILD_IN,!connected);
    }
    return connected;
}