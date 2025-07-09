import serial
import json
import re
from datetime import datetime

PORT = 'COM4'      # Change this to your serial port
BAUD = 115200
JSON_FILE = "usb_log.jsonl"

BAD_DEVICES = {
    "16D0:27DB": "Rubber Ducky",
    "1781:0C9F": "Malduino",
    "1A86:7523": "HID Clone",
    "2341:8036": "Uno Clone"
}

pattern = r"VID:\s*([0-9A-Fa-f]+),\s*PID:\s*([0-9A-Fa-f]+)"

with serial.Serial(PORT, BAUD, timeout=1) as ser, open(JSON_FILE, "a") as outfile:
    print(f"Logging USB events to {JSON_FILE}...")
    while True:
        line = ser.readline().decode(errors='ignore').strip()
        match = re.search(pattern, line)
        if match:
            vid, pid = match.groups()
            key = f"{vid.upper()}:{pid.upper()}"
            label = BAD_DEVICES.get(key, "Unknown Device")
            log = {
                "timestamp": datetime.now().isoformat(),
                "vid": vid.upper(),
                "pid": pid.upper(),
                "label": label
            }
            json.dump(log, outfile)
            outfile.write('\n')
            print(f"🟢 Logged: {label} ({key})")
