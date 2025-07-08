#include <Usb.h>
#include <hidboot.h>
#include <usbhub.h>

USB     usb;
USBHub  hub(&usb);
HIDBoot<HID_PROTOCOL_KEYBOARD>    keyboard(&usb);

class KbdRptParser : public KeyboardReportParser {
  void OnKeyDown(uint8_t mod, uint8_t key) {
    Serial.print("[KEY] ");
    Serial.println(key, HEX);
  }
};

KbdRptParser kbdParser;

void setup() {
  Serial.begin(115200);
  Serial.println("usb_sniffer_plus.ino started");
  delay(200);
  if (usb.Init() == -1) {
    Serial.println("USB Host Shield init failed");
    while (1);
  }
  Serial.println("USB Host Shield initialized");
  keyboard.SetReportParser(0, &kbdParser);
}

void loop() {
  usb.Task();
  if (usb.getUsbTaskState() == USB_STATE_RUNNING) {
    for (uint8_t addr = 1; addr < USB_NUMDEVICES; addr++) {
      if (usb.getDevTableEntry(addr)) {
        USB_DEVICE_DESCRIPTOR desc;
        if (usb.getDevDescr(addr, sizeof(USB_DEVICE_DESCRIPTOR), (uint8_t*)&desc) == 0) {
          Serial.print("VID: ");
          Serial.print(desc.idVendor, HEX);
          Serial.print(", PID: ");
          Serial.println(desc.idProduct, HEX);
        }
      }
    }
    delay(3000);
  }
}
