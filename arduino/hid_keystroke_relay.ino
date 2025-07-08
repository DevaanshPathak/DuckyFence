#include <Usb.h>
#include <hidboot.h>
#include <usbhub.h>

USB     usb;
USBHub  hub(&usb);
HIDBoot<HID_PROTOCOL_KEYBOARD>    keyboard(&usb);

class KbdRptParser : public KeyboardReportParser {
  void OnKeyDown(uint8_t mod, uint8_t key) {
    Serial1.print("KEY:");
    Serial1.println(key, HEX);
  }
};

KbdRptParser parser;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  delay(200);
  usb.Init();
  keyboard.SetReportParser(0, &parser);
}

void loop() {
  usb.Task();
}
