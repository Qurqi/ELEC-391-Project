#include "Arduino_BMI270_BMM150.h"

char userInput;
float x, y, z, angle;
float sampling_rate;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  IMU.begin(); 
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  sampling_rate = 1/IMU.gyroscopeSampleRate();
  Serial.println(sampling_rate);
}

void loop() {

  if(angle == NULL){
    angle = 0;
  }
  IMU.readGyroscope(x,y,z);
  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(x,y,z);
    angle = angle + x*sampling_rate;
    if(Serial.available()>0){
      userInput = Serial.read();
      if(userInput =='g'){
        Serial.println(-angle);
      }
    }
    delay(10);
  }
}
