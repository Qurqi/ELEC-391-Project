import time
import serial 
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import matplotlib.ticker as ticker

def animate(i, dataList, ser):
    ser.write(b'g')
    arduinoData_string = ser.readline().decode('ascii')

    try:
        arduinoData_float = float(arduinoData_string)
        dataList.append(arduinoData_float)

    except:
        pass

    dataList =dataList[-50:]
    
    ax.clear()
    ax.plot(dataList)
    ax.yaxis.set_major_locator(ticker.MultipleLocator(10))
    ax.set_ylim([-90, 90])
    
    ax.set_title('Azimuth Angle Readings vs. Time')
    ax.set_xlabel('Time (s)')
    ax.set_ylabel('Azimuth Angles (degrees)')

dataList = []

fig  = plt.figure()
ax = fig.add_subplot(111)

ser = serial.Serial("COM3", 9600)
time.sleep(2)

ani = animation.FuncAnimation(fig, animate, frames=100, fargs=(dataList, ser), interval = 100)
plt.show()
