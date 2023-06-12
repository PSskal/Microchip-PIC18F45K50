import serial

ser = serial.Serial(port="COM8", baudrate=9600, bytesize=8, timeout=2)
while True:
    value = ser.readline()
    valueInString = value.decode('latin-1')
    print(valueInString)
