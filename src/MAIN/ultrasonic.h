#include <MAIN/config.h>
#include <NewPing.h>
#include "filter.h"

// objects for the hc-sr04
NewPing ultrasonic[NUMBER_ULTRASONIC_SENSORS] = {
    NewPing(TRIG_1, ECHO_1, ULTRASONIC_MAX_RANGE), 
    NewPing(TRIG_2, ECHO_2, ULTRASONIC_MAX_RANGE), 
    NewPing(TRIG_3, ECHO_3, ULTRASONIC_MAX_RANGE)
};

MedianFilter median_filter[NUMBER_ULTRASONIC_SENSORS] = {
    MedianFilter(10, 0), 
    MedianFilter(10, 0), 
    MedianFilter(10, 0)
}; 

Kalman kalman_filter[NUMBER_ULTRASONIC_SENSORS] = {
    Kalman(),
    Kalman(), 
    Kalman()
}; 

int ultrasonicArrays[NUMBER_SAMPLES][NUMBER_ULTRASONIC_SENSORS]; // array of the most recent sensor values

int currentValue_ultrasonic[NUMBER_ULTRASONIC_SENSORS]; 
int sum_ultrasonic [NUMBER_ULTRASONIC_SENSORS]; 

// funcion to to shift each value on the array so that the new data can slot into position 0
void rollingValue(){
    
    for (int j = 0; j < NUMBER_ULTRASONIC_SENSORS; j++)
    {
        for(int i = NUMBER_SAMPLES; i > 0; j--)
        {
            ultrasonicArrays[i][j] = ultrasonicArrays[i-1][j]; 
        }

        ultrasonic[0][j] = currentValue_ultrasonic[j];
    }    
}

// calculate the sample moving average
void moving_average(){
    
    for 
    for (int i = 0; i < 5; i++){
        sum_ultrasonicLeft = sum_ultrasonicLeft + ultrasonicLeftArray[i];
        sum_ultrasonicMiddle = sum_ultrasonicMiddle + ultrasonicMiddleArray[i]; 
        sum_ultrasonicRight = sum_ultrasonicRight + ultrasonicRightArray[i];
    }

    sum_ultrasonicLeft = sum_ultrasonicLeft/5; 
    sum_ultrasonicMiddle = sum_ultrasonicMiddle/5;
    sum_ultrasonicRight = sum_ultrasonicRight/5;
 
}

int ultrasonic_measurments(){

    currentValue_ultrasonicLeft = ultrasonicLeft.read(CM); 
    currentValue_ultrasonicMiddle = ultrasonicMiddle.read(CM); 
    currentValue_ultrasonicRight = ultrasonicRight.read(CM);

    sum_ultrasonicLeft = 0; 
    sum_ultrasonicMiddle = 0; 
    sum_ultrasonicRight = 0; 

    moving_average();
    sum_ultrasonicRight = moving_average_filter(ultrasonicRightArray, 5);
   
    rollingValue();

    LeftUltrasonicFilter.in((int)currentValue_ultrasonicLeft);
    MiddleUltrasonicFilter.in((int)currentValue_ultrasonicMiddle);
    RightUltrasonicFilter.in((int)currentValue_ultrasonicRight);

    // Serial.println(sum_ultrasonicLeft);
    // Serial.print(" | ");
    // Serial.print(sum_ultrasonicMiddle); 
    // Serial.print(" | ");
    // Serial.print(sum_ultrasonicRight); 
    // Serial.println("");

    //Para testes do filtro de mediana
    Serial.print(LeftUltrasonicFilter.out());
    Serial.print(" | ");
    Serial.print(MiddleUltrasonicFilter.out());
    Serial.print(" | ");
    Serial.print(RightUltrasonicFilter.out()); 
    Serial.println("");

    //Para testes do filtro de kalman
    // Serial.print(LeftKalmanFilter.filter(currentValue_ultrasonicLeft));
    // Serial.print(" | ");
    // Serial.print(MiddleKalmanFilter.filter(currentValue_ultrasonicMiddle));
    // Serial.print(" | ");
    // Serial.print(RightKalmanFilter.filter(currentValue_ultrasonicRight)); 
    // Serial.println("");
    
    return currentValue_ultrasonicLeft, currentValue_ultrasonicMiddle, currentValue_ultrasonicRight;
}
