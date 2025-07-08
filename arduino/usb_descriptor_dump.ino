#include <Usb.h>
#include <usbhub.h>

USB     usb;
USBHub  hub(&usb);

void setup() {
  Serial.begin(115200);
  delay(200);
  usb.Init();
}

void loop() {
  usb.Task();
  if (usb.getUsbTaskState() == USB_STATE_RUNNING) {
    for (uint8_t addr = 1; addr < USB_NUMDEVICES; addr++) {
      if (usb.getDevTableEntry(addr)) {
        USB_DEVICE_DESCRIPTOR d;
        if (usb.getDevDescr(addr, sizeof(d), (uint8_t*)&d) == 0) {
          Serial.println("== USB Device Descriptor ==");
          Serial.print("VID: "); Serial.println(d.idVendor, HEX);
          Serial.print("PID: "); Serial.println(d.idProduct, HEX);
          Serial.print("USB Version: "); Serial.print((d.bcdUSB >> 8) & 0xFF, HEX); Serial.print("."); Serial.println(d.bcdUSB & 0xFF, HEX);
          Serial.print("Device Class: "); Serial.println(d.bDeviceClass, HEX);
          Serial.print("Max Packet Size: "); Serial.println(d.bMaxPacketSize0);
          Serial.print("Manufacturer Index: "); Serial.println(d.iManufacturer);
          Serial.print("Product Index: "); Serial.println(d.iProduct);
          Serial.print("Serial Number Index: "); Serial.println(d.iSerialNumber);
          Serial.println("============================");
        }
      }
    }
    delay(5000);
  }
}
