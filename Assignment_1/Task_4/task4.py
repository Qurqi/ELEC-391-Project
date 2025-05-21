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
Ax = [0]
Ay = [0]

Gx = [0]
Gy = [0]

Wx = [0]
Wy = [0]

# Create Lists for each angle
ax1.plot(Ax, Ay, label='A', color='blue')
ax1.plot(Gx, Gy, label='G', color='red')
ax1.plot(Wx, Wy, label='W', color='green')
ax1.legend(loc='upper right')

def reconfigure_graph():
    # set up the figure
    fig = plt.figure()
    ax1 = fig.add_subplot(1,1,1)
    plt.ylim(-90, 90)

    ax1.set_title('Azimuth Angle Readings vs. Time')
    ax1.set_xlabel('Time (s)')
    ax1.set_ylabel('Azimuth Angles (degrees)')

    ax1.plot(Ax, Ay, label='A', color='blue')
    ax1.plot(Gx, Gy, label='G', color='red')
    ax1.plot(Wx, Wy, label='W', color='green')

    # Create Lists for each angle
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

def animate(frame):

    Acc,Gry,Wei = getValues()
    # plot Acc, Gyr, and Wei on the same graph
    # graph must be bounded between -90 and 90
    # check if data is None. If it is, return nothing
    if Acc is None or Gry is None or Wei is None:
        return

    # Add values to angle matrices
    Ay.append(Acc)
    Gy.append(Gry)
    Wy.append(Wei)
    # add time axis
    Ax.append(frame)
    Gx.append(frame)
    Wx.append(frame)

    if len(Ax) > 100:
        Ay = Ay[-100:]
        Gy = Gy[-100:]
        Wy = Wy[-100:]
        Ax = Ax[-100:]
        Gx = Gx[-100:]
        Wx = Wx[-100:]
        
        ax1.clear()
        
    ax1.plot(Ax, Ay,color='blue')
    ax1.plot(Gx, Gy,color='red')
    ax1.plot(Wx, Wy,color='green')

    




# read in 100 data points from the serial port and plot them
ani = animation.FuncAnimation(fig, animate, frames = 100,interval=1)
plt.show() 
 

 
