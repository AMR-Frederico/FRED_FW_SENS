#include <MAIN/config.h>
#include "Adafruit_VL53L0X.h"

// address we will assign if dual sensor is present
#define LASER1_ADDRESS 0x30
#define LASER2_ADDRESS 0x31

Adafruit_VL53L0X laserLeft = Adafruit_VL53L0X();
Adafruit_VL53L0X laserRight = Adafruit_VL53L0X();

VL53L0X_RangingMeasurementData_t laserLeft_measure;
VL53L0X_RangingMeasurementData_t laserRight_measure;

// array of the most recent 5 sensor values
int laserLeftArray[5];
int laserRightArray[5]; 

int currentValue_laserLeft;
int currentValue_laserRight; 

// swap two variables
int temp;
#define swap(w, z) temp = w; w = z; z = temp;
#define sort(x, y) if(x < y) { swap(x, y); }

void setID() {

  digitalWrite(XSHUNT_1, LOW);    
  digitalWrite(XSHUNT_2, LOW);
  delay(10);
  
  digitalWrite(XSHUNT_1, HIGH);
  digitalWrite(XSHUNT_2, HIGH);
  delay(10);

  digitalWrite(XSHUNT_1, HIGH);
  digitalWrite(XSHUNT_2, LOW);
  delay(10);

  digitalWrite(XSHUNT_2, HIGH);
  delay(10);

}

// funcion to to shift each value on the array so that the new data can slot into position 0
void rollingValue(){
    
    for (int i = 4; i > 0; i--){
        laserLeftArray[i] = laserLeftArray[i-1];
        laserRightArray[i] = laserRightArray[i-1];
    }

    laserLeftArray[0] = currentValue_laserLeft;
    laserRightArray[0] = currentValue_laserRight; 
     
}

// this funcion swapp a value, if in comparation a value is bigger than another, their position is swapped in the array
// comparing all the values, we can get the median value of the array
int median(int a, int b, int c, int d, int e){

   sort(a,b);
   sort(d,e);
   sort(a,c);
   sort(b,c);
   sort(a,d);
   sort(c,d);
   sort(b,e);
   sort(b,c);

   return c; 
}

void laser_init() {

  pinMode(XSHUNT_1, OUTPUT);
  pinMode(XSHUNT_2, OUTPUT);

  digitalWrite(XSHUNT_1, LOW);
  digitalWrite(XSHUNT_2, LOW);

  setID();
}

int laser_measurments(){

    currentValue_laserLeft = laserLeft_measure.RangeMilliMeter; 
    currentValue_laserRight = laserRight_measure.RangeMilliMeter; 

    // if the sensor reading is diferente by 5 centimeters from the last value, que can consider that trash, and replace with the median
    // the value 5 is arbitrarily
    if ((currentValue_laserLeft > laserLeftArray[0] + 5) || (currentValue_laserLeft < laserLeftArray[0] - 5)){
        currentValue_laserLeft = median(laserLeftArray[0], laserLeftArray[1], laserLeftArray[2], 
                                             laserLeftArray[3], laserLeftArray[4]); 

        currentValue_laserRight = median(laserRightArray[0], laserRightArray[1], laserRightArray[2],
                                              laserRightArray[3], laserRightArray[4]); 
    }

    // update array of sensor values
    rollingValue();

    return currentValue_laserLeft, currentValue_laserRight;
}


