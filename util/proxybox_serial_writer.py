import time
import serial

while True:
  mydata = b"0,1,2,3,4,5,6,7,8,9,10,11\n"
  ser = serial.Serial('/dev/ttymxc0',
                      baudrate=115200,
                      bytesize=8,
                      parity='N',
                      stopbits=1,
                      timeout=0.5)
  ser.write(mydata)
  time.sleep(1)