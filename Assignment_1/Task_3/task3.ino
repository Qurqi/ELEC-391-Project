#include "Arduino_BMI270_BMM150.h"

char userInput;
float x, y, z, angle;
unsigned long start_time = 0;
unsigned long end_time = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  IMU.begin(); 
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
}

void loop() {

  if(angle == NULL){
    angle = 0;
  }
  
  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(x,y,z);
    start_time = micros();
    angle = angle + x*(start_time-end_time)*0.000001;
    end_time = micros();
    if(Serial.available()>0){
      userInput = Serial.read();
      if(userInput =='g'){
        Serial.println(-angle);
      }
    }
  }

}
