#include <MAIN/config.h>
#include <Ultrasonic.h>
#include "filter.h"

// objects for the hc-sr04
Ultrasonic ultrasonic[NUMBER_ULTRASONIC_SENSORS] = {
    Ultrasonic(TRIG_1, ECHO_1, ULTRASONIC_MAX_RANGE),
    Ultrasonic(TRIG_2, ECHO_2, ULTRASONIC_MAX_RANGE),
    Ultrasonic(TRIG_3, ECHO_3, ULTRASONIC_MAX_RANGE)};

MedianFilter median_filter[NUMBER_ULTRASONIC_SENSORS] = {
    MedianFilter(NUMBER_SAMPLES, 0),
    MedianFilter(NUMBER_SAMPLES, 0),
    MedianFilter(NUMBER_SAMPLES, 0)};
/* 
Kalman kalman_filter[NUMBER_ULTRASONIC_SENSORS] = {
    Kalman(),
    Kalman(),
    Kalman()}; */

int ultrasonicArrays[NUMBER_SAMPLES][NUMBER_ULTRASONIC_SENSORS]; // array of the most recent sensor values

int currentValue_ultrasonic[NUMBER_ULTRASONIC_SENSORS];
int previousValue_ultrasonic[NUMBER_ULTRASONIC_SENSORS]; 
int filtered_ultrasonic[NUMBER_ULTRASONIC_SENSORS];
int currentSensor = 0;

int movingAverage_sample[NUMBER_SAMPLES]; 

// funcion to to shift each value on the array so that the new data can slot into position 0
void rollingValue()
{

    for (int j = 0; j <= NUMBER_ULTRASONIC_SENSORS; j++)
    {
        for (int i = NUMBER_SAMPLES-1; i >= 0; i--)
        {
            ultrasonicArrays[i][j] = ultrasonicArrays[i - 1][j];
        }

        ultrasonicArrays[0][j] = currentValue_ultrasonic[j];
    }
}

void ultrasonic_measurments(int previousTime)
{
    previousValue_ultrasonic[currentSensor] = ultrasonic[currentSensor].read(); 

    if (millis() - previousTime > 33 && currentSensor < NUMBER_ULTRASONIC_SENSORS) {
        currentValue_ultrasonic[currentSensor] = ultrasonic[currentSensor].read(CM);
        median_filter[currentSensor].in((int)currentValue_ultrasonic); 

        if ((currentValue_ultrasonic[currentSensor] - previousValue_ultrasonic[currentSensor] > 1) || (currentValue_ultrasonic[currentSensor] - previousValue_ultrasonic[currentSensor] < 1)) {
            currentValue_ultrasonic[currentSensor] = median_filter[currentSensor].out(); 
        }

        currentSensor = currentSensor + 1;  

        rollingValue(); 
    }

    if (currentSensor == 2) {
        for (int j = 0; j < NUMBER_ULTRASONIC_SENSORS; j++) {
            for (int i = 0; i < NUMBER_SAMPLES; i++) {
                movingAverage_sample[i] = ultrasonicArrays[i][j]; 
            }
            
            filtered_ultrasonic[j] = moving_average_filter(movingAverage_sample, NUMBER_SAMPLES);

            Serial.println(filtered_ultrasonic[j]); 
            Serial.print(" | "); 
        }

    }
    
}
