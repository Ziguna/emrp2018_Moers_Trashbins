# emrp2018_Moers_Trashbins
This tutorial shows you how to measure the trash level of trashbins using the VL53L0X time-of-flight sensor, Heltec WIFI LoRA 32 (V2) board, The Things Network (TTN) and ... database.
## 1. Hardware Requirements
The following pieces of hardware are required for this tutorial:
- VL53L0X break-out board. You can get one here from Adafruit: https://www.adafruit.com/product/3317
- WIFI LoRA 32 (V2) board with antenna. This board includes an ESP32 microcontroller, an SX125x LoRa module. For more information: http://www.heltec.cn/project/wifi-lora-32/?lang=en

## 2. Software installation
### 2.1 Arduino IDE
Download and install the Arduino IDE from: https://www.arduino.cc/en/main/software

### 2.2 Libraries
Heltec support two sets of libraries to simplify the use of the integrated OLED, LoRa and other modules of the WIFI LoRA 32 (V2) board.

#### 2.2.1 Heltec Board Support Package
Download the repository [WiFi_Kit_series](https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series) as zip and extract it to `/Documents/Arduino/hardware/heltec`. Create the folders manually if they have not been created.

Navigate to `/Documents/Arduino/hardware/heltec/esp32/tools`, double-click on `get.exe` and wait for the script to finish.

Make sure the following folders are generated:

![tools folder](https://github.com/emrp/emrp2018_Moers_Trashbins/blob/master/pictures/instruction/tools_folder.jpg

#### 2.2.2 Heltec Extended Libraries
Download the repository [Heltec_ESP32](https://github.com/HelTecAutomation/Heltec_ESP32) as zip and extract it to /Documents/Arduino/.

Plug in the heltec board and wait for the drivers to install (if needed).

Open the Arduino IDE, click on `Tools->Boards` and choose `Wifi_LoRa_32_V2`.

Click on `Sketch->Include Library->Add .Zip Libaries...`.

In the pop-up window, navigate to `/Documents/Arduino/` and choose `Heltec_ESP32-master.zip`.

#### 2.2.3 Running an OLED Example
Make sure the chosen board is `Wifi_LoRa_32_V2`.

![choose_board](https://github.com/emrp/emrp2018_Moers_Trashbins/blob/master/pictures/instruction/choos_board.jpg)

Click on \
`File->Examples->(Example from Custom Libraries)->Heltec ESP32 Dev-Boards->OLED->SSD1306SimpleDemo`.

![choose_example](https://github.com/emrp/emrp2018_Moers_Trashbins/blob/master/pictures/instruction/choose_example.jpg)

Click the `Upload` button to upload the program to the board. Check the OLED display to see if it works.

Open `Tools->Serial Monitor` to see the printed messages. Make sure the baudrate is set to `115200`. You can upload the program again or press the hardware reset button on the board to reset the program.

You can also try other examples within `Heltec ESP32 Dev-Boards`.

The next step is to install the library for the VL53L0X time-of-flight sensor and also the LMIC library to enable LoRaWan capabilities.
#### 2.2.4 Installing the VL53L0X Adafruit Library
Click on `Tools -> Manage Libraries...` and search for `VL53L0X`.\
Choose and install the `Adafruit_VL53L0X` library by Adafruit.

![VL53L0X_Adafruit](https://github.com/emrp/emrp2018_Moers_Trashbins/blob/master/pictures/instruction/VL53L0X_Adafruit.jpg)
#### 2.2.5 Installing the LMIC Library
Click on `Tools -> Manage Libraries...` and search for `LMIC`.\
Choose and install the `MCCI LoRaWAN LMIC` library by IBM, Mathis Koojiman, Terry Moore, ChaeHee Won, Frank Rose.

![LMIC library](https://github.com/emrp/emrp2018_Moers_Trashbins/blob/master/pictures/instruction/LMIC_LoRaWan.jpg)
#### 2.2.5 Installing the CayeneLPP Library
The CayenneLPP library is needed for the encoding and decoding of data to be handled by The Things Network. The API reference can be viewed [here](https://www.thethingsnetwork.org/docs/devices/arduino/api/cayennelpp.html).
Click on `Tools -> Manage Libraries...` and search for `CayenneLPP`.\
Choose and install the `CayeneLPP` library by Electronic Cats.
![CayenneLPP](https://github.com/emrp/emrp2018_Moers_Trashbins/blob/master/pictures/instruction/CayenneLPP.jpg)
## 3. Hardware Connections
### 3.1 WIFI LoRA 32 (V2) Board Pinouts
The originial pinout diagram provided by Heltec can be obtained [here](https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series/blob/master/PinoutDiagram/WIFI_LoRa_32_V2.pdf).
![Board pinouts](https://github.com/emrp/emrp2018_Moers_Trashbins/blob/master/pictures/instruction/WIFI_LoRA_32_V2_Pinouts.jpg)The pins used by the OLED module are fixed and are taken care of by the Heltec library.\
The pins used by the LoRa module are fixed and must be explicitly defined in the code to properly interface with the LMIC library [see section...].\
The pins for the OLED and LoRa module should not be used to interface with any other external peripherals.
### 3.2 



