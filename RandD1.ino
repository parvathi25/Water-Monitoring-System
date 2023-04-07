#include <RTClib.h>
#include <WiFi.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <driver/adc.h>

#define TDS_SENSOR_PIN 34
#define LED_PIN 2
#define SWITCH_PIN 4
#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22
#define RTC_SDA_PIN 18
#define RTC_SCL_PIN 19
#define SD_CS_PIN 5

#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PASS "your_wifi_password"

#define FILENAME "/data.txt"

RTC_DS3231 rtc;
File dataFile;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(SWITCH_PIN, INPUT_PULLUP);

  Serial.begin(115200);
  while (!Serial);

  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  Wire.beginTransmission(0x68);
  if (!Wire.endTransmission()) {
    rtc.begin();
  } else {
    Serial.println("RTC not found!");
  }

  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  dataFile = SD.open(FILENAME, FILE_WRITE);
  if (!dataFile) {
    Serial.println("File open failed!");
    return;
  }

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi connected");
}

void loop() {
  float tds_value = read_tds_sensor();
  log_data(tds_value);

  int switch_state = digitalRead(SWITCH_PIN);
  if (switch_state == LOW) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  Serial.print("TDS: ");
  Serial.println(tds_value);

  delay(1000);
}

float read_tds_sensor() {
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten((adc1_channel_t)TDS_SENSOR_PIN, ADC_ATTEN_DB_11);
  uint32_t adc_data = adc1_get_raw((adc1_channel_t)TDS_SENSOR_PIN);
  float voltage = adc_data * 3.3 / 4096.0;
  float tds_value = voltage * 1000 / 5.0;
  return tds_value;
}

void log_data(float tds_value) {
  DateTime now = rtc.now();
  dataFile.print(now.year(), DEC);
  dataFile.print('/');
  dataFile.print(now.month(), DEC);
  dataFile.print('/');
  dataFile.print(now.day(), DEC);
  dataFile.print(' ');
  dataFile.print(now.hour(), DEC);
  dataFile.print(':');
  dataFile.print(now.minute(), DEC);
  dataFile.print(':');
  dataFile.print(now.second(), DEC);
  dataFile.print(',');
  dataFile.println(tds_value);
}
