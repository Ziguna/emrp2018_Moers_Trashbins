

# emrp2018_Moers_Trashbins
This tutorial shows how to measure the trash level of trashbins using the VL53L0X time-of-flight sensor, Heltec WIFI LoRa 32 (V2) board, The Things Network (TTN) and ... database.
## 1. Hardware Requirements
The following pieces of hardware are required for this tutorial:
- VL53L0X break-out board. Different boards by different suppliers may vary in appearance but the pinouts are generally the same. A popular VL53L0X break-out board is from Adafruit: https://www.adafruit.com/product/3317
- WIFI LoRa 32 (V2) board with antenna. This board includes an ESP32 microcontroller, an SX125x LoRa module. For more information: http://www.heltec.cn/project/wifi-lora-32/?lang=en

## 2. Software installation
### 2.1 Arduino IDE
Download and install the Arduino IDE from: https://www.arduino.cc/en/main/software

### 2.2 Libraries
Heltec support two sets of libraries to simplify the use of the integrated OLED, LoRa and other modules of the WIFI LoRa 32 (V2) board.

#### 2.2.1 Heltec Board Support Package
Download the repository [WiFi_Kit_series](https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series) as zip and extract it to `/Documents/Arduino/hardware/heltec`. Create the folders manually if they have not been created.

Navigate to `/Documents/Arduino/hardware/heltec/esp32/tools`, double-click on `get.exe` and wait for the script to finish.

Make sure the following folders are generated:

![tools folder](https://github.com/emrp/emrp2018_Moers_Trashbins/blob/master/pictures/sensor_node_ttn/tools_folder.jpg)

#### 2.2.2 Heltec Extended Libraries
Download the repository [Heltec_ESP32](https://github.com/HelTecAutomation/Heltec_ESP32) as zip and extract it to /Documents/Arduino/.

Plug in the heltec board and wait for the drivers to install (if needed).

Open the Arduino IDE, click on `Tools->Boards` and choose `Wifi_LoRa_32_V2`.

Click on `Sketch->Include Library->Add .Zip Libaries...`.

In the pop-up window, navigate to `/Documents/Arduino/` and choose `Heltec_ESP32-master.zip`.

#### 2.2.3 Running an OLED Example
Make sure the chosen board is `Wifi_LoRa_32_V2`.

![choose_board](https://github.com/emrp/emrp2018_Moers_Trashbins/blob/master/pictures/sensor_node_ttn/choose_board.jpg)

Click on \
`File->Examples->(Example from Custom Libraries)->Heltec ESP32 Dev-Boards->OLED->SSD1306SimpleDemo`.

![choose_example](https://github.com/emrp/emrp2018_Moers_Trashbins/blob/master/pictures/sensor_node_ttn/choose_example.jpg)

Click the `Upload` button to upload the program to the board. Check the OLED display to see if it works.

Open `Tools->Serial Monitor` to see the printed messages. Make sure the baudrate is set to `115200`. To reset the program, simply upload the program again or press the hardware reset button on the board.

Other examples are available within `Heltec ESP32 Dev-Boards`.

The next step is to install the library for the VL53L0X time-of-flight sensor and also the LMIC library to enable LoRaWan capabilities.
#### 2.2.4 Installing the VL53L0X Adafruit Library
Click on `Tools -> Manage Libraries...` and search for `VL53L0X`. Choose and install the `Adafruit_VL53L0X` library by Adafruit.

![VL53L0X_Adafruit](https://github.com/emrp/emrp2018_Moers_Trashbins/blob/master/pictures/sensor_node_ttn/VL53L0X_Adafruit.jpg)
#### 2.2.5 Installing the LMIC Library
Click on `Tools -> Manage Libraries...` and search for `LMIC`. Choose and install the `MCCI LoRaWAN LMIC` library by IBM, Mathis Koojiman, Terry Moore, ChaeHee Won, Frank Rose.

![LMIC library](https://github.com/emrp/emrp2018_Moers_Trashbins/blob/master/pictures/sensor_node_ttn/LMIC_LoRaWan.jpg)
#### 2.2.5 Installing the CayeneLPP Library
The CayenneLPP library is needed for the encoding and decoding of data to be handled by The Things Network. The API reference can be viewed [here](https://www.thethingsnetwork.org/docs/devices/arduino/api/cayennelpp.html).
Click on `Tools -> Manage Libraries...` and search for `CayenneLPP`. Choose and install the `CayeneLPP` library by Electronic Cats.
![CayenneLPP](https://github.com/emrp/emrp2018_Moers_Trashbins/blob/master/pictures/sensor_node_ttn/CayenneLPP.jpg)
## 3. Hardware Connections
### 3.1 WIFI LoRa 32 (V2) Board Pinouts
The originial pinout diagram provided by Heltec can be obtained [here](https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series/blob/master/PinoutDiagram/WIFI_LoRa_32_V2.pdf).
![Board pinouts](https://github.com/emrp/emrp2018_Moers_Trashbins/blob/master/pictures/sensor_node_ttn/WIFI_LoRA_32_V2_Pinouts.jpg)The pins used by the OLED module are fixed and are taken care of by the Heltec library.\
The pins used by the LoRa module are fixed and must be explicitly defined in the code to properly interface with the LMIC library [see section...].\

### 3.2 VL53L0X Connections
The VL53L0X sensor uses I2C to communicates with the microcontroller, this case being the ESP32 microcontroller on the Heltec LoRa Board. The I2C bus consists of two lines: SCL for clock and SDA for data. More information on the working mechanism of the I2C bus can be found [here](https://robot-electronics.co.uk/i2c-tutorial).
VL53L0X breakout board provides the following pinouts:
 
 -  VDD: positive supply voltage, to be connected to a power supply (2.6V to 3.5V)
 - GND: ground
 - SCL: clock line for I2C
 - SDA: data line for I2C
 - GPIO1: Interrupt output
 - XSHUT: shutdown pin, active low. Driving this pin LOW will put the sensor to standby mode.
 
The wiring between the VL53L0X breakout board and the WIFI LoRa 32 (V2) Board should be as in the figure below.

[PICTURE TO BE ADDED]

Note that the GPIO1 pin should be left unconnected as it is not used within the scope of this application.
## 4 Setting up TTN
### 4.1 Setting up a new TTN Application
Register for an account at https://www.thethingsnetwork.org/ \
Login and navigate to https://console.thethingsnetwork.org/ \
Click on `APPLICATION` to go to the application panel.
![console](https://github.com/emrp/emrp2018_Moers_Trashbins/blob/master/pictures/sensor_node_ttn/ttn_console.jpg)

Click on the `add application` button to add a new application. 
![add application](https://github.com/emrp/emrp2018_Moers_Trashbins/blob/master/pictures/sensor_node_ttn/ttn_add_app.jpg)

Give the application a **_unique application ID_** and choose the proper handler with respect to the region where the application is to be deployed.
![application id](https://github.com/emrp/emrp2018_Moers_Trashbins/blob/master/pictures/sensor_node_ttn/ttn_add_app_2.jpg)

The website will redirect to the application's `overview` page right after it is created.
![app overview](https://github.com/emrp/emrp2018_Moers_Trashbins/blob/master/pictures/sensor_node_ttn/ttn_add_app_3.jpg)
### 4.2 Setting the Payload Format
Click on the `Payload Formats` tab on the upper right corner of the `Application Overview` page. 
Choose `CayenneLPP` from the drop-down menu and click `save`.
![payload formats](https://github.com/emrp/emrp2018_Moers_Trashbins/blob/master/pictures/sensor_node_ttn/ttn_payload_formats.jpg)

### 4.3 Registering a Device
From the the `Application Overview` page, click on the `Devices` tab.
Click on the `register device` button.
![register device](https://github.com/emrp/emrp2018_Moers_Trashbins/blob/master/pictures/sensor_node_ttn/ttn_register_devices.jpg)

Give the device a *unique device name* (within the scope of the application, meaning there should be no other devices in the application with the same name).\
On the `Device EUI` section, click the `generate` button to automatically generate a Device EUI.\
Click the `Register` button to finish registering the device.
![register device panel](https://github.com/emrp/emrp2018_Moers_Trashbins/blob/master/pictures/sensor_node_ttn/ttn_register_devices_2.jpg)

The website will redirect to the `Device Overview` page after registration is complete.
![device overview](https://github.com/emrp/emrp2018_Moers_Trashbins/blob/master/pictures/sensor_node_ttn/ttn_device_overview.jpg)
Note that:

 - The **Activation Method** must be `OTAA` (changable in the `Settings` tab).
 - **Device EUI**, **Application EUI** and **App Key** are essensital for the configuration within the embedded software which will be covered in [section ...]

## 5 Writing the Embedded Software
### 5.1 Overview and Prerequisites
#### 5.1.1 Overview
The embedded software is to be flashed into the ESP32 microcontroller on the WIFI LoRa 32 (V2) board. The operating cycle is summarized in the figure below.\
![operating cycle](https://github.com/emrp/emrp2018_Moers_Trashbins/blob/master/pictures/sensor_node_ttn/operating_cycle.jpg)

The following includes must be listed in the code. Note that `lmic.h` must be included before `hal/hal.h`
````
#include <lmic.h>
#include <hal/hal.h>
#include <heltec.h>
#include <CayenneLPP.h>
#include <Adafruit_VL53L0X.h>
#include <driver/rtc_io.h>
````

#### 5.1.2 Device Keys
Each registered device from the TTN application has its unique **Application UI**, **Device EUI** and **Application Key** and can be found on the each [`Device Overview`](https://github.com/emrp/emrp2018_Moers_Trashbins/blob/master/pictures/sensor_node_ttn/ttn_device_overview.jpg) page. 
These values have to be copied in to the following variables inside the code:

 - ``static const u1_t PROGMEM APPEUI[8]``: little-endian format
 - ``static const u1_t PROGMEM DEVEUI[8]``: little-endian format
 - ``static const u1_t PROGMEM APPKEY[16]``: big-endian format

The correct endian formats are provided by clicking on the buttons right next to each key's values on the TTN [`Device Overview`](https://github.com/emrp/emrp2018_Moers_Trashbins/blob/master/pictures/sensor_node_ttn/ttn_device_overview.jpg) page.

Note that the values of these three keys are stored in the *program memory* section of the microcontroller, which means they will not be erased when the microcontroller is reset or when the microcontroller wakes up from sleep.

#### 5.1.3 Pin Configuration for LoRa Module
The pin configuration **must be** exactly as follows for the Heltec WIFI LoRa 32 (V2) board:
````
const lmic_pinmap lmic_pins = {
  .nss = 18,
  .rxtx = LMIC_UNUSED_PIN,
  .rst = 14,
  .dio = {26, 33, 32},
};
````

#### 5.2 Initialization
The `setup()` function takes care of the initialization stage. The following components are initialized:

 - Heltec board components: OLED display, LoRa module, UART Serial communication
 - The LMIC stack
 - RTC sleep timer (for waking up after sleep)

Note that the BootCount variable is stored into the RTC memory (by declaring it with `RTC_DATA_ATTR`). This is to ensure that the value will not be reset after the microcontroller wakes up.

### 5.3 Measurement
The measurement process is performed by three functions:

 - `L0X_init()`: driving the XSHUT pin HIGH to wake up the VL53L0X sensor and after that enabling the I2C communication between the sensor and the microcontroller
 
 - `L0X_getDistance`: performing the measurement to measure the trash level. The ouput is the mean of 5 successive measurements. If there is an error in measurement or the measured value is out of range (which is also considered an error as the maximum height of the trashcan is within the sensor's measuring range), a value of zero will be returned.
 
 - `L0X_deinit`: driving the XSHUT pin LOW to put the sensor into STANDBY mode which helps reduce power consumption.

The above three functions are called within the `do_send()` function.

### 5.4 Encoding and Transmission
The encoding is done with the CayenneLPP library and is inside the `do_send()` function:
````
lpp.reset();
lpp.addDigitalOutput(1, distance);
````

The maximum payload size is 51 bytes.
The transmission of data is performed right after the encoding has been completed:

`LMIC_setTxData2(1, lpp.getBuffer(), lpp.getSize(), 0);`

### 5.5 Sleep
The sleep timer (RTC) has to be enabled in the `setup()` function to wake the microcontroller up: 
`esp_sleep_enable_timer_wakeup()`

Before putting the microcontroller to sleep, all external peripherals (OLED display, LoRa module, VL53L0X) have to be also put into sleep mode. This is done via the `turnOffPeripherals()` function. 
Note that within this function, `rtc_gpio_hold_en(L0X_SHUTDOWN)` holds the value of the XSHUT pin on the VL53L0X sensor (connected to GPIO pin 13 on the Helec board) to keep the sensor on STANDBY mode even when the microcontroller has been put to sleep. 

The microcontroller is put into sleep mode by calling `esp_deep_sleep_start()`. Both `turnOffPeripherals()` and `esp_deep_sleep_start()` are called inside the `onEvent()` function. The `onEvent()` function is the event handler of the LMIC routine, which handles LoRaWan events.
