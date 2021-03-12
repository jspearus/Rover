import time
import serial


SerPort = serial.Serial(
    port="/dev/ttyTHS1",
    baudrate=9600,
    bytesize=serial.EIGHTBITS,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
)

class RoverControl():
  def __init__(self):
    print("Initialized")  


  def move_Rover(self, Throttle, Steering):
    SerPort.write(bytes('move@','utf-8') + bytes(Throttle,'utf-8') + bytes('$', 'utf-8') + bytes(Steering,'utf-8') + bytes('#\n','utf-8'))
    

  def Serial_out(self):
     while True:
        Throttle = input('Enter Throttle? -100 to 100 ')
        if Throttle == 'q':
          SerPort.close()
          print("Serial Com Terminated")
          break
        Steering = input('Enter Steering? -100 to 100 ')
        if Steering == 'q':
          SerPort.close()
          print("Serial Com Terminated")
          break
        SerPort.write(bytes('move@','utf-8') + bytes(Throttle,'utf-8') + bytes('$', 'utf-8') + bytes(Steering,'utf-8') + bytes('#\n','utf-8'))  
        time.sleep(1)
        SerPort.write(bytes('move@','utf-8') + bytes('0', 'utf-8') + bytes('$', 'utf-8') + bytes('0','utf-8') + bytes('#\n','utf-8')) 
        print("rover Stopped")




  if __name__ =='__main__':
    print("rover control class")
    print("run form import")