#include "Arduino_BMI270_BMM150.h"

char buffer[20];
char userInput;
float k = 0.01;
float W_angle[2] = {0,0};
float G_angle = 0;
float rate;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  IMU.begin(); 
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  rate = IMU.gyroscopeSampleRate();
  rate = (1/rate);


  
}
void loop() {

  float Ax, Ay, Az, Gx, Gy, Gz, A_angle;

  // check for user input 
  if (Serial.available() > 0) {
      userInput = Serial.read();
      // if user input is 'k' send k value
      if (userInput == 'k') {
          Serial.println(k);
      }
    
      // if user input is 'd' return angle data
      if (userInput == 'd') {
          if (IMU.accelerationAvailable()) {
                  IMU.readAcceleration(Ax, Ay, Az);
                  A_angle = atan(Ay/Az)*180/3.1415926;
          }

              // Read Gyroscope values
          if (IMU.gyroscopeAvailable()) {
                IMU.readGyroscope(Gx, Gy, Gz);
                // read the sampling rate
                // calculate the angle
                // replace G_angle[0] with G_angle[1], then replace G_angle[1] with Gy
                G_angle = G_angle + Gx*(rate);
          }

          // calculate weighted angle

          // update previous angle reading
          W_angle[1] = k*(W_angle[0]+(G_angle*-1)) + (1-k)*(A_angle);

          // pack the buffer with colon-delimited angle values
          sprintf(buffer, "%.2f:%.2f:%.2f", A_angle, (G_angle*-1), W_angle[1]);
          // send the buffer to the serial port
          Serial.println(buffer);
          //W_angle[0] = W_angle[1];

          // clear the buffer 
          memset(buffer, 0, sizeof(buffer));
      }
      // if user input is 'n' iterate k-value by 0.01
      if (userInput == 'n') {
          k += 0.01;
          
      }
      // if user input is 'e' exit
      if (userInput == 'e'){
        Serial.println("Exit signal Recieved. Transmission ended");
        while (1);
      }
  }
  
}
