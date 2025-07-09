import serial
import re

BAD_DEVICES = {
    "16D0:27DB": "Rubber Ducky",
    "1781:0C9F": "Malduino",
    "1A86:7523": "HID Clone"
}

port = 'COM4'
baud = 115200

ser = serial.Serial(port, baud, timeout=1)
pattern = r"VID: (\w+), PID: (\w+)"

while True:
    line = ser.readline().decode(errors='ignore').strip()
    match = re.search(pattern, line)
    if match:
        vid, pid = match.groups()
        key = f"{vid.upper()}:{pid.upper()}"
        if key in BAD_DEVICES:
            print(f"🚨 ALERT: {BAD_DEVICES[key]} detected! ({key})")
        else:
            print(f"✅ Safe Device: {key}")
