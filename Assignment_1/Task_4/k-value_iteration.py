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

# set up the serial connection
ser = serial.Serial('/dev/ttyACM0', baudrate = 115200, timeout=1)
time.sleep(2) # wait for the serial connection to establish
style.use('fivethirtyeight')

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
    data.append(W_angle)

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
    
    plt.xlim(Ax[0], Ax[-1])
    # plot the data on the same graph
    ax1.plot(Ax, Ay, label='Acc', color='blue')
    ax1.plot(Gx, Gy, label='Gry', color='red')
    ax1.plot(Wx, Wy, label='Wei', color='green')
    ax1.legend(loc='upper right')


while k_value < 1.0:

    # read in the k-value for the current iteration
    # send arduino a 'k' to signal it to send the k-value
    ser.write(b'k')
    k_value = ser.readline().decode('utf-8')
    ser.flushInput()
    # convert the k-value to a float
    k_value = float(k_value)
    # read in 100 data points from the serial port and plot them
    for i in range(100):
        ani = animation.FuncAnimation(fig, animate, frames = np.linspace(1,100,100), interval=100)
        plt.show()
        i+=1

    # reset i
    i = 0
    # convert that list into a numpy array  
    data = np.array(data) 
    # store that array in a dictionary with a key of the given k-value
    data_dict[k_value] = data
    # clear the data list for the next k-value
    data = []
     # clear the plot
    ax1.clear()
    Ax = []
    Ay = []

    Gx = []
    Gy = []

    Wx = []
    Wy = []
    # send signal to the Arduino to increment to the next k-value
    ser.write(b'n') # send a byte to the Arduino to increment the k-value


# after reading data for all k values between 0.01 and 0.99, find the mean and variance of each k-value matrix
for k_value, data in data_dict.items():
    # calculate the mean and variance of the data
    mean = np.mean(data)
    variance = np.var(data)
    # store the mean and variance in a dictionary with the k-value as the key
    data_dict[k_value] = {'mean': mean, 'variance': variance}
# plot the mean and variance of each k-value
fig, ax = plt.subplots()

ax.plot(data_dict.keys(), data['mean'], 'o', label='Mean')
ax.plot(data_dict.keys(), data['variance'], 'x', label='Variance')
ax.set_xlabel('k-value')
ax.set_ylim([min(data['mean'] and data['variance']), max(data['mean'] and data['variance'])])
ax.set_ylabel('Mean (o) /Variance (x)')
ax.set_title('Mean and Variance of Azimuth Angle Reading vs. k-value')
ax.legend()
plt.show()

    



    