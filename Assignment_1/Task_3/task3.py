import time
import serial 
import matplotlib.pyplot as plt
import matplotlib.animation as animation


def animate(i, dataList, ser):
    ser.write(b'g')
    arduinoData_string = ser.readline().decode('ascii')
    print(arduinoData_string)

    try:
        arduinoData_float = float(arduinoData_string)
        dataList.append(arduinoData_float)

    except:
        pass

    dataList =dataList[-50:]
    
    ax.clear()
    ax.plot(dataList)

    ax.set_ylim([-90, 90])


dataList = []

fig  = plt.figure()
ax = fig.add_subplot(111)

ax.set_title('Azimuth Angle Reading vs. Time')
ax.set_xlabel('Time (s)')
ax.set_ylabel('Azimuth Angle (degrees)')

ser = serial.Serial("/dev/ttyACM0", 115200)
time.sleep(2)
ser.write(b'g')


while True:
    
    ani = animation.FuncAnimation(fig, animate, frames=100, fargs=(dataList, ser), interval = 100)

    plt.show()
    ser.close()