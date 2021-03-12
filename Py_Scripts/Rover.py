from rover_control import RoverControl
from Lidar import lidarNav
import time


rc = RoverControl()
ln = lidarNav()

print("Rover Terminal Control")



#rc.Serial_out()
Throttle = input("Enter Throttle: ")
Steering = input("Enter Steering: ")
rc.move_Rover(Throttle, Steering)
time.sleep(1)
rc.move_Rover(0,0)
ln.startLidar()
time.sleep(5)
ln.stopLidar()
