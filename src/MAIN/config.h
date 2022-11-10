#include <Arduino.h>

// ------------- FILTERS CONFIGURATIONS 
#define NUMBER_SAMPLES 5


// -------------- ULTRASONIC CONFIG 
#define NUMBER_ULTRASONIC_SENSORS 3
#define ULTRASONIC_MAX_RANGE 400    // value in centimeters 

#define ECHO_1 36
#define TRIG_1 25

#define ECHO_2 34
#define TRIG_2 27

#define ECHO_3 32
#define TRIG_3 12


// ----------- LASER CONFIG 
#define XSHUNT_1 19
#define XSHUNT_2 22 

// ----------- DEBUG 
#define  LED_BUILD_IN  2