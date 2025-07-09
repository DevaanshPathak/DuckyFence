import serial
import re
import platform

# Optional alert libraries
try:
    import winsound
except ImportError:
    winsound = None

try:
    from playsound import playsound
except ImportError:
    playsound = None

try:
    import pyttsx3
    tts = pyttsx3.init()
except ImportError:
    tts = None

from time import sleep

PORT = 'COM4'   # Change this to your serial port
BAUD = 115200

BAD_DEVICES = {
    "16D0:27DB": "Rubber Ducky",
    "1781:0C9F": "Malduino",
    "1A86:7523": "HID Clone"
}

pattern = r"VID:\s*([0-9A-Fa-f]+),\s*PID:\s*([0-9A-Fa-f]+)"
seen = set()

def alert(label):
    print(f"🚨 ALERT: {label}")
    if winsound:
        winsound.Beep(1000, 400)
    elif playsound:
        playsound("alert.wav")
    if tts:
        tts.say(f"Alert. {label} connected.")
        tts.runAndWait()

with serial.Serial(PORT, BAUD, timeout=1) as ser:
    print("Monitoring for BadUSB devices...")
    while True:
        line = ser.readline().decode(errors='ignore').strip()
        match = re.search(pattern, line)
        if match:
            vid, pid = match.groups()
            key = f"{vid.upper()}:{pid.upper()}"
            if key in BAD_DEVICES and key not in seen:
                alert(BAD_DEVICES[key])
                seen.add(key)
