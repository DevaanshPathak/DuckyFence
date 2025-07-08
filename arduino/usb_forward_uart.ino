#include <Usb.h>
#include <hidboot.h>
#include <usbhub.h>

USB     usb;
USBHub  hub(&usb);

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  delay(200);
  usb.Init();
}

void loop() {
  usb.Task();
  if (usb.getUsbTaskState() == USB_STATE_RUNNING) {
    for (uint8_t addr = 1; addr < USB_NUMDEVICES; addr++) {
      if (usb.getDevTableEntry(addr)) {
        USB_DEVICE_DESCRIPTOR desc;
        if (usb.getDevDescr(addr, sizeof(USB_DEVICE_DESCRIPTOR), (uint8_t*)&desc) == 0) {
          Serial1.print("VID: ");
          Serial1.print(desc.idVendor, HEX);
          Serial1.print(", PID: ");
          Serial1.println(desc.idProduct, HEX);
        }
      }
    }
    delay(3000);
  }
}
