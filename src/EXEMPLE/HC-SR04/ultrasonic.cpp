#include <MAIN/config.h>
#include <Ultrasonic.h>

// objects for the hc-sr04
Ultrasonic ultrasonic1(TRIG_1, ECHO_1); 
Ultrasonic ultrasonic2(TRIG_2, ECHO_2); 
Ultrasonic ultrasonic3(TRIG_3, ECHO_3); 


void setup(){

    Serial.begin(115200); 

}

void loop(){
    Serial.print("Sensor 01 (m): ");
    Serial.print(ultrasonic1.read(CM)/100);

    Serial.print("Sensor 02 (m): ");
    Serial.print(ultrasonic2.read(CM)/100);

    Serial.print("Sensor 03 (m): ");
    Serial.print(ultrasonic3.read(CM)/100);
}