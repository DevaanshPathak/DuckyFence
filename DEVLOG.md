# 🛠️ DEVLOG — DuckyFence: RP2040 BadUSB Defense Board

## Hours 1-2

Started the project by outlining its scope: a hardware-based defense board that detects malicious USB devices like HID injectors (BadUSB). Decided on RP2040 as the MCU for its dual-core processing and USB support. Selected the core components including the RP2040, W25Q128JV QSPI flash, AMS1117 regulator, and USB-A/C connectors. Created a rough block diagram and began building the schematic in EasyEDA Pro — focusing first on the power section and RP2040 core wiring. Spent time reviewing the hardware design guide from Raspberry Pi to ensure correct pin usage and decoupling strategies.

---

## Hours 2-3

Completed the schematic wiring for the BOOTSEL circuit, RUN pin, UART header, and SWD debug interface. Added decoupling capacitors for each IOVDD, DVDD, USBVDD, and flash VCC. Finalized the AMS1117 section with appropriate input/output bulk capacitors and verified pin mapping. Chose a minimal USB-C footprint (7-pin) for power-only use and ensured CC1/CC2 were pulled down with 5.1 kΩ resistors. Connected USB-A D+/D− lines to the RP2040's native USB pins and added a power-only USB-C interface to serve as UART output to Arduino. Reviewed the schematic against the official RP2040 datasheet and cleaned up net labels.

---

## Hours 3-5

Converted the schematic to PCB layout and began placing critical components. Positioned the RP2040 at the center, with the flash and crystal placed tightly nearby for optimal signal integrity. Routed the AMS1117 power input from USB-C and output to all 3.3 V power pins, while placing decoupling capacitors as close to the IC as possible. Routed USB-A D+/D− traces as a differential pair, matching lengths and spacing to avoid impedance mismatches. Filled both top and bottom layers with a solid GND plane. Placed the GPIO breakout header and added test points for key signals like D+/D−, VBUS, and RUN.

---

## Hours 5-6

Finalized placement of UART and debug headers. Re-routed a few power traces for improved clarity and safety, and added copper fills for cleaner EMI performance. Created a custom back silkscreen layout with board name, author, GitHub URL, and a fun slogan. Tuned the differential USB traces to be length-matched using EasyEDA Pro's trace tuning tools. Ran DRC (Design Rule Check) and resolved spacing, clearance, and unconnected net warnings. Exported the final Gerbers, BOM, and Pick & Place files for manufacturing through JLCPCB.

---

## Hours 6-8

Compiled a complete external BOM for Amazon parts, including the Arduino Uno, USB Host Shield, USB-to-Serial adapter, and jumper wires. Finalized the README with full feature list, BOMs, use case diagram, and pinout table. Created this devlog summarizing each day’s work. Organized the GitHub repository with schematic PDFs, BOM files, screenshots, and Gerbers.

---

## Hours 8-11

Worked on firmware development for basic testing and communication. Wrote `blink.c` to verify power and GPIO control, followed by `uart_logger.c` for UART transmission to Arduino. Created `arduino_uart_logger.c` to parse USB descriptor logs from Arduino via serial. Set up `usb_detect.c` using TinyUSB to verify USB device mount states. Added all firmware files, a Pico SDK `CMakeLists.txt`, and usage instructions to the repo. Began Arduino integration by setting up the USB Host Shield sketch to detect and print VID/PID of connected USB devices (BadUSB). Verified serial logs were correctly received on both Arduino Serial Monitor and RP2040 UART.