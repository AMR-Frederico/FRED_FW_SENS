#include <Adafruit_MPU6050.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;
float accX, accY, accZ, gyroX, gyroY, gyroZ;
float ax_offset = 0,  ay_offset = 0,  az_offset = 0,  gx_offset = 0, gy_offset = 0,  gz_offset = 0; //variaveis de offset
float mean_ax = 0,  mean_ay = 0,  mean_az = 0,  mean_gx = 0, mean_gy = 0,  mean_gz = 0;
int acel_deadzone=8;     //Accelerometer error allowed
int giro_deadzone=1;     //Giro error allowed
int bufferSize=1000;     // Number of readings used to average

void readMPU() {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

  // Using the MotionApps library
    accX = a.acceleration.x;
    accY = a.acceleration.y;
    accZ = a.acceleration.z;
    gyroX = g.gyro.x;
    gyroY = g.gyro.y;
    gyroZ = g.gyro.z;
}

void setOffsets (float accXoff, float accYoff, float accZoff, float gyroXoff, 
                float gyroYoff, float gyroZoff)
{
    ax_offset = accXoff; 
    ay_offset = accYoff;
    az_offset = accZoff;
    gx_offset = gyroXoff;
    gy_offset = gyroYoff;
    gz_offset = gyroZoff;
}

void meanSensors_() {
  
  long i=0,buff_ax=0,buff_ay=0,buff_az=0,buff_gx=0,buff_gy=0,buff_gz=0;

  while (i<(bufferSize+101)) {
    
    // read raw accel/gyro measurements from device
    readMPU();
    
    if (i>100 && i<=(bufferSize+100)) { //First 100 measures are discarded
      buff_ax=buff_ax+accX;
      buff_ay=buff_ay+accY;
      buff_az=buff_az+accZ;
      buff_gx=buff_gx+gyroX;
      buff_gy=buff_gy+gyroY;
      buff_gz=buff_gz+gyroZ;
    }
    if (i==(bufferSize+100)){
      mean_ax=buff_ax/bufferSize;
      mean_ay=buff_ay/bufferSize;
      mean_az=buff_az/bufferSize;
      mean_gx=buff_gx/bufferSize;
      mean_gy=buff_gy/bufferSize;
      mean_gz=buff_gz/bufferSize;
    }
    i++;
    delay(2); //Needed so we don't get repeated measures
  }
}


void calibrate_() {
  ax_offset=-mean_ax/8;
  ay_offset=-mean_ay/8;
  az_offset=(16384-mean_az)/8;

  gx_offset=-mean_gx/4;
  gy_offset=-mean_gy/4;
  gz_offset=-mean_gz/4;
  while (1){
    int ready=0;
/*     mpu.setXAccelOffset(ax_offset);
    mpu.setYAccelOffset(ay_offset);
    mpu.setZAccelOffset(az_offset);

    mpu.setXGyroOffset(gx_offset);
    mpu.setYGyroOffset(gy_offset);
    mpu.setZGyroOffset(gz_offset); */

    // Get the mean values from the sensor  
    meanSensors_();

    if (abs(mean_ax)<=acel_deadzone) ready++;
    else ax_offset=ax_offset-mean_ax/acel_deadzone;

    if (abs(mean_ay)<=acel_deadzone) ready++;
    else ay_offset=ay_offset-mean_ay/acel_deadzone;

    //Padrão de retorno correto: 0 for X and Y accel, and +16384 for Z accel, por isso a subtração de 16384 no Z
    if (abs(16384-mean_az)<=acel_deadzone) ready++;
    else az_offset=az_offset+(16384-mean_az)/acel_deadzone;

    if (abs(mean_gx)<=giro_deadzone) ready++;
    else gx_offset=gx_offset-mean_gx/(giro_deadzone+1);

    if (abs(mean_gy)<=giro_deadzone) ready++;
    else gy_offset=gy_offset-mean_gy/(giro_deadzone+1);

    if (abs(mean_gz)<=giro_deadzone) ready++;
    else gz_offset=gz_offset-mean_gz/(giro_deadzone+1);

    if (ready==6) break;
  }
}

void loop() {

    /* Get new sensor events with the readings */
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    /* Print out the values */
    Serial.print("Acceleration X: ");
    Serial.print(a.acceleration.x);
    Serial.print(", Y: ");
    Serial.print(a.acceleration.y);
    Serial.print(", Z: ");
    Serial.print(a.acceleration.z);
    Serial.println(" m/s^2");

    Serial.print("Rotation X: ");
    Serial.print(g.gyro.x);
    Serial.print(", Y: ");
    Serial.print(g.gyro.y);
    Serial.print(", Z: ");
    Serial.print(g.gyro.z);
    Serial.println(" rad/s");

    /* Print out the values - offset */
    Serial.print("Acceleration X offset: ");
    Serial.print(a.acceleration.x - ax_offset);
    Serial.print(", Y: ");
    Serial.print(a.acceleration.y - ay_offset);
    Serial.print(", Z: ");
    Serial.print(a.acceleration.z - az_offset);
    Serial.println(" m/s^2");

    Serial.print("Rotation X offset: ");
    Serial.print(g.gyro.x - gx_offset);
    Serial.print(", Y: ");
    Serial.print(g.gyro.y- gy_offset);
    Serial.print(", Z: ");
    Serial.print(g.gyro.z - gz_offset);
    Serial.println(" rad/s");

    Serial.print("Temperature: ");
    Serial.print(temp.temperature);
    Serial.println(" degC");

    Serial.println("");
    delay(500);
}



//----------------------------------------------------//
// Calibrate bias of the accelerometer and gyroscope
// Sensor needs to be calibrated at each power cycle.
//----------------------------------------------------//
void calibrateSensor()
{
  // Initialize the offset values
  setOffsets(0, 0, 0, 0, 0, 0);
  
  meanSensors_();
  delay(1000);

  calibrate_();
  delay(1000);
}


void setup(void) {
    Serial.begin(115200);
    while (!Serial)
        delay(10);

    Serial.println("Adafruit MPU6050 test!");

    // Try to initialize!
    if (!mpu.begin()) {
        Serial.println("Failed to find MPU6050 chip");
        while (1) {
        delay(10);
        }
    }
    Serial.println("MPU6050 Found!");

    //? set the acelerometer range, test to get the best range
    // mpu.setAccelerometerRange(MPU6050_RANGE_2_G);          //! acceptables values: 2G (default value), 4G, 8G, 16G 
    // mpu.setGyroRange(MPU6050_RANGE_250_DEG);               //! acceptables values: 250; 500; 1000; 2000 (deg/s) 

    mpu.setFilterBandwidth(MPU6050_BAND_10_HZ); 
 
}
