# 🛡️ DuckyFence — RP2040-Based BadUSB Defense Board

**DuckyFence** is a custom RP2040-based development board designed to detect and log malicious USB devices (like BadUSB HID injectors). It acts as a hardware-level USB inspection layer, forwarding suspicious device data via UART for analysis using an Arduino + USB Host Shield or a PC. This project is built for hardware security researchers, makers, and defenders.

---

## 🔧 Features

- RP2040 (QFN-56) MCU with 16MB W25Q128JV QSPI Flash
- USB-A port for connecting suspicious USB devices
- USB-C port for power input and UART communication
- AMS1117-3.3V voltage regulator (powered from USB-C)
- UART header for connecting to Arduino or USB-Serial adapter
- GPIO breakout (GPIO2–9) for future expansion
- SWD header for debugging (SWCLK, SWDIO)
- BOOTSEL and RUN buttons
- Clean silkscreen with labeled headers and structured layout

---

## 🧱 Architecture

```plaintext
[ USB-A ] ← Suspicious USB device
     ↓
[ RP2040 ] → Logs descriptor/traffic
     ↓
[ UART TX/RX ] → Arduino Uno via USB Host Shield
     ↓
[ Python script or Serial Logger on PC ]
````

---

## 📦 Bill of Materials

### 🖥️ PCB BOM (LCSC Components)

* See [`BOM_PCB.xlsx`](./BOM_PCB.xlsx)
* Includes: RP2040 (`C2040`), Flash (`C84677`), AMS1117 (`C347222`), capacitors, resistors, USB ports, buttons, headers

### 🧰 Additional Hardware (Amazon)

See [`BOM_AMAZON.xlsx`](./BOM_AMAZON.xlsx)

| Item                                  | Qty   | Notes                               |
| ------------------------------------- | ----- | ----------------------------------- |
| Arduino Uno                           | 1     | Used as USB host + UART logger      |
| USB Host Shield                       | 1     | Needed to interface with USB-A port |
| USB-to-Serial Adapter | 1     | Optional — direct UART to PC        |
| USB-C 5V adapter                      | 1     | To power the board                  |
| Jumper wires                          | 1 set | Connect UART, GPIOs, or Arduino     |
| USB-A male cable                      | 1     | Connect BadUSB devices              |
| Breadboard (optional)                 | 1     | For prototyping GPIO uses           |

---

## 💻 Software Stack

| Platform                        | Use                                                  |
| ------------------------------- | ---------------------------------------------------- |
| **Arduino IDE**                 | Program Uno for USB descriptor reading               |
| **USB Host Shield Library 2.0** | Interact with USB devices                            |
| **Python + pyserial**           | Optional UART log parsing                            |
| **USBPcap (Optional)**          | Full USB traffic sniffing from PC side               |
| **RP2040 Firmware**             | Add TinyUSB or custom USB handlers (to be developed) |

---

## 🔌 Pinout Reference

| Function      | RP2040 Pin             | GPIO                                 | Notes                       |
| ------------- | ---------------------- | ------------------------------------ | --------------------------- |
| USB DP/DM     | Pins 34/33             | —                                    | Connected to USB-A          |
| UART TX/RX    | GPIO0/GPIO1            | TX → Header Pin 4, RX ← Header Pin 3 |                             |
| SWD           | Pins 24/25             | —                                    | SWDIO / SWCLK               |
| RUN           | Pin 30                 | —                                    | Reset pin                   |
| BOOTSEL       | Via GPIO + 1k + header |                                      |                             |
| GPIO Breakout | Pins 4–12              | GPIO2–9                              | 1×10 header with 3.3V + GND |

---

## 📑 Devlog

See [`DEVLOG.md`](./DEVLOG.md)

---

## 📸 Preview
![Top View](https://hc-cdn.hel1.your-objectstorage.com/s/v3/92b4674ab81d90fcc6c0f7a240fd5c8e7d33c3d9_image.png)
![Side View](https://hc-cdn.hel1.your-objectstorage.com/s/v3/513496f467b40c6f818115a2d973a3af67951d1e_image.png)
![Bottom View](https://hc-cdn.hel1.your-objectstorage.com/s/v3/fce8cc3f3c91b013d8dc43dced6fc8b10b9459c3_image.png)
---

## 🔧 Firmware

The `firmware/` folder contains all basic test and logging programs for the DuckyFence RP2040 board.

### Included Programs

| File                    | Description                                                           |
| ----------------------- | --------------------------------------------------------------------- |
| `blink.c`               | Blinks onboard LED (GPIO25) to verify power and boot                  |
| `uart_logger.c`         | Sends log messages via UART (GPIO0 TX → Arduino/PC)                   |
| `arduino_uart_logger.c` | Listens on UART for USB device logs sent by an Arduino Uno            |
| `usb_detect.c`          | Initializes TinyUSB (device mode); logs connection status via USB CDC |

### Requirements

* [Pico SDK](https://github.com/raspberrypi/pico-sdk) (CMake project)
* UART logs viewable via Arduino Serial Monitor or terminal
* Optional: USB-to-Serial adapter or Arduino Uno connected to UART header

### Build Instructions

```bash
git submodule update --init
mkdir build && cd build
cmake ..
make
```

Upload `.uf2` to the RP2040 using drag-and-drop bootloader or `picotool`.

---

## 👤 Author

Designed by [Devaansh Pathak](https://github.com/devaanshpathak)
Project Repo: [github.com/devaansh-pathak/duckyfence](https://github.com/devaanshpathak/duckyfence)