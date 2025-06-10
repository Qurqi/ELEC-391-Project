import serial
import time
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style
import numpy as np
import statistics

##FINDS THE LOWEST MEAN FREQUENCY CONTENT FOR EACH k-VALUE
## Reads in 100 samples from arduino as it is roating back and forth
## takes the DFT of the data and finds the mean frequency content
## the k-value with the lowest mean frequency content is the k-value which gives the smoothest curve


# create array and dictionary to store data
dataA_dict = {}
dataB_dict = {} # dictionary to store data for each k-value

k_value = 0.0
# set up the serial connection
ser = serial.Serial('/dev/ttyUSB0', baudrate = 115200, timeout=1)

time.sleep(2) # wait for the serial connection to establish
style.use('fivethirtyeight')

# Create Lists for each motor RPM
Ra = 0.0## RPM A
Rb = 0.0 ## RPM B


ser.write(b'k')
# read in the k-value from the serial port
## wait for the k-value to be sent
k_value = float(ser.readline().decode('utf-8').strip())
print("i-value: ", k_value)
ser.flushInput()

def getValues():
    ser.write(b'd') # send a byte to the Arduino to start sending data

    # read in the data from the serial port   
    # append curent rate to  
    arduinoData = ser.readline().decode('utf-8')
    ser.flushInput()
    # Validate data somehow
    # check if the data is empty
    if arduinoData == '':
        print("No data received")
        return None, None, None
    # break up data into a list.
    # data is colon delimited
    split_data = arduinoData.split(':')

    Ra = int(split_data[0])
    Rb = int(split_data[1])

    return Ra, Rb

while k_value < 1.0: ## change to k < 0.03 FOR TESTING. Bump to LT 1 after testing
    
    # read in the k-value for the current iteration
    # send arduino a 'k' to signal it to send the k-value
    ser.write(b'k')
    # read in the k-value from the serial port
    ## wait for the k-value to be sent
    k_value = float(ser.readline().decode('utf-8').strip())
    print("i-value: ", k_value)
    ser.flushInput()

    Ra,Rb = getValues()
    
    # convert that list into a numpy array  
    print("DataA: ", Ra)
    print("DataB: ", Rb)
    # store that array in a dictionary with a key of the given k-value
    dataA_dict[k_value] = Ra
    dataB_dict[k_value] = Rb
    
    # send signal to the Arduino to increment to the next k-value
    ser.write(b'n') # send a byte to the Arduino to increment the k-value


# end communication with the Arduino
ser.write(b'e') # send a byte to the Arduino to increment the k-value

# plot RPM for A and B
fig, ax = plt.subplots()
ax.plot(dataA_dict.keys(), dataA_dict.values(), label='Duty Cycle Vs. RPM', color='Orange')
ax.plot(dataB_dict.keys(), dataB_dict.values(), label='Duty Cycle Vs. RPM', color='blue')
ax.set_title('RPM values Vs. PWM values')
ax.set_xlabel('PWM Values (duty cycle percentage)')
ax.set_ylabel('RPM in rotations/min')
ax.legend(loc='upper right')
plt.show()
# close the serial connection
ser.close()


    



    
