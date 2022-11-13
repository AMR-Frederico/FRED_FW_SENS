#include <Arduino.h>

//* pins of the ULTRASONIC sensor (hc-sr04)
#define ECHO_1 36
#define TRIG_1 25

#define ECHO_2 34
#define TRIG_2 27

#define ECHO_3 32
#define TRIG_3 12

#define ULTRASONIC_FIELD_OF_VIEW 0.26      //! value in rad 
#define ULTRASONIC_MIN_RANGE 0.05          //! value in meters
#define ULTRASONIC_MAX_RANGE 4             //! value in meters 

//* pins of LASER sensor (vl53l0x)
#define XSHUNT_1 19
#define XSHUNT_2 18

#define LASER_FIELD_OF_VIEW 0.44           //! value in rad
#define LASER_MIN_RANGE 0.05               //! value in meters 
#define LASER_MAX_RANGE 2                  //! value in meters 

// ----------- DEBUG 
#define  LED_BUILD_IN  2