#include <lmic.h>
#include <hal/hal.h>
#include <heltec.h>
#include <CayenneLPP.h>
#include <Adafruit_VL53L0X.h>
#include <driver/rtc_io.h>

#define uS_TO_S_FACTOR    1000000  // Conversion factor for micro seconds to seconds
#define SLEEP_TIME_IN_SEC 60       // Time ESP32 will go to sleep (in seconds)
#define BUILTIN_LED       25
#define L0X_SHUTDOWN      GPIO_NUM_13
#define TX_INTERVAL       1 // might become longer due to duty cycle limitations

RTC_DATA_ATTR uint8_t BootCount = 0;
static bool SleepIsEnabled = false;

Adafruit_VL53L0X lox; // time-of-flight infarred sensor
CayenneLPP lpp(51);

// This EUI must be in little-endian format, so least-significant-byte
// first. When copying an EUI from ttnctl output, this means to reverse
// the bytes. For TTN issued EUIs the last bytes should be 0xD5, 0xB3, 0x70.
static const u1_t PROGMEM APPEUI[8] = { 0xCC, 0x46, 0x01, 0xD0, 0x7E, 0xD5, 0xB3, 0x70 };
void os_getArtEui (u1_t* buf) {
  memcpy_P(buf, APPEUI, 8);
}
// This should also be in little endian format, see above.
static const u1_t PROGMEM DEVEUI[8] = { 0x87, 0xDC, 0x3F, 0xF5, 0xD4, 0xEF, 0x8B, 0x00 };
void os_getDevEui (u1_t* buf) {
  memcpy_P(buf, DEVEUI, 8);
}
// This key should be in big endian format (or, since it is not really a
// number but a block of memory, endianness does not really apply). In
// practice, a key taken from ttnctl can be copied as-is.
// 
static const u1_t PROGMEM APPKEY[16] = { 0x17, 0xBE, 0x1E, 0x87, 0xAC, 0xEB, 0x30, 0x3B, 0x95, 0x29, 0x6A, 0xA6, 0x76, 0x62, 0x8A, 0x1B };
void os_getDevKey (u1_t* buf) {
  memcpy_P(buf, APPKEY, 16);
}

static osjob_t sendjob;

// Schedule TX every this many seconds (might become longer due to duty
// cycle limitations),
// uncomment only when sleep is not used

// Pin mapping
const lmic_pinmap lmic_pins = {
  .nss = 18,
  .rxtx = LMIC_UNUSED_PIN,
  .rst = 14,
  .dio = {26, 33, 32},
};

void onEvent (ev_t ev) {
  Serial.print(os_getTime());
  Serial.print(": ");
  Heltec.display->clear();
  switch (ev) {
    case EV_SCAN_TIMEOUT:
      Serial.println(F("EV_SCAN_TIMEOUT"));
      Heltec.display->drawString(0, 7, "EV_SCAN_TIMEOUT");
      break;
    case EV_BEACON_FOUND:
      Serial.println(F("EV_BEACON_FOUND"));
      Heltec.display->drawString(0, 7, "EV_BEACON_FOUND");
      break;
    case EV_BEACON_MISSED:
      Serial.println(F("EV_BEACON_MISSED"));
      Heltec.display->drawString(0, 7, "EV_BEACON_MISSED");
      break;
    case EV_BEACON_TRACKED:
      Serial.println(F("EV_BEACON_TRACKED"));
      Heltec.display->drawString(0, 7, "EV_BEACON_TRACKED");
      break;
    case EV_JOINING:
      Serial.println(F("EV_JOINING"));
      Heltec.display->drawString(0, 7, "EV_JOINING   ");
      break;
    case EV_JOINED:
      Serial.println(F("EV_JOINED"));
      Heltec.display->drawString(0, 7, "EV_JOINED    ");
      {
        u4_t netid = 0;
        devaddr_t devaddr = 0;
        u1_t nwkKey[16];
        u1_t artKey[16];
        LMIC_getSessionKeys(&netid, &devaddr, nwkKey, artKey);
        Serial.print("netid: ");
        Serial.println(netid, DEC);
        Serial.print("devaddr: ");
        Serial.println(devaddr, HEX);
        Serial.print("artKey: ");
        for (int i=0; i<sizeof(artKey); ++i) {
          Serial.print(artKey[i], HEX);
        }
        Serial.println("");
        Serial.print("nwkKey: ");
        for (int i=0; i<sizeof(nwkKey); ++i) {
          Serial.print(nwkKey[i], HEX);
        }
        Serial.println("");
      }
      // Disable link check validation (automatically enabled
      // during join, but not supported by TTN at this time).
      LMIC_setLinkCheckMode(0);
      break;
    case EV_RFU1:
      Serial.println(F("EV_RFU1"));
      Heltec.display->drawString(0, 7, "EV_RFUI");
      break;
    case EV_JOIN_FAILED:
      Serial.println(F("EV_JOIN_FAILED"));
      Heltec.display->drawString(0, 7, "EV_JOIN_FAILED");
      
      // Go to sleep
      turnOffPeripherals();
      esp_deep_sleep_start(); // equals to a reboot
      break;
    case EV_REJOIN_FAILED:
      Serial.println(F("EV_REJOIN_FAILED"));
      Heltec.display->drawString(0, 7, "EV_REJOIN_FAILED");
      //break;
      break;
    case EV_TXCOMPLETE:
      Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
      Heltec.display->drawString(0, 7, "EV_TXCOMPLETE");
      if (LMIC.txrxFlags & TXRX_ACK) {
        Serial.println(F("Received ack"));
        Heltec.display->drawString(0, 7, "Received ACK");
      }
      if (LMIC.dataLen) {
        Serial.println(F("Received "));
        Heltec.display->drawString(0, 6, "RX ");
        Serial.println(LMIC.dataLen);
        Heltec.display->printf("%i bytes", LMIC.dataLen);
        Serial.println(F(" bytes of payload"));
        Heltec.display->printf("RSSI %d SNR %.1d", LMIC.rssi, LMIC.snr);
      }
      
      //Schedule next transmission, comment this out only when DEEP sleep is used
      os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(TX_INTERVAL), do_send);
      
      // Go to sleep
      goToSleep();

      break;
    case EV_LOST_TSYNC:
      Serial.println(F("EV_LOST_TSYNC"));
      Heltec.display->drawString(0, 7, "EV_LOST_TSYNC");
      break;
    case EV_RESET:
      Serial.println(F("EV_RESET"));
      Heltec.display->drawString(0, 7, "EV_RESET");
      break;
    case EV_RXCOMPLETE:
      // data received in ping slot
      Serial.println(F("EV_RXCOMPLETE"));
      Heltec.display->drawString(0, 7, "EV_RXCOMPLETE");
      break;
    case EV_LINK_DEAD:
      Serial.println(F("EV_LINK_DEAD"));
      Heltec.display->drawString(0, 7, "EV_LINK_DEAD");
      break;
    case EV_LINK_ALIVE:
      Serial.println(F("EV_LINK_ALIVE"));
      Heltec.display->drawString(0, 7, "EV_LINK_ALIVE");
      break;
    case EV_TXSTART:
        Serial.println(F("EV_TXSTART"));
        Heltec.display->drawString(0, 7, "EV_TXSTART");
        
        break;    
    default:
      Serial.println(F("Unknown event"));
      Heltec.display->drawString(0, 7, "UNKNOWN EVENT %d");
      break;
  }
  Heltec.display->display();
}

void do_send(osjob_t* j) {
  // Check if there is not a current TX/RX job running
  if (LMIC.opmode & OP_TXRXPEND) {
    Serial.println(F("OP_TXRXPEND, not sending"));
    Heltec.display->drawString(0, 7, "OP_TXRXPEND, not sent");
  } else {     
    delay(100);

    // Measure distance
    L0X_init();
    int16_t distance = 0;
    distance = L0X_getDistance();
    L0X_deinit();

    // Encode using CayenneLPP
    lpp.reset();
    lpp.addDigitalOutput(1, distance);
    
    // Prepare upstream data transmission at the next possible time.
    LMIC_setTxData2(1, lpp.getBuffer(), lpp.getSize(), 0);
    
    Serial.println(F("Packet queued"));
    Heltec.display->clear();
    Heltec.display->drawString(0, 7, "PACKET QUEUED");
    Heltec.display->display();
  }
  // Next TX is scheduled after TX_COMPLETE event.
}

void setup() {
  Heltec.begin(true /*DisplayEnable Enable*/, true /*LoRa Enable*/, true /*Serial Enable*/, true, 866E6);
  Heltec.LoRa.setSpreadingFactor(8);
  
  Serial.println("=============================================");
  if (BootCount == 0)
  {    
    Serial.println("First Boot");
    Serial.println("=============================================");
  }  
  else
  {
    Serial.println("Woke up");
    Serial.println("=============================================");
  }
  
  Serial.print("BootCount: "); Serial.println(BootCount);
  BootCount++;
  
  // LMIC init
  os_init();
  
  // Reset the MAC state. Session and pending data transfers will be discarded.
  LMIC_reset();
  
  //delay(100);
  LMIC_setLinkCheckMode(0);
 
  // Start job (sending automatically starts OTAA too)
  do_send(&sendjob);
}

void loop() {
  if (!SleepIsEnabled) // Enable peripherals again (after each sleep)
  {
    Heltec.begin(true /*DisplayEnable Enable*/, true /*LoRa Enable*/, true /*Serial Enable*/, true, 866E6);
    Serial.println("Enabling sleep inside loop");
    Heltec.LoRa.setSpreadingFactor(8);
    
    SleepIsEnabled = true;
  }
  os_runloop_once();
}

void resetDisplay(void)
{
  pinMode(16, OUTPUT);
  digitalWrite(16, LOW);
  delay(50);
  digitalWrite(16, HIGH);
}

void L0X_init(void)
{ 
  rtc_gpio_hold_dis(L0X_SHUTDOWN);
  pinMode(L0X_SHUTDOWN, OUTPUT);
  digitalWrite(L0X_SHUTDOWN, HIGH);
  
  delay(100);
  //Wire.begin(21, 22, 100000);
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
}

void L0X_deinit(void)
{
  digitalWrite(L0X_SHUTDOWN, LOW);
}

// return distance in cm
int16_t L0X_getDistance(void)
{
  VL53L0X_RangingMeasurementData_t measure;
  int16_t distance = 0;
  for (int i = 0; i < 5; i++)
  {
    delay(100);
    Serial.print("Reading a measurement... ");
    lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
    
    if (measure.RangeStatus != 4) {  // phase failures have incorrect data
      distance += measure.RangeMilliMeter / 10;
      Serial.print("Distance (cm): "); Serial.println(measure.RangeMilliMeter / 10);
    } else {
      Serial.println(" out of range ");
      return 0;
    }
  }
  return (distance / 5);
}

// 
void turnOffPeripherals(void)
{
  Serial.println("Going to sleep");
  delay(100);
  Serial.end();
  rtc_gpio_hold_en(L0X_SHUTDOWN);
  Heltec.display->sleep();
  LoRa.sleep();
}

void goToSleep(void)
{
  SleepIsEnabled = true;
  // Sleep all peripherals and enable RTC timer
  turnOffPeripherals();
  esp_sleep_enable_timer_wakeup(SLEEP_TIME_IN_SEC * uS_TO_S_FACTOR);
  esp_light_sleep_start();
  
  // After timer runs out
  //Serial.begin(115200);           // uncomment this for debugging
  //Serial.println("after sleep");  // uncomment this for debugging
  SleepIsEnabled = false;  
}
