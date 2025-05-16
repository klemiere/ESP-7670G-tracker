#include "Sim.h"
#include "Gps.h"
#include "esp_sleep.h"

#define RX_PIN 17 //SIM7670 TX
#define TX_PIN 18 //SIM7670 RX
#define DTR_PIN 5 //SIM7670 DTR

HardwareSerial sim7670(1);  // UART1
Sim sim(sim7670);
Gps gps(sim);

String trackerIdentifier = "";

void setup() {

  Serial.begin(115200);
  sim7670.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);
  delay(2000);

  Serial.println("Initializing sim module...");
  sim.init();
  Serial.println("Sim module initialized!");
  delay(2000);
  String trackerIdentifier = sim.getICCID();

  Serial.println("Setting time...");
  sim.sendAT("AT+CSQ");
  sim.sendAT("AT+CNTP=\"pool.ntp.org\",0");
  sim.sendAT("AT+CNTP", 20);
  sim.sendAT("AT+CCLK?");
  Serial.println("Time set!");
  delay(2000);

  Serial.println("Initializing GPS module...");
  gps.init();
  Serial.println("GPS module initialized!");
  delay(2000);
  
}

void loop() {

  String gnssArray[25];
  gps.getGnssRawData(gnssArray);
  String dateTime = sim.getDateTime();
  String latitude = gps.getLatitude(gnssArray);
  String longitude = gps.getLongitude(gnssArray);

  String json = sim.serialize(trackerIdentifier, dateTime, latitude, longitude);
  sim.sendData("http://51.178.25.133:8000/post_coordinates", json);
  Serial.flush();
  esp_sleep_enable_timer_wakeup(3600 * 1000000); // 1 hour in microseconds
  esp_light_sleep_start();

}
