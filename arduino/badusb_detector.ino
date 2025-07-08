#include <Usb.h>
#include <hidboot.h>
#include <usbhub.h>

USB     usb;
USBHub  hub(&usb);
HIDBoot<HID_PROTOCOL_KEYBOARD> keyboard(&usb);

unsigned long attachTime = 0;
bool deviceConnected = false;
bool alertTriggered = false;

class KbdRptParser : public KeyboardReportParser {
  void OnKeyDown(uint8_t mod, uint8_t key) {
    if (!alertTriggered && millis() - attachTime < 1000) {
      Serial.println("⚠️ ALERT: HID typed immediately after connect (BadUSB?)");
      alertTriggered = true;
    } else {
      Serial.print("KEY: ");
      Serial.println(key, HEX);
    }
  }
};

KbdRptParser parser;

void setup() {
  Serial.begin(115200);
  delay(200);
  usb.Init();
  keyboard.SetReportParser(0, &parser);
}

void loop() {
  usb.Task();
  if (!deviceConnected && usb.getUsbTaskState() == USB_STATE_RUNNING) {
    deviceConnected = true;
    attachTime = millis();
    alertTriggered = false;
    Serial.println("USB HID device connected.");
  }

  if (deviceConnected && usb.getUsbTaskState() != USB_STATE_RUNNING) {
    deviceConnected = false;
    Serial.println("Device disconnected.");
  }
}
