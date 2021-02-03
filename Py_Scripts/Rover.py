from rover_control import RoverControl

rc = RoverControl()

print("Hello World")

rc.move_forward(5, -6)
rc.Serial_out()
