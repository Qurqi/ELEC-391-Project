#include "Arduino_BMI270_BMM150.h"

char userInput;

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
  float x, y, z, angle;
  // put your main code here, to run repeatedly:
  IMU.readAcceleration(x,y,z);
  if (IMU.accelerationAvailable()) {
    if(Serial.available()>0){
      userInput = Serial.read();
      if(userInput =='g'){
        IMU.readAcceleration(x, y, z);
        angle = atan(y/z)*180/3.1415926;
        Serial.println(angle);
        delay(10);
      }
    }
  }
}
