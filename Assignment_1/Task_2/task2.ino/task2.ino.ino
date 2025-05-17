#include "Arduino_BMI270_BMM150.h"

char buffer[20];
char userInput;
float k = 0.01;
float G_angle[2] = {0, 0};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  IMU.begin(); 
  if (!IMU.begin()) {
    Serial.println("38967a:Failed to initialize IMU!:38967a");
    while (1);
  }
  
}

void loop() {

  float Ax, Ay, Az, Gx, Gy, Gz, A_angle, W_angle;

  // check for user input 
  if (Serial.available() > 0) {
      userInput = Serial.read();
      // if user input is 'k' send k value
      if (userInput == 'k') {
          sprintf(buffer, "38967a:%.2f:38967a", k);
          Serial.println(k);
          memset(buffer, 0, sizeof(buffer));

      }
    
      // if user input is 'd' return angle data
      if (userInput == 'd') {
          if (IMU.accelerationAvailable()) {
                  IMU.readAcceleration(Ax, Ay, Az);
                  A_angle = atan(Ay/Az)*180/3.1415926;
                  //Serial.println(angle);
          }

              // Read Gyroscope values
          if (IMU.gyroscopeAvailable()) {
                IMU.readGyroscope(Gx, Gy, Gz);
                // read the sampling rate
                float rate = IMU.gyroscopeSampleRate();
                // calculate the angle
                // replace G_angle[0] with G_angle[1], then replace G_angle[1] with Gz
                G_angle[0] = G_angle[1];
                G_angle[1] = Gz*(1/rate);
          }

          // calculate weighted angle
          W_angle = k*(G_angle[0]+G_angle[1]) + (1-k)*(A_angle);

          // pack the buffer with colon-delimited angle values
          sprintf(buffer, "38967a:%.2f:%.2f:%.2f:38967a", A_angle, G_angle[1], W_angle);
          // send the buffer to the serial port
          Serial.println(buffer);
          // clear the buffer 
          memset(buffer, 0, sizeof(buffer));
      }
      // if user input is 'n' iterate k-value by 0.01
      if (userInput == 'n') {
          k += 0.01;
          if (k > 1) {
              k = 1;
          }
      }
      // if user input is 'e' exit
      if (userInput == 'e'){
        Serial.println("38967a:Exit signal Recieved. Transmission ended:38967a");
        while (1);
      }
  }
  
}