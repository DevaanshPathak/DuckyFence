# 🛡️ DuckyFence — RP2040-Based BadUSB Defense Board

**DuckyFence** is a custom RP2040-based board built to detect and log suspicious USB devices — especially ones that act like keyboards (HID injectors), like the infamous BadUSB. It works as a hardware-level USB inspection tool. Connect it to an Arduino + USB Host Shield (or a serial adapter), and it’ll help you spot and track rogue USB activity in real time.

---

## 🔧 Features

- RP2040 (QFN-56) with 16MB QSPI Flash (W25Q128JV)
- USB-A port to plug in suspect devices
- USB-C port for 5V power and UART out
- AMS1117-3.3V voltage regulator (USB-powered)
- UART header for Arduino or USB-to-Serial adapter
- GPIO breakout (GPIO2–9) for extensions
- SWD debug header (SWCLK + SWDIO)
- BOOTSEL jumper and RUN button
- Clean silkscreen and compact layout

---

## 🧱 Architecture

```plaintext
[ USB-A ] ← Suspect USB Device
     ↓
[ RP2040 ] → Forwards device info
     ↓
[ UART TX/RX ] → Arduino Uno (USB Host Shield)
     ↓
[ Serial Monitor or Python Parser on PC ]
````

---

## 📦 BOM (Bill of Materials)

### 🖥️ PCB Parts (LCSC)

* See [`BOM_PCB.xlsx`](./BOM_PCB.xlsx)
* Includes: RP2040 (`C2040`), W25Q128JV Flash (`C84677`), AMS1117 (`C347222`), USB ports, passives, headers, buttons

### 🧰 Extra Hardware (Amazon)

See [`BOM_AMAZON.xlsx`](./BOM_AMAZON.xlsx)

| Item                  | Qty | Use                                |
| --------------------- | --- | ---------------------------------- |
| Arduino Uno           | 1   | Logs USB data via USB Host Shield  |
| USB Host Shield       | 1   | Lets Arduino read devices on USB-A |
| USB-to-Serial Adapter | 1   | Optional – direct UART to PC       |
| USB-C Power Adapter   | 1   | Powers the board                   |
| Jumper Wires          | 1   | For UART or GPIOs                  |
| USB-A Cable           | 1   | Connects BadUSB devices            |
| Breadboard (optional) | 1   | For testing GPIO breakouts         |

---

## 💻 Software Setup

| Tool/Platform           | Use                            |
| ----------------------- | ------------------------------ |
| Arduino IDE             | Program Arduino Uno            |
| USB Host Shield Library | USB device detection (Arduino) |
| Python + pyserial       | Optional: read UART on PC      |
| USBPcap (Optional)      | Log USB traffic from PC        |
| Pico SDK (CMake)        | Build firmware for RP2040      |

---

## 🔌 Pinout (Important Pins)

| Function      | RP2040 Pin    | GPIO    | Notes                       |
| ------------- | ------------- | ------- | --------------------------- |
| USB D+/D−     | Pins 34 / 33  | —       | Connected to USB-A          |
| UART TX/RX    | GPIO0 / GPIO1 | TX/RX   | UART header → Arduino / PC  |
| SWD Debug     | Pins 24 / 25  | —       | For SWCLK and SWDIO         |
| RUN           | Pin 30        | —       | Reset pin                   |
| BOOTSEL       | Header + 1kΩ  | —       | For programming             |
| GPIO Breakout | Pins 4–12     | GPIO2–9 | On a 1×10 header with power |

---

## 🔧 Firmware

The [`firmware/`](./firmware) folder includes all core test programs and UART loggers for the RP2040.

### What’s Inside

| File                    | What it Does                                        |
| ----------------------- | --------------------------------------------------- |
| `blink.c`               | Blinks onboard LED to confirm boot                  |
| `uart_logger.c`         | Sends log messages over UART                        |
| `arduino_uart_logger.c` | Listens for logs from Arduino (USB device info)     |
| `usb_detect.c`          | Uses TinyUSB to detect host connection via USB CDC  |
| `usb_filter.c`          | Flags known malicious VID/PIDs, triggers GPIO alert |
| `usb_passthrough.c`     | Forwards UART to PC via USB CDC (serial bridge)     |
| `gpio_pulse.c`          | Pulses a GPIO on every USB device detection         |

### How to Build

Make sure you have the [Pico SDK](https://github.com/raspberrypi/pico-sdk) set up:

```bash
git submodule update --init
mkdir build && cd build
cmake ..
make
```

You’ll get `.uf2` files in `build/` which can be dragged to the RP2040 USB drive.

---

## 🔌 Arduino Code

All Arduino sketches are in [`arduino/`](./arduino). These run on the Uno + USB Host Shield and include:

* `usb_sniffer.ino` – Basic VID/PID logger
* `usb_forward_uart.ino` – Forwards USB data to RP2040 over UART
* `usb_sniff_to_sd.ino` – Logs USB data to SD card
* `usb_alarm.ino` – Triggers GPIO pin when a known bad device is detected
* `usb_descriptor_dump.ino` – Dumps full USB descriptor
* `hid_keystroke_relay.ino` – Logs keycodes from HID keyboards
* `usb_class_logger.ino` – Shows the USB class for each device
* `badusb_detector.ino` – Detects if a device starts typing instantly (BadUSB)

---

## 📑 Devlog

Check out the full build log in [`DEVLOG.md`](./DEVLOG.md).
Includes day-wise updates, hours spent, firmware progress, screenshots, and submission checklist.

---

## 🖼️ Preview

![Top View](https://hc-cdn.hel1.your-objectstorage.com/s/v3/92b4674ab81d90fcc6c0f7a240fd5c8e7d33c3d9_image.png)
![Side View](https://hc-cdn.hel1.your-objectstorage.com/s/v3/513496f467b40c6f818115a2d973a3af67951d1e_image.png)
![Bottom View](https://hc-cdn.hel1.your-objectstorage.com/s/v3/fce8cc3f3c91b013d8dc43dced6fc8b10b9459c3_image.png)

---

## 👤 Made by

[Devaansh Pathak](https://github.com/devaanshpathak)
GitHub Repo: [github.com/devaanshpathak/duckyfence](https://github.com/devaanshpathak/duckyfence)

---

Want to build one yourself or contribute ideas? Pull requests welcome!
