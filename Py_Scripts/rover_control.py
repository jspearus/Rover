import time
import serial

class RoverControl():
  def __init__(self):
    print("complete")  


  def move_forward(self, speed, dist):
    print("speed = ", speed)
    print("distance = ", dist)

  def Serial_out(self):
    with serial.Serial('COM13', 9600, timeout=10) as ser:
      while True:
        led_on = input('Do you want the LED on? ')[0]
        if led_on in 'yY':
            ser.write(bytes('YES\n','utf-8'))
        if led_on in 'Nn':
            ser.write(bytes('NO\n','utf-8')) 



  if __name__ =='__main__':
    print("rover control class")
    print("run form import")