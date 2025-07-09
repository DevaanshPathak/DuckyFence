import tkinter as tk
from tkinter import ttk, filedialog
import serial
import threading
import re
from datetime import datetime
import csv

PORT = 'COM4'
BAUD = 115200

BADUSB_LIST = {
    "16D0:27DB": "Rubber Ducky",
    "1781:0C9F": "Malduino",
    "1A86:7523": "HID Clone"
}

pattern = r"VID:\s*([0-9A-Fa-f]+),\s*PID:\s*([0-9A-Fa-f]+)"
log_entries = []

class USBLoggerGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("🛡️ DuckyFence USB Logger")
        self.root.geometry("720x500")

        # Header label
        header = tk.Label(root, text="DuckyFence — USB Detection Monitor", font=("Segoe UI", 14, "bold"), pady=10)
        header.pack()

        # Log view table
        self.tree = ttk.Treeview(root, columns=("Timestamp", "VID", "PID", "Class", "Label"), show="headings", height=15)
        for col in self.tree["columns"]:
            self.tree.heading(col, text=col)
            self.tree.column(col, anchor="center", width=120)
        self.tree.pack(fill=tk.BOTH, expand=True, padx=12, pady=6)

        # Buttons
        btn_frame = tk.Frame(root)
        btn_frame.pack(pady=5)

        ttk.Button(btn_frame, text="📁 Export CSV", command=self.export_csv).pack(side=tk.LEFT, padx=8)
        ttk.Button(btn_frame, text="🧹 Clear Log", command=self.clear_log).pack(side=tk.LEFT, padx=8)
        ttk.Button(btn_frame, text="🔄 Reconnect", command=self.reconnect).pack(side=tk.LEFT, padx=8)

        # Status bar
        self.status_var = tk.StringVar()
        self.status = tk.Label(root, textvariable=self.status_var, bd=1, relief=tk.SUNKEN, anchor=tk.W, padx=6, font=("Segoe UI", 9))
        self.status.pack(fill=tk.X, side=tk.BOTTOM)

        self.ser = None
        self.running = True
        self.thread = threading.Thread(target=self.serial_read_thread, daemon=True)
        self.thread.start()

    def add_log(self, vid, pid):
        ts = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        key = f"{vid}:{pid}"
        label = BADUSB_LIST.get(key, "Unknown")
        dev_class = "HID" if "HID" in label or pid in ["27DB", "0C9F"] else "Unknown"
        color = "gray"

        if label in ["Rubber Ducky", "Malduino", "HID Clone"]:
            tag = "badusb"
            color = "red"
        elif label == "Unknown":
            tag = "unknown"
        else:
            tag = "safe"
            color = "green"

        row = (ts, vid, pid, dev_class, label)
        self.tree.insert("", "end", values=row, tags=(tag,))
        self.tree.tag_configure("badusb", background="#FFCDD2", foreground="black")
        self.tree.tag_configure("safe", background="#C8E6C9", foreground="black")
        self.tree.tag_configure("unknown", background="#ECEFF1", foreground="black")

        self.tree.yview_moveto(1.0)  # Auto-scroll
        log_entries.append(row)
        self.status_var.set(f"Connected | Entries: {len(log_entries)}")

    def serial_read_thread(self):
        try:
            self.ser = serial.Serial(PORT, BAUD, timeout=1)
            self.status_var.set(f"Connected to {PORT} at {BAUD} baud.")
            while self.running:
                line = self.ser.readline().decode(errors='ignore').strip()
                match = re.search(pattern, line)
                if match:
                    vid, pid = match.groups()
                    self.add_log(vid.upper(), pid.upper())
        except Exception as e:
            self.status_var.set(f"❌ Serial Error: {e}")

    def export_csv(self):
        file = filedialog.asksaveasfilename(defaultextension=".csv")
        if not file:
            return
        with open(file, "w", newline='') as f:
            writer = csv.writer(f)
            writer.writerow(["Timestamp", "VID", "PID", "Class", "Label"])
            for row in log_entries:
                writer.writerow(row)
        self.status_var.set(f"✔️ Exported log to {file}")

    def clear_log(self):
        self.tree.delete(*self.tree.get_children())
        log_entries.clear()
        self.status_var.set("🧹 Log cleared.")

    def reconnect(self):
        self.running = False
        if self.ser:
            self.ser.close()
        self.running = True
        self.thread = threading.Thread(target=self.serial_read_thread, daemon=True)
        self.thread.start()
        self.status_var.set("🔄 Reconnecting...")

if __name__ == "__main__":
    root = tk.Tk()
    app = USBLoggerGUI(root)
    root.protocol("WM_DELETE_WINDOW", lambda: (setattr(app, 'running', False), root.destroy()))
    root.mainloop()
