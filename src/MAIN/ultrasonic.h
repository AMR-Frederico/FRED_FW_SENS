#include <MAIN/config.h>
#include <Ultrasonic.h> 
#include "filter.h"

// objects for the hc-sr04
Ultrasonic ultrasonicLeft(TRIG_1, ECHO_1); 
Ultrasonic ultrasonicMiddle(TRIG_2, ECHO_2); 
Ultrasonic ultrasonicRight(TRIG_3, ECHO_3); 

MedianFilter LeftUltrasonicFilter(10,0);
MedianFilter MiddleUltrasonicFilter(10,0);
MedianFilter RightUltrasonicFilter(10,0);

Kalman LeftKalmanFilter();
Kalman MiddleKalmanFilter();
Kalman RightKalmanFilter();

// array of the most recent 5 sensor values
int ultrasonicLeftArray[5];
int ultrasonicMiddleArray[5]; 
int ultrasonicRightArray[5]; 

int currentValue_ultrasonicLeft;
int currentValue_ultrasonicMiddle; 
int currentValue_ultrasonicRight; 

int sum_ultrasonicLeft;
int sum_ultrasonicMiddle;
int sum_ultrasonicRight;



// funcion to to shift each value on the array so that the new data can slot into position 0
void rollingValue(){
    
    for (int i = 4; i > 0; i--){
        ultrasonicLeftArray[i] = ultrasonicLeftArray[i-1];
        ultrasonicMiddleArray[i] = ultrasonicMiddleArray[i-1]; 
        ultrasonicRightArray[i] = ultrasonicRightArray[i-1];
    }

    ultrasonicLeftArray[0] = currentValue_ultrasonicLeft;
    ultrasonicMiddleArray[0] = currentValue_ultrasonicMiddle; 
    ultrasonicRightArray[0] = currentValue_ultrasonicRight; 
    
}

// calculate the sample moving average
void moving_average(){
    
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
