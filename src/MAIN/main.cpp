#include <Arduino.h>


#define echo1 36 
#define trig1 25

#define echo2 34
#define trig2 27 

#define echo3 32 
#define trig3 12

int count = 0;

int m_ultra_1 = 0; 
int m_ultra_2 = 0; 
int m_ultra_3 = 0; 

void setup()
{
  pinMode(echo1, INPUT); 
  pinMode(echo2, INPUT); 
  pinMode(echo3, INPUT); 

  pinMode(trig1, OUTPUT);
  pinMode(trig2, OUTPUT);
  pinMode(trig3, OUTPUT);

  digitalWrite(trig1, 0);
  digitalWrite(trig2, 0);
  digitalWrite(trig3, 0);

  Serial.begin(9600);
}

long ultrasound(int trig, int echo)
{
  digitalWrite(trig, LOW); 
  delayMicroseconds(5); 
  digitalWrite(trig, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trig, LOW); 

  long time = pulseIn(echo, HIGH);  

  long distance = (time/2)*0.0343; 

  return distance; 
}

void loop() 
{
    if(ultrasound(trig1, echo1) >= 0 && ultrasound(trig1, echo1) < 100)
    {
      m_ultra_1 = m_ultra_1 + ultrasound(trig1, echo1); 
    }

    if(ultrasound(trig2, echo2) >= 0 && ultrasound(trig2, echo2) < 100)
    {
      m_ultra_2 = m_ultra_2 + ultrasound(trig2, echo2);  
    }

    if(ultrasound(trig3, echo3) >= 0 && ultrasound(trig3, echo3) < 100)
    {
      m_ultra_3 = m_ultra_3+ ultrasound(trig3, echo3);  
    }
     
    count = count + 1; 

    if (count == 100)
    { 
      m_ultra_1 = m_ultra_1/100; 
      m_ultra_2 = m_ultra_2/100; 
      m_ultra_3 = m_ultra_3/100; 

      Serial.print(m_ultra_1); 
      Serial.print(": "); 
      Serial.print(m_ultra_2); 
      Serial.print(": "); 
      Serial.println(m_ultra_3); 

      m_ultra_1 = 0;
      m_ultra_2 = 0;
      m_ultra_3 = 0;

      count = 0; 
    }

}