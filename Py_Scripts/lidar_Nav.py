
from pyrplidar import PyRPlidar

lidar = PyRPlidar()
lidar.connect(port="COM21", baudrate=115200, timeout=3)

class lidarNav():
    def __init__(self):
        print("Initialized")  

    def getInfo(self):

        info = lidar.get_info()
        print("info :", info)

        health = lidar.get_health()
        print("health :", health)

        samplerate = lidar.get_samplerate()
        print("samplerate :", samplerate)


        scan_modes = lidar.get_scan_modes()
        print("scan modes :")
        for scan_mode in scan_modes:
            print(scan_mode)


lidar.disconnect()

if __name__ =='__main__':
    print("Lidar Nav class")
    print("run form import")