#include <MAIN/config.h>
#include <MAIN/ultrasonic.h>
#include <MAIN/ros_com.h>
#include <MAIN/imu.h>
#include <RTOS.h>


#include "led_strip.h"
#include "filter.h"
//#include <MAIN/laser.h>

bool _connect = false ;
bool imu_status, dmp_status; 

float quaternions[4];           //x, y, z, w
float linaer_acceleration[3];   //x, y, z
float angular_velocity[3];      //x, y, z 

int ultrasonic_range[NUMBER_ULTRASONIC_SENSORS];  
int previousTime; 

TaskHandle_t SensorsTask;
TaskHandle_t RosTask;

void sensors_task_code(void *pvParameters);
void ros_task_code(void *pvParameters);

float leftUltrasonic; 
float middleUltrasonic;
float rightUltrasonic;

float imuYaw; 

void setup(){

  nh.initNode(); 
  led_strip_init();

  pinMode(LED_BUILD_IN,OUTPUT);
  digitalWrite(LED_BUILD_IN,LOW);

  //Serial.begin(9600);

  previousTime = millis();

  // check that the IMU initializes correctly
  imu_status, dmp_status = imu_setup();

  if(dmp_status == 0) {
    digitalWrite(LED_BUILD_IN, HIGH); 
  }

  //create a task that will be executed in the sensors_task_code function, 
  // with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
      sensors_task_code, /* Task function. */
      "SensorsTask",   /* name of task. */
      10000,     /* Stack size of task */
      NULL,      /* parameter of the task */
      1,         /* priority of the task */
      &SensorsTask,    /* Task handle to keep track of created task */
      0);        /* pin task to core 0 */

  //create a task that will be executed in the ros_task_code() function, 
  // with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
      ros_task_code, /* Task function. */
      "RosTask",   /* name of task. */
      10000,     /* Stack size of task */
      NULL,      /* parameter of the task */
      1,         /* priority of the task */
      &RosTask,    /* Task handle to keep track of created task */
      1);        /* pin task to core 1 */

}

void sensors_task_code(void *pvParameters)
{
  int* ultrasonic_range = ultrasonic_measurments(previousTime); 
  
  float leftUltrasonic = ultrasonic_range[0]; 
  float middleUltrasonic = ultrasonic_range[1];
  float rightUltrasonic= ultrasonic_range[2];

  //! only get the sensor data if the IMU started correctly
  if (dmp_status ==0 ){

    float imuYaw= imu_get_yaw(); 
  }


  vTaskDelay(1);
}

void ros_task_code(void *pvParameters)
{
  ros_init( leftUltrasonic,    float ultrasonic_middle, 
                 float ultrasonic_right,   float imu_yaw, 
                 float led_color );

  vTaskDelay(1);

}

void loop(){
  // delay(100);
}   
