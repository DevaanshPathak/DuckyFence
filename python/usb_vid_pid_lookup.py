import sys
import os
import requests

USB_IDS_URL = "http://www.linux-usb.org/usb.ids"
USB_IDS_FILE = "usb.ids"

def download_usb_ids():
    print("🔄 Downloading latest usb.ids...")
    response = requests.get(USB_IDS_URL)
    if response.status_code == 200:
        with open(USB_IDS_FILE, "w", encoding="utf-8") as f:
            f.write(response.text)
        print("✅ usb.ids updated.")
    else:
        print("❌ Failed to download usb.ids.")

def load_usb_ids():
    if not os.path.exists(USB_IDS_FILE):
        download_usb_ids()

    vid_map = {}
    current_vid = None
    with open(USB_IDS_FILE, "r", encoding="utf-8") as f:
        for line in f:
            if line.startswith("#") or not line.strip():
                continue
            if not line.startswith("\t"):
                parts = line.strip().split(None, 1)
                if len(parts) == 2:
                    current_vid = parts[0].upper()
                    vid_map[current_vid] = {"name": parts[1], "products": {}}
            else:
                parts = line.strip().split(None, 1)
                if len(parts) == 2 and current_vid:
                    pid = parts[0].upper()
                    name = parts[1]
                    vid_map[current_vid]["products"][pid] = name
    return vid_map

def lookup(vid, pid, vid_map):
    vid = vid.upper()
    pid = pid.upper()
    vendor = vid_map.get(vid)
    if not vendor:
        print(f"❌ VID {vid} not found.")
        return
    product = vendor["products"].get(pid, "Unknown Product")
    print(f"🔍 Vendor: {vendor['name']}")
    print(f"🔍 Product: {product}")

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python usb_vid_pid_lookup.py <VID> <PID>")
        sys.exit(1)

    vid, pid = sys.argv[1], sys.argv[2]
    vid_pid_map = load_usb_ids()
    lookup(vid, pid, vid_pid_map)
