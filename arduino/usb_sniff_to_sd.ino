#include <Usb.h>
#include <usbhub.h>
#include <SPI.h>
#include <SD.h>

USB     usb;
USBHub  hub(&usb);

File logFile;

void setup() {
  Serial.begin(115200);
  delay(200);
  usb.Init();
  SD.begin(10);
}

void loop() {
  usb.Task();
  if (usb.getUsbTaskState() == USB_STATE_RUNNING) {
    for (uint8_t addr = 1; addr < USB_NUMDEVICES; addr++) {
      if (usb.getDevTableEntry(addr)) {
        USB_DEVICE_DESCRIPTOR d;
        if (usb.getDevDescr(addr, sizeof(d), (uint8_t*)&d) == 0) {
          logFile = SD.open("log.csv", FILE_WRITE);
          if (logFile) {
            logFile.print("VID,");
            logFile.print(d.idVendor, HEX);
            logFile.print(",PID,");
            logFile.print(d.idProduct, HEX);
            logFile.print(",Class,");
            logFile.println(d.bDeviceClass, HEX);
            logFile.close();
          }
        }
      }
    }
    delay(5000);
  }
}
