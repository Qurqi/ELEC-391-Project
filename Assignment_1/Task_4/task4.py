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

Ay = []

Gy = []

Wy = []

# Create Lists for each angle
ax1.plot(Ay, label='A', color='blue')
ax1.plot(Gy, label='G', color='red')
ax1.plot(Wy, label='W', color='green')
ax1.legend(loc='upper right')

def getValues():

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

    return A_angle, G_angle, W_angle

def animate(i,Ay,Gy,Wy):

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

    Ay.append(float(split_data[0]))
    Gy.append(float(split_data[1]))
    Wy.append(float(split_data[2]))

    Ay = Ay[-100:]
    Gy = Gy[-100:]
    Wy = Wy[-100:]

    ax1.clear()
    ax1.plot(Ay,color='blue')
    ax1.plot(Gy,color='red')
    ax1.plot(Wy,color='green')

    ax1.legend(['A', 'G', 'W'], loc='upper right')


# read in 100 data points from the serial port and plot them
ani = animation.FuncAnimation(fig, animate, frames = 100, fargs = (Ay,Gy,Wy), interval=1)
plt.show() 
 
