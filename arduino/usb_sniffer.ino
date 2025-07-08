#include <Usb.h>
#include <hidboot.h>
#include <usbhub.h>

USB     usb;
USBHub  hub(&usb);
HIDBoot<HID_PROTOCOL_KEYBOARD>    keyboard(&usb);

void setup() {
  Serial.begin(115200);
  Serial.println("DuckyFence Arduino USB Host Starting...");

  if (usb.Init() == -1) {
    Serial.println("USB Host Shield init failed");
    while (1);
  }
  Serial.println("USB Host Shield init successful");
}

void loop() {
  usb.Task();

  if (usb.getUsbTaskState() == USB_STATE_RUNNING) {
    for (uint8_t addr = 1; addr < USB_NUMDEVICES; addr++) {
      if (usb.getDevTableEntry(addr)) {
        USB_DEVICE_DESCRIPTOR desc;
        if (usb.getDevDescr(addr, sizeof(USB_DEVICE_DESCRIPTOR), (uint8_t*)&desc) == 0) {
          Serial.print("Device Found - VID: ");
          Serial.print(desc.idVendor, HEX);
          Serial.print(", PID: ");
          Serial.println(desc.idProduct, HEX);

          // Optional: Send this to RP2040 via Serial1 or SoftwareSerial
        }
      }
    }

    delay(3000); // Prevent flooding output
  }
}
