import serial
from datetime import datetime

port = 'COM4'  # Change to your serial port
baud = 115200

with serial.Serial(port, baud, timeout=1) as ser, open("usb_log.txt", "a") as log:
    print("Logging started...")
    while True:
        line = ser.readline().decode(errors='ignore').strip()
        if line:
            timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
            log.write(f"[{timestamp}] {line}\n")
            print(f"[{timestamp}] {line}")
