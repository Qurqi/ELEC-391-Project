#include "Arduino_BMI270_BMM150.h"

char userInput;
float G_angle[2] = {0, 0};

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

  float Gx, Gy, Gz;

  // check for user input 
  if (Serial.available() > 0) {
      userInput = Serial.read();
      
      // if user input is 'd' return angle data
      if (userInput == 'g') {
              // Read Gyroscope values
          if (IMU.gyroscopeAvailable()) {
                IMU.readGyroscope(Gx, Gy, Gz);
                // read the sampling rate
                float rate = IMU.gyroscopeSampleRate();
                // calculate the angle
                // replace G_angle[0] with G_angle[1], then replace G_angle[1] with Gy
                G_angle[0] = G_angle[1];
                G_angle[1] = G_angle[0] + Gy*(1/rate);
                Serial.println(G_angle[1]);
          }
      }
      // if user input is 'e' exit
      if (userInput == 'e'){
        Serial.println("Exit signal Recieved. Transmission ended");
        while (1);
      }
  }
  
}
