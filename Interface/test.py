import serial

try:
    arduino = serial.Serial("/dev/ttyACM1", timeout = 1000)
except:
    print("Check your port")

rawdata = []
count = 0

while count < 3:
    rawdata.append(str(arduino.readline()))
    count += 1
print(rawdata)
