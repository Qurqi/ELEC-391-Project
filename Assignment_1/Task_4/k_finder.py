import serial
import time
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style
import numpy as np
import statistics


# create array and dictionary to store data
data = []
data_dict = {}
variance_dict = {}
mean_dict = {}
k_value = 0.0
i = 0

# set up the serial connection
ser = serial.Serial('/dev/ttyACM0', baudrate = 115200, timeout=1)
time.sleep(2) # wait for the serial connection to establish
style.use('fivethirtyeight')
# set up the figure
fig = plt.figure()
ax1 = fig.add_subplot(1,1,1)
plt.ylim(-90, 90)

ax1.set_title('Azimuth Angle Readings vs. Time')
ax1.set_xlabel('Time (s)')
ax1.set_ylabel('Azimuth Angles (degrees)')

# Create Lists for each angle
Ax = []
Ay = []

Gx = []
Gy = []

Wx = []
Wy = []

# Create Lists for each angle
ax1.plot(Ax, Ay, label='A', color='blue')
ax1.plot(Gx, Gy, label='G', color='red')
ax1.plot(Wx, Wy, label='W', color='green')
ax1.legend(loc='upper right')

ser.write(b'k')
# read in the k-value from the serial port
## wait for the k-value to be sent
k_value = float(ser.readline().decode('utf-8').strip())
print("k-value: ", k_value)
ser.flushInput()

def getValues():

    print("Getting values...")
    ser.write(b'd') # send a byte to the Arduino to start sending data

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
    A_angle = float(split_data[0])
    G_angle = float(split_data[1])
    W_angle = float(split_data[2])
    data.append(W_angle)

    return A_angle, G_angle, W_angle

while k_value < 1: ## change to k < 0.03 FOR TESTING. Bump to LT 1 after testing

    # read in the k-value for the current iteration
    # send arduino a 'k' to signal it to send the k-value
    ser.write(b'k')
    # read in the k-value from the serial port
    ## wait for the k-value to be sent
    k_value = float(ser.readline().decode('utf-8').strip())
    print("k-value: ", k_value)
    ser.flushInput()

    # read in 100 data points from the serial port and plot them
    #ani = animation.FuncAnimation(fig, animate, frames = 100, fargs = (Ay,Gy,Wy), interval=1)
    #plt.show() 
    for i in range(100):
        # get the values from the Arduino
        Ad,Gd,Wd = getValues()
        i+=1
    
    i = 0  
    # convert that list into a numpy array  
    data = np.array(data) 
    print("Data: ", data)
    # store that array in a dictionary with a key of the given k-value
    data_dict[k_value] = data
    # clear the data list for the next k-value
    data = []
     # clear the plot
    # send signal to the Arduino to increment to the next k-value
    ser.write(b'n') # send a byte to the Arduino to increment the k-value



# after reading data for all k values between 0.01 and 0.99, find the mean and variance of each k-value matrix
for k_value, data in data_dict.items():
    # calculate the mean and variance of the data
    mean = np.mean(data)
    variance = np.var(data)
    # store the mean and variance in a dictionary with the k-value as the key
    mean_dict[k_value] = mean
    variance_dict[k_value] = variance
# plot the mean and variance of each k-value

print("Data Dictionary: ", data_dict)
fig, ax = plt.subplots()
ax.plot(list(mean_dict.keys()), list(mean_dict.values()), label='Mean', color='blue')
ax.plot(list(variance_dict.keys()), list(variance_dict.values()), label='Variance', color='red')
ax.set_title('Mean and Variance of Azimuth Angles vs. k-value')
ax.set_xlabel('k-value')
ax.set_ylabel('Mean and Variance')
ax.legend(loc='upper right')
plt.show()
# close the serial connection
ser.close()


    



    