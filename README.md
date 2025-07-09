# 🛡️ DuckyFence — RP2040-Based BadUSB Defense Board

**DuckyFence** is a custom-built USB defense board powered by the RP2040. It helps detect and log suspicious USB devices — especially ones that act like keyboards (like Rubber Ducky or other BadUSB-style HID injectors). This board sits between the attacker and your system, giving you real-time visibility into USB activity before it’s too late.

It’s built for makers, researchers, and defenders who want a simple, low-cost, and reliable way to catch malicious USB behavior.

---

## 🔧 Features

- RP2040 (QFN-56) microcontroller with 16MB QSPI Flash
- USB-A port to plug in potentially malicious devices
- USB-C port for 5V input + UART communication
- AMS1117-3.3V regulator for stable onboard power
- UART header for connecting to Arduino or USB-Serial adapter
- Breakout for GPIO2–9 to use for LEDs, buzzers, or expansion
- SWD header for debugging with external programmer
- BOOTSEL jumper and a physical RUN button
- Minimal, well-labeled design with clean silkscreen layout

---

## 🧱 How It Works

```plaintext
[ USB-A ] ← Suspicious USB device
     ↓
[ RP2040 ] — Parses descriptors or monitors keypresses
     ↓
[ UART Header ] — Connects to Arduino or USB-to-Serial
     ↓
[ Host Computer ] — Displays or logs data using Python tools
````

The board inspects what's plugged into the USB-A port and passes logs via UART to your Arduino or PC for alerting and analysis.

---

## 📦 Bill of Materials

### 🖥️ Core PCB Parts (LCSC)

See [`BOM_PCB.xlsx`](./BOM_PCB.xlsx)
Includes:

* RP2040 (`C2040`)
* Flash (`C84677`)
* AMS1117 (`C347222`)
* USB-A and USB-C ports
* Passive parts, buttons, headers

### 🧰 Additional Hardware (Amazon)

See [`BOM_AMAZON.xlsx`](./BOM_AMAZON.xlsx)

| Item                  | Qty | Why You Need It                   |
| --------------------- | --- | --------------------------------- |
| Arduino Uno           | 1   | Logs USB info via USB Host Shield |
| USB Host Shield       | 1   | Reads device descriptors          |
| USB-to-Serial Adapter | 1   | Optional — direct UART to PC      |
| USB-C Power Adapter   | 1   | Powers the board                  |
| Jumper Wires          | 1   | For UART, GPIO, or expansion      |
| USB-A Cable           | 1   | Plug in test devices or payloads  |
| Breadboard (optional) | 1   | For prototyping with GPIOs        |

---

## 💻 Software Overview

| Tool/Platform           | What It's For                     |
| ----------------------- | --------------------------------- |
| Arduino IDE             | Write/Upload USB Host Shield code |
| USB Host Shield Library | Detect USB devices (on Arduino)   |
| Python + pyserial       | Parse logs or alert via UART      |
| USBPcap (optional)      | Sniff USB traffic from PC         |
| Pico SDK (CMake)        | Build firmware for RP2040         |

---

## 🔌 Key Pins & Headers

| Purpose       | RP2040 Pin    | GPIO    | Notes                     |
| ------------- | ------------- | ------- | ------------------------- |
| USB D+/D−     | Pins 34 / 33  | —       | Connected to USB-A port   |
| UART TX / RX  | GPIO0 / GPIO1 | TX/RX   | Connected to header       |
| SWD Debug     | Pins 24 / 25  | —       | For SWCLK and SWDIO       |
| RUN           | Pin 30        | —       | Physical reset button     |
| BOOTSEL       | —             | —       | Jumper with 1kΩ pullup    |
| GPIO Breakout | Pins 4–12     | GPIO2–9 | 1×10 header for expansion |

---

## 🔧 Firmware

The [`firmware/`](./firmware) folder contains all C-based programs that run on the RP2040.

### Included Programs

| File                    | Description                                  |
| ----------------------- | -------------------------------------------- |
| `blink.c`               | Blink onboard LED (for power/debug check)    |
| `uart_logger.c`         | Print logs over UART to PC or Arduino        |
| `arduino_uart_logger.c` | Listen for USB logs sent from Arduino        |
| `usb_detect.c`          | Use TinyUSB to check if PC is connected      |
| `usb_filter.c`          | Match known bad VID/PIDs, trigger alerts     |
| `usb_passthrough.c`     | Forward UART to USB CDC (serial bridge mode) |
| `gpio_pulse.c`          | Pulse GPIO for every new device event        |

### How to Build It

Requires the [Pico SDK](https://github.com/raspberrypi/pico-sdk)

```bash
git submodule update --init
mkdir build && cd build
cmake ..
make
```

Flash `.uf2` files by dropping them onto the RP2040 in BOOTSEL mode.

---

## 🤖 Arduino Sketches

Found in [`arduino/`](./arduino), these run on the Uno + USB Host Shield.

| Sketch                    | Purpose                                       |
| ------------------------- | --------------------------------------------- |
| `usb_sniffer.ino`         | Logs VID/PID when device is plugged in        |
| `usb_forward_uart.ino`    | Forwards USB info over Serial1 to RP2040      |
| `usb_sniff_to_sd.ino`     | Saves USB logs to SD card                     |
| `usb_alarm.ino`           | Triggers alert (buzzer/LED) on bad devices    |
| `usb_descriptor_dump.ino` | Dumps full USB descriptor info                |
| `hid_keystroke_relay.ino` | Logs HID keystrokes sent from suspicious USB  |
| `usb_class_logger.ino`    | Prints USB class (HID, Mass Storage, etc.)    |
| `badusb_detector.ino`     | Flags HID devices that start typing instantly |

---

## 🐍 Python Tools

Check the [`python/`](./python) folder for host-side tools:

* `log_serial.py` — Save serial output to `usb_log.txt`
* `alert_on_badusb.py` — Warn when BadUSB signatures are found
* `usb_log_to_csv.py` — Save clean CSV of USB events
* `usb_json_logger.py` — Log events as newline-delimited JSON
* `usb_audio_alert.py` — Beep or speak when a device matches the blocklist
* `usb_command_forwarder.py` — Send simple shell commands to RP2040 via CDC
* `usb_badusb_logger_gui.py` — GUI log viewer with alerts and CSV export
* `usb_diff_log.py` — Compare two logs to detect spoofed or new USB devices
* `usb_vid_pid_lookup.py` — Look up vendor/product from VID/PID using `usb.ids`

All scripts are cross-platform and use `pyserial`. Some use `tkinter`, `playsound`, or `pyttsx3` (optional).

---

Absolutely! Here's a clean, concise, and human-friendly **NeoPixel section** for your `README.md`. You can paste this right under your **Python Tools** section:

---

## 🌈 NeoPixel Status Indicator

DuckyFence includes support for a **single WS2812 NeoPixel RGB LED** as a visual status indicator. It gives instant feedback about USB device activity — even without a serial monitor.

### 🎯 Behavior

| Color     | Meaning                            |
| --------- | ---------------------------------- |
| 🟡 Yellow | No USB device connected            |
| 🟢 Green  | Safe device detected (not flagged) |
| 🔴 Red    | BadUSB or known malicious VID/PID  |

### ⚙️ Setup

* **GPIO3** is used to control the NeoPixel
* Connect VCC to **3.3V**, GND to **GND**, and **DIN** to GPIO3
* Add a **330Ω resistor** in series with DIN
* Add a **100 µF capacitor** across VCC–GND for stability

### 📁 Firmware File

The NeoPixel logic lives in:

* `firmware/usb_neopixel_status.c`
* Uses PIO + `ws2812.pio` for smooth color control
* Integrated with `usb_filter.c` to reflect detection events in real time

This makes the board fully usable even without any UART or PC connection — just power it up, plug in a USB device, and watch the LED show the result.

---


## 📝 Devlog

Read [`DEVLOG.md`](./DEVLOG.md) for the full build journal.

Covers:

* Day-by-day and hour-by-hour progress
* Schematic and layout development
* Firmware milestones
* Host-side tools
* BOM breakdown and PCB export

---

## 🖼️ Project Preview

![Top View](https://hc-cdn.hel1.your-objectstorage.com/s/v3/92b4674ab81d90fcc6c0f7a240fd5c8e7d33c3d9_image.png)
![Side View](https://hc-cdn.hel1.your-objectstorage.com/s/v3/513496f467b40c6f818115a2d973a3af67951d1e_image.png)
![Bottom View](https://hc-cdn.hel1.your-objectstorage.com/s/v3/fce8cc3f3c91b013d8dc43dced6fc8b10b9459c3_image.png)

---

## 👤 Made with love by :

**[Devaansh Pathak](https://github.com/devaanshpathak)**
GitHub Repo: [github.com/devaanshpathak/duckyfence](https://github.com/devaanshpathak/duckyfence)

---

⚙️ Want to build your own or contribute ideas? Pull requests welcome!
