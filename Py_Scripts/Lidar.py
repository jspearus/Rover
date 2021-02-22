import time
import serial


SerPort = serial.Serial(
    port="/dev/ttyACM0",
    baudrate=9600,
    bytesize=serial.EIGHTBITS,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
)



class lidarNav():
    def __init__(self):
        print("Lidar Initialized")  

    def startLidar(self):
       SerPort.write(bytes('start#','utf-8'))
       print("scan started") 


    def stopLidar(self):
        SerPort.write(bytes('stop#','utf-8'))
        print("scan stoped") 


    if __name__ =='__main__':
        print("Lidar Navigation class")
        print("run form import")