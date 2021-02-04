import time
import serial
arduino = serial.Serial(port='/dev/ttyACM0', 9600, timeout=1)

class RoverControl():
  def __init__(self):
    print("complete")  


  def move_forward(self, speed, dist):
    print("speed = ", speed)
    print("distance = ", dist)

  def Serial_out(self):
        led_on = input('Do you want the LED on? ')[0]
        if led_on == 'y':
            ser.write(str.encode('test#\n'))
        if led_on in 'Nn':
            ser.write(bytes('NO\n','utf-8')) 



  if __name__ =='__main__':
    print("rover control class")
    print("run form import")
