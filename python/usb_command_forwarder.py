import serial
import threading

PORT = 'COM4'     # Replace with your RP2040 USB CDC COM port
BAUD = 115200

ser = serial.Serial(PORT, BAUD, timeout=1)

def read_serial():
    while True:
        if ser.in_waiting:
            data = ser.readline().decode(errors='ignore').strip()
            if data:
                print(f"< {data}")

thread = threading.Thread(target=read_serial, daemon=True)
thread.start()

print("🛠️  DuckyFence CDC Shell — type 'exit' to quit.\n")

while True:
    try:
        cmd = input("> ")
        if cmd.lower() == "exit":
            break
        ser.write((cmd + "\n").encode())
    except KeyboardInterrupt:
        break

ser.close()
