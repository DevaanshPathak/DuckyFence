import serial
import csv
import time
import re
from datetime import datetime

PORT = 'COM4'      # Change this to your serial port
BAUD = 115200
SOURCE = "Arduino"  # Or "RP2040", depending on your setup
CSV_FILE = "usb_log.csv"

pattern = r"VID:\s*([0-9A-Fa-f]+),\s*PID:\s*([0-9A-Fa-f]+)"
logged = set()

with serial.Serial(PORT, BAUD, timeout=1) as ser, open(CSV_FILE, mode='a', newline='') as file:
    writer = csv.writer(file)
    if file.tell() == 0:
        writer.writerow(["Timestamp", "VID", "PID", "Class", "Source"])
    print(f"Logging to {CSV_FILE}...")

    while True:
        line = ser.readline().decode(errors='ignore').strip()
        match = re.search(pattern, line)
        if match:
            vid, pid = match.groups()
            timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
            device_class = "HID" if "HID" in line or int(pid, 16) in [0x27DB, 0x0C9F] else "Unknown"
            key = (vid, pid)
            if key not in logged:
                writer.writerow([timestamp, vid.upper(), pid.upper(), device_class, SOURCE])
                print(f"[{timestamp}] {vid}:{pid} → {device_class}")
                logged.add(key)
