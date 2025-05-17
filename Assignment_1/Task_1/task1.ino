#include "Arduino_BMI270_BMM150.h"

char userInput;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  IMU.begin(); 
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  
}

void loop() {

  float Ax, Ay, Az, angle;

  // check for user input 
  if (Serial.available() > 0) {
      userInput = Serial.read();    
      // if user input is 'g' return angle data
      if (userInput == 'g') {
          if (IMU.accelerationAvailable()) {
                  IMU.readAcceleration(Ax, Ay, Az);
                  angle = atan(Ay/Az)*180/3.1415926;
                  Serial.println(angle);
          }
      }
      // if user input is 'e' exit
      if (userInput == 'e'){
        Serial.println("Exit signal Recieved. Transmission ended");
        while (1);
      }
  }
  
}