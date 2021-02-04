import time
import serial
arduino = serial.Serial('/dev/ttyACM0', 9600, timeout=1)

class RoverControl():
  def __init__(self):
    print("Initialized")  


  def move_forward(self, speed, dist):
    print("speed = ", speed)
    print("distance = ", dist)

  def Serial_out(self):
     while True:
        Throttle = input('Enter Throttle? -100 to 100 ')
        Steering = input('Enter Steering? -100 to 100 ')
        arduino.write(bytes('move@','utf-8')) 
        arduino.write(bytes(Throttle,'utf-8'))
        arduino.write(bytes('-', 'utf-8')) 
        arduino.write(bytes(Steering,'utf-8'))
        arduino.write(bytes('#\n','utf-8')) 
        time.sleep(1)
        arduino.write(bytes('move@','utf-8')) 
        arduino.write(bytes('0','utf-8'))
        arduino.write(bytes('-', 'utf-8')) 
        arduino.write(bytes('0','utf-8'))
        arduino.write(bytes('#\n','utf-8')) 
        print("rover Stopped")




  if __name__ =='__main__':
    print("rover control class")
    print("run form import")
