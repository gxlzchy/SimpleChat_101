#include <CurieBLE.h>

BLEPeripheral textPeripheral;  // BLE Peripheral Device (the board you're programming)
BLEService textService("19B10010-E8F2-537E-4F6C-D104768A1214"); // BLE text Service

// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLECharacteristic textCharacteristic("19B10011-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite | BLENotify,1000);

void setup() {
  Serial.begin(9600);
  // set advertised local name and service UUID:
  textPeripheral.setLocalName("Chat");
  textPeripheral.setAdvertisedServiceUuid(textService.uuid());

  // add service and characteristic:
  textPeripheral.addAttribute(textService);
  textPeripheral.addAttribute(textCharacteristic);
  // begin advertising BLE service:
  textPeripheral.begin();

  Serial.println("BLE Text Peripheral");
}

void loop() {
  // listen for BLE peripherals to connect:
  BLECentral central = textPeripheral.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());
    short i;  //int i may have some problems.
    short cnt;
    // while the central is still connected to peripheral:
    while (central.connected()) {
      if (textCharacteristic.written()) {
        cnt=textCharacteristic.valueLength();
        for (i=0;i<cnt;i++) {Serial.write((char)textCharacteristic.value()[i]);}
        Serial.println();
      }
      unsigned char c[100];

      if  ( Serial.available() ){
       cnt=Serial.available();
       i=0;
       while (i<cnt) {c[i]=Serial.read();i++;}
       textCharacteristic.setValue(c, cnt);
       i=0;
       while (i<cnt) {Serial.write(c[i]);i++;}
       Serial.println();
     }
    }
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
    }
}

