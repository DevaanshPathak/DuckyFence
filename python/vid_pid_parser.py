import serial
import re

port = 'COM4'
baud = 115200

ser = serial.Serial(port, baud, timeout=1)
pattern = r"VID: (\w+), PID: (\w+)"

print("Waiting for USB events...")
while True:
    line = ser.readline().decode(errors='ignore').strip()
    match = re.search(pattern, line)
    if match:
        vid, pid = match.groups()
        print(f"🔎 Detected Device — VID: 0x{vid}, PID: 0x{pid}")
