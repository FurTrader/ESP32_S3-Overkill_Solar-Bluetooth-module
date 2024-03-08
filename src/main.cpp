#include "Arduino.h"
#include <ArduinoBLE.h>
#include <esp_log_internal.h>
#include <HardwareSerial.h>
#include "esp_mac.h"

#define UART_RX_PIN 40
#define UART_TX_PIN 41

BLEService uartService("0000ff00-0000-1000-8000-00805f9b34fb");
BLECharacteristic readCharacteristic("0000ff01-0000-1000-8000-00805f9b34fb", BLERead | BLENotify, 512);
BLECharacteristic writeCharacteristic("0000ff02-0000-1000-8000-00805f9b34fb", BLEWriteWithoutResponse | BLENotify, 512);

HardwareSerial uart(2);

void onCharacteristicNotify(BLEDevice device, BLECharacteristic characteristic) {

  int len = writeCharacteristic.valueLength();
  const uint8_t *value = writeCharacteristic.value();

  uart.write(value, len);
  esp_log_buffer_hex_internal("BLE -> ", value, len, ESP_LOG_INFO);

}

void setup() {

  Serial.begin(115200);
  Serial.setDebugOutput(true);
  esp_log_level_set("*", ESP_LOG_INFO);

  while (!Serial);
  
  log_e("setup");

  uart.begin(9600, SERIAL_8N1 , UART_RX_PIN, UART_TX_PIN);

  if (!BLE.begin()) {
    log_e("starting Bluetooth® Low Energy module failed!");
    while (1);
  }

  // set advertised local name and service UUID:
  BLE.setLocalName("OKS-CAN-BLE");
  BLE.setAdvertisedService(uartService);

  uint8_t mac_address[6];
  esp_efuse_mac_get_default(mac_address);

  BLE.setManufacturerData(mac_address, 6);
  
  esp_log_buffer_hex_internal("MAC -> ", mac_address, 6, ESP_LOG_INFO);

  uartService.addCharacteristic(writeCharacteristic);
  uartService.addCharacteristic(readCharacteristic);

  // add service
  BLE.addService(uartService);

  // set the initial value for the characeristic:
  writeCharacteristic.writeValue(0);
  readCharacteristic.writeValue(0);

  writeCharacteristic.setEventHandler(BLEWritten, onCharacteristicNotify);

  BLE.advertise();

  log_i("BLE UART Peripheral");
}

void loop() {
  // listen for Bluetooth® Low Energy peripherals to connect:
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {
    log_i("Connected to central: %s", central.address().c_str());

    // while the central is still connected to peripheral:
    while (central.connected()) {

      uint8_t buf[512];
      int p = 0;

      while(uart.available()){
        buf[p++] = uart.read();
      }

      if(p){
        readCharacteristic.setValue(buf, p);
        esp_log_buffer_hex_internal("BMS -> ", buf, p, ESP_LOG_INFO);
      }

      vTaskDelay(pdMS_TO_TICKS(10));
    }

    log_i("Disconnected from central: %s", central.address());
  }
}