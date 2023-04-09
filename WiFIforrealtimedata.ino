#include <WiFi.h>
#include <WiFiUdp.h>
#include <time.h>

// Replace with your network credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// NTP server details
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 0;

// Create a UDP instance to communicate with the NTP server
WiFiUDP udp;

void setup() {
  Serial.begin(115200);
  Serial.println("Connecting to WiFi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }

  Serial.println("Connected to WiFi");

  // Set the time zone and initiate the UDP communication
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  udp.begin(2390);
}

void loop() {
  time_t now = time(nullptr);

  if (now > 0) {
    // The time has been fetched successfully
    Serial.println(ctime(&now));
  } else {
    // The time has not been fetched yet
    Serial.println("Waiting for time sync...");
  }

  delay(1000);
}
