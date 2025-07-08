#include <Usb.h>
#include <usbhub.h>

USB     usb;
USBHub  hub(&usb);

const int alertPin = 7;

struct BadUSB {
  uint16_t vid;
  uint16_t pid;
};

BadUSB blacklist[] = {
  {0x16D0, 0x27DB},
  {0x1781, 0x0C9F},
  {0x1A86, 0x7523},
  {0x2341, 0x8036}
};

const int blacklist_len = sizeof(blacklist) / sizeof(blacklist[0]);

void setup() {
  Serial.begin(115200);
  pinMode(alertPin, OUTPUT);
  digitalWrite(alertPin, LOW);
  delay(200);
  usb.Init();
}

void loop() {
  usb.Task();
  if (usb.getUsbTaskState() == USB_STATE_RUNNING) {
    for (uint8_t addr = 1; addr < USB_NUMDEVICES; addr++) {
      if (usb.getDevTableEntry(addr)) {
        USB_DEVICE_DESCRIPTOR desc;
        if (usb.getDevDescr(addr, sizeof(desc), (uint8_t*)&desc) == 0) {
          bool alert = false;
          for (int i = 0; i < blacklist_len; i++) {
            if (desc.idVendor == blacklist[i].vid && desc.idProduct == blacklist[i].pid) {
              alert = true;
              break;
            }
          }
          if (alert) {
            Serial.println("⚠️ ALERT: Blocked USB Device Detected!");
            digitalWrite(alertPin, HIGH);
          } else {
            Serial.println("USB device allowed.");
            digitalWrite(alertPin, LOW);
          }
        }
      }
    }
    delay(3000);
  }
}
