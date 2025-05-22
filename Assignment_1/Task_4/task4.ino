#include "Arduino_BMI270_BMM150.h"

char buffer[20];
char userInput;
float k = 0.82;
float W_angle[2] = {0,0};
float G_angle = 0;
float NG_angle = 0;
float rate;
float NN_angle;
float G_angleC=0;
unsigned long start_time = 0;
unsigned long end_time = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  IMU.begin(); 
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  rate = (1/IMU.gyroscopeSampleRate());

}
void loop() {

  float Ax, Ay, Az, Gx, Gy, Gz, A_angle;

  // check for user input 
      userInput = Serial.read();
      // if user input is 'k' send k value
      if (IMU.accelerationAvailable()) {
              IMU.readAcceleration(Ax, Ay, Az);
      }
      if (IMU.gyroscopeAvailable()) {
                IMU.readGyroscope(Gx, Gy, Gz);

                // read the sampling rate
                // calculate the angle
                // replace G_angle[0] with G_angle[1], then replace G_angle[1] with Gy
                start_time = micros();
                G_angle = Gx*(start_time-end_time)*0.000001;
                end_time =micros();
                G_angleC = G_angleC + G_angle;
          }

      A_angle = atan(Ay/Az)*180/3.1415926;
      // calculate weighted angle
      // update previous angle reading
      NG_angle = -1*G_angle;
      NN_angle = -1*G_angleC;
      W_angle[1] = k*(W_angle[1]+NG_angle) + (1-k)*(A_angle);
      
      sprintf(buffer, "%.2f:%.2f:%.2f", A_angle, NN_angle, W_angle[1]);


      if (userInput == 'k') {
          Serial.println(k);
      }
    
      // if user input is 'd' return angle data
      if (userInput == 'd') {
          // pack the buffer with colon-delimited angle values
        //sprintf(buffer, "%.2f:%.2f:%.2f", A_angle, NG_angle, W_angle[1]);
        // send the buffer to the serial port
          
        Serial.println(buffer);
        
          
        // clear the buffer 
        //memset(buffer, 0, sizeof(buffer));
      }
      // if user input is 'n' iterate k-value by 0.01
      if (userInput == 'n') {
          k += 0.01; 
          G_angle = 0;
        
      }
      // if user input is 'e' exit
      if (userInput == 'e'){
        Serial.println("Exit signal Recieved. Transmission ended");
        
        while (1);
      }
  
  
}
