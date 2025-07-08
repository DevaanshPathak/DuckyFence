# 🛠️ DEVLOG — DuckyFence: RP2040 BadUSB Defense Board

## ⏱️ Hours 1–2: Planning & Core Schematic

- Outlined goal: detect and log suspicious USB devices like HID injectors (BadUSB).
- Selected RP2040 (QFN-56), W25Q128JV flash, AMS1117-3.3 regulator, USB-A for input, and USB-C for power + UART.
- Designed core schematic in EasyEDA Pro:
  - RP2040 wiring
  - Flash, crystal, decoupling caps
  - Power input and voltage regulation
- Referenced Raspberry Pi’s hardware design guide to ensure proper pin use and grounding strategy.

---

## ⏱️ Hours 2–3: I/O, Debug & Final Schematic Touches

- Wired up BOOTSEL circuit using a 1kΩ pull-up + jumper.
- Added RUN reset button, SWD debug header, UART header, and test points for key pins.
- Connected USB-A D+/D− to RP2040 (GPIOs 24/25).
- Pulled down USB-C CC1/CC2 with 5.1k resistors for 5V power-only mode.
- Broke out GPIO2–GPIO9 via a 1×10 header with 3.3V and GND.
- Finalized schematic and cleaned net labels before layout.

---

## ⏱️ Hours 3–5: PCB Layout & Routing

- Started layout with RP2040 centered, flash + crystal nearby.
- Routed all power nets (3.3V, 1.1V, GND) and placed all decoupling/bulk caps close to their ICs.
- Routed D+ and D− traces from USB-A as a differential pair with length tuning.
- Added USB-C port for 5V in and UART TX/RX out.
- Cleaned up ground planes, filled zones, and ran preliminary DRC.

---

## ⏱️ Hours 5–6: Finishing Board Design

- Finalized placement of UART, GPIO, SWD, and BOOTSEL headers.
- Re-tuned differential pairs, matched lengths, and filled both layers with GND.
- Added silkscreen labels, logo, GitHub URL, and project name.
- Ran full DRC, resolved all spacing and clearance issues.
- Exported Gerbers, BOM, and pick-and-place files for manufacturing via JLCPCB.

---

## ⏱️ Hours 6–7: External BOM & Repo Setup

- Created external BOM (`BOM_AMAZON.xlsx`) with:
  - Arduino Uno
  - USB Host Shield
  - USB-to-Serial adapter
  - SD card module
  - Jumper wires and test gear
- Finalized folder structure:
  - `/firmware` for RP2040 programs
  - `/arduino` for USB Host sketches
  - `/docs` with schematic, BOMs, images
- Added `README.md`, feature list, architecture diagram, and pinout table.

---

## ⏱️ Hours 7–9: Firmware Programming

- ✅ `blink.c` — power check (GPIO25)
- ✅ `uart_logger.c` — send logs over UART
- ✅ `arduino_uart_logger.c` — receive and parse VID/PID over UART
- ✅ `usb_detect.c` — use TinyUSB to detect PC connection (USB CDC)
- ✅ `usb_filter.c` — filters known bad VID/PIDs and triggers an alert
- ✅ `usb_passthrough.c` — forwards UART data to USB (RP2040 as serial bridge)
- ✅ `gpio_pulse.c` — pulses a GPIO (LED/buzzer) on every detection event

All firmware compiled using the Pico SDK with TinyUSB support. Each was added to the `CMakeLists.txt` for easy building.

---

## ⏱️ Hours 9–11: Arduino USB Host Integration

- ✅ `usb_sniffer.ino` — basic VID/PID logger
- ✅ `usb_forward_uart.ino` — sends VID/PID over Serial1 to RP2040
- ✅ `usb_sniffer_plus.ino` — logs HID keypresses via USB Host Shield
- ✅ `usb_alarm.ino` — alerts when a known BadUSB is plugged in
- ✅ `usb_descriptor_dump.ino` — full USB descriptor logger
- ✅ `usb_class_logger.ino` — logs device class (HID, Mass Storage, etc.)
- ✅ `usb_sniff_to_sd.ino` — saves USB logs to SD card for offline review
- ✅ `hid_keystroke_relay.ino` — forwards raw keycodes from USB keyboard
- ✅ `badusb_detector.ino` — alerts if keypresses occur <1s after plug-in

Verified serial logs and alerts via both Arduino Serial Monitor and RP2040 UART receiver.

---

## ✅ Summary

- ⏱️ **Total time**: 11 hours
- 🧠 **All schematic + layout done manually**
- 🔌 **Full hardware integration with Arduino & RP2040**
- 📁 **Includes firmware, host-side Arduino tools, and structured docs**
- 📦 **Ready for JLCPCB fab with BOM + Pick & Place**
- 📸 **Project repo includes screenshots, schematic, Gerbers, and more**

---

Want to build your own or extend it?  
Check out the full repo: [github.com/devaanshpathak/duckyfence](https://github.com/devaanshpathak/duckyfence)
