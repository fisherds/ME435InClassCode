import serial
import time

ser = serial.Serial('/dev/tty.usbserial-DN0421L4')  # open serial port
print(ser.name)         # check which port was really used
for k in range(5):
    print("ON")
    ser.write(b'ON\n')
    time.sleep(1)
    print("OFF")
    ser.write(b'OFF\n')
    time.sleep(1)
ser.close()             # close port
print("Done")