import time
import serial #https://pythonhosted.org/pyserial/pyserial.html#overview

counter  = 0
while True:
  mydata = f"{counter},1,2,3,4,5,6,7,8,9,10,11\n"
  ser = serial.Serial('/dev/ttymxc0',
                      baudrate=115200,
                      bytesize=8,
                      parity='N',
                      stopbits=1,
                      timeout=0.5)
  ser.write(bytes(mydata,'utf-8'))
  if counter > 255:
    counter = 0
  else:
    counter +=1
  time.sleep(1)