import serial

ser = serial.Serial(port="COM9", baudrate=9600, bytesize=8, timeout=2)

def send_data(data):
    try:
        ser.write(data.encode('utf-8'))
    except serial.SerialException as e:
        print("Error al enviar datos:", e)

def handle_data(data):
    try:
        # Hacer algo con los datos recibidos
        print("Datos recibidos:", data)
    except Exception as e:
        print("Error al manejar datos recibidos:", e)


try:
    received_data = ""
    while True:
        if ser.in_waiting > 0:
            received_data += ser.read(ser.in_waiting).decode('utf-8')
        if "\n" in received_data:
            lines = received_data.split("\n")
            for line in lines[:-1]:
                print("Datos recibidos:", line)
            received_data = lines[-1]

except KeyboardInterrupt:
    pass

finally:
    ser.close()