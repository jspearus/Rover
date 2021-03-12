from rover_control import RoverControl
from Lidar import lidarNav
import time


rc = RoverControl()
ln = lidarNav()

print("Rover Terminal Control")



#rc.Serial_out() # for debug and testing
while True:
    Throttle = input("Enter Throttle: ")
    if Throttle == 'q':
        print("Finished")
        break
    Steering = input("Enter Steering: ")
    if Steering == 'q':
        print("Finished")
        break
    rc.move_Rover(Throttle, Steering)
    time.sleep(2) # 1 sec dosen't work...2 sec does?
    print("Stopped")
    Throttle = '0'
    Steering = '0'
    rc.move_Rover(Throttle, Steering)
ln.startLidar()
time.sleep(5)
ln.stopLidar()
