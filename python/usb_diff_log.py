import csv
from datetime import datetime
from collections import defaultdict

LOG1 = "usb_log1.csv"  # Older snapshot
LOG2 = "usb_log2.csv"  # Newer/current snapshot

def parse_log(filename):
    log = {}
    with open(filename, newline='') as f:
        reader = csv.DictReader(f)
        for row in reader:
            key = f"{row['VID']}:{row['PID']}"
            timestamp = row['Timestamp']
            log[key] = timestamp
    return log

log1 = parse_log(LOG1)
log2 = parse_log(LOG2)

set1 = set(log1.keys())
set2 = set(log2.keys())

new_devices = set2 - set1
removed_devices = set1 - set2
unchanged = set1 & set2

print("=== USB Diff Report ===\n")

if new_devices:
    print("🆕 New Devices Detected:")
    for dev in new_devices:
        print(f"  + {dev} at {log2[dev]}")
else:
    print("✅ No new devices.\n")

if removed_devices:
    print("\n❌ Devices Removed Since Last Log:")
    for dev in removed_devices:
        print(f"  - {dev} last seen at {log1[dev]}")
else:
    print("\n✅ No removed devices.\n")

print(f"\n📊 Total Devices:")
print(f"  Log 1: {len(log1)}")
print(f"  Log 2: {len(log2)}")
print(f"  Shared: {len(unchanged)}")
