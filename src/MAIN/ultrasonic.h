#include <MAIN/config.h>
#include <Ultrasonic.h> 

// objects for the hc-sr04
Ultrasonic ultrasonicLeft(TRIG_1, ECHO_1); 
Ultrasonic ultrasonicMiddle(TRIG_2, ECHO_2); 
Ultrasonic ultrasonicRight(TRIG_3, ECHO_3); 

// array of the most recent 5 sensor values
int ultrasonicLeftArray[5];
int ultrasonicMiddleArray[5]; 
int ultrasonicRightArray[5]; 

int currentValue_ultrasonicLeft;
int currentValue_ultrasonicMiddle; 
int currentValue_ultrasonicRight; 

// swap two variables
int temp;
#define swap(w, z) temp = w; w = z; z = temp;
#define sort(x, y) if(x < y) { swap(x, y); }

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

int ultrasonic_measurments(){

    currentValue_ultrasonicLeft = ultrasonicLeft.read(CM); 
    currentValue_ultrasonicMiddle = ultrasonicMiddle.read(CM); 
    currentValue_ultrasonicRight = ultrasonicRight.read(CM); 

    // if the sensor reading is diferente by 5 centimeters from the last value, que can consider that trash, and replace with the median
    // the value 5 is arbitrarily
    if ((currentValue_ultrasonicLeft > ultrasonicLeftArray[0] + 5) || (currentValue_ultrasonicLeft < ultrasonicLeftArray[0] - 5)){
        currentValue_ultrasonicLeft = median(ultrasonicLeftArray[0], ultrasonicLeftArray[1], ultrasonicLeftArray[2], 
                                             ultrasonicLeftArray[3], ultrasonicLeftArray[4]); 
        
        currentValue_ultrasonicMiddle = median(ultrasonicMiddleArray[0], ultrasonicMiddleArray[1], ultrasonicMiddleArray[2],
                                               ultrasonicMiddleArray[3], ultrasonicMiddleArray[4]); 

        currentValue_ultrasonicRight = median(ultrasonicRightArray[0], ultrasonicRightArray[1], ultrasonicRightArray[2],
                                              ultrasonicRightArray[3], ultrasonicRightArray[4]); 
    }

    // update array of sensor values
    rollingValue();

    return currentValue_ultrasonicLeft, currentValue_ultrasonicMiddle, currentValue_ultrasonicRight;
}
