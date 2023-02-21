#include <MAIN/config.h>

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"

#include "MPU6050_6Axis_MotionApps20.h"

// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

MPU6050 mpu;

// define output readings
#define OUTPUT_READABLE_QUATERNION   // quaternion is the standart for imu readings
#define OUTPUT_READABLE_YAWPITCHROLL // get yaw is enought for odometry

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;        // [w, x, y, z]         quaternion container
VectorInt16 accel;   // [x, y, z]            accel sensor measurements
VectorInt16 gyro;    // [x, y, z]            gyro sensor measurements
VectorInt16 aaReal;  // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld; // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity; // [x, y, z]            gravity vector
float euler[3];      // [psi, theta, phi]    Euler angle container
float ypr[3];        // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector        // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
float quaternion[3];
float acceleration[3];
float velocity[3];

bool imu_setup()
{
// join I2C bus (I2Cdev library doesn't do this automatically)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
    Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
#endif

    // Serial.begin(115200);
    //  while (!Serial); // wait for Leonardo enumeration, others continue immediately

    // // wait for ready
    // Serial.println(F("\nSend any character to begin DMP programming and demo: "));
    // while (Serial.available() && Serial.read()); // empty buffer
    // while (!Serial.available());                 // wait for data
    // while (Serial.available() && Serial.read()); // empty buffer again

    // delay(5);

    // initialize device
    //Serial.println(F("Initializing I2C devices..."));
    mpu.initialize();

    // verify connection
    Serial.println(F("Testing device connections..."));
    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

    // load and configure the DMP
    Serial.println(F("Initializing DMP..."));
    devStatus = mpu.dmpInitialize();

    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

    // make sure it worked (returns 0 if so)
    if (devStatus == 0)
    {
        // Calibration Time: generate offsets and calibrate our MPU6050
        mpu.CalibrateAccel(6);
        mpu.CalibrateGyro(6);
        // mpu.PrintActiveOffsets();
        //  turn on the DMP, now that it's ready
        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    }
    else
    {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }
    return mpu.testConnection(), devStatus;
}

float imu_get_yaw()
{

    // read a packet from FIFO
    if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer))
    { // Get the Latest packet

        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

        //* display Euler angles in radians
        // Serial.print("ypr\t");
        // Serial.println(ypr[0], 5); // yaw
        // Serial.print("\t");
        // Serial.println(ypr[1],5);       //pitch
        // Serial.print("\t");
        // Serial.println(ypr[2],5);       //roll
        // Serial.print("\t");
    }
    return ypr[0];
}

float *imu_get_quaternion()
{

    // read a packet from FIFO
    if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer))
    {                                         // Get the Latest packet
        mpu.dmpGetQuaternion(&q, fifoBuffer); // quaternion
    }

    quaternion[0] = q.x;
    quaternion[1] = q.y;
    quaternion[2] = q.z;
    quaternion[3] = q.w;

    return quaternion;
}

float *imu_get_accel()
{

    // read a packet from FIFO
    if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer))
    {                                        // Get the Latest packet
        mpu.dmpGetAccel(&accel, fifoBuffer); // linar acceleratio
    }

    acceleration[0] = accel.x;
    acceleration[1] = accel.y;
    acceleration[2] = accel.z;

    return acceleration;
}

float *imu_get_gyro()
{

    // read a packet from FIFO
    if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer))
    {                                      // Get the Latest packet
        mpu.dmpGetGyro(&gyro, fifoBuffer); // angular velocity
    }

    velocity[0] = gyro.x;
    velocity[1] = gyro.y;
    velocity[2] = gyro.z;

    return velocity;
}