# Son-of-Nomad
ESP32 Dongle for JBD BMSs

Phase 1: Emulate the original JBD BLE dongle.

To Do:
 - USB to UART bridge
 - OLED display
 - CANBUS
   - This is the hard part...


The CANBUS part is going to work like this project: https://github.com/Uksa007/esphome-jk-bms-can

I asked to buy the source code for this but they dont want to sell.

They have organised some documentation that will be helpful, such as: https://github.com/FurTrader/esphome-jk-bms-can/blob/main/docs/BMS-CAN%20Communication%20protocol.pdf

Key features:
 - 


## getting this to work on the Nomad Dev board 5:

Steve says:    
when I tried to upload this project to the Nomad there was a failure related to the flash size.    
It was because the Arduino framework didnt have a board definition.    
to fix it:     
Make sure you have a board definition json file here: "\.platformio\platforms\espressif32\boards\Nomad_BMS_ESP32-S3-WROOM-1-N4R2.json"
find the variant name in that file: "variant": "ESP32-S3-WROOM-1-N4R2"     
make a new folder with this name in the framework\variants folder. mine was here: "\.platformio\packages\framework-arduinoespressif32\variants\"     
Into this folder I copied 2 files which I got from some adafruit board folders: "partitions-4MB-tinyuf2.csv" and "pins_arduino.h"
I have added these files to a .zip and stuck it in the repo.   