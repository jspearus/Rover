from rover_control import RoverControl
from Lidar import lidarNav
import time


rc = RoverControl()
ln = lidarNav()

print("Rover Terminal Control")



#rc.Serial_out()
ln.startLidar()
time.sleep(5)
ln.stopLidar()
