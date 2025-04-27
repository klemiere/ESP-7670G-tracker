#include "Sim7670.h"
#include "Sim.h"
#include "Gps.h"

#define RX_PIN 17 //SIM7670 TX
#define TX_PIN 18 //SIM7670 RX
#define DTR_PIN 5 //SIM7670 DTR
#define SIM_PIN "1234"
#define SIM_PUK "30755571"

HardwareSerial sim7670(1);  // UART1
Sim sim(sim7670, SIM_PIN, SIM_PUK);
Gps gps(sim);


void setup() {

  Serial.begin(115200);
  sim7670.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);
  delay(2000);

  Serial.println("Initializing sim module...");
  sim.init();
  Serial.println("Sim module initialized!");
  delay(2000);

  Serial.println("Initializing GPS module...");
  gps.init();
  Serial.println("GPS module initialized!");
  delay(2000);

  Serial.println("Setting time...");
  sim.sendAT("AT+CNTP=\"pool.ntp.org\",0");
  sim.sendAT("AT+CNTP");
  Serial.println("Time set!");
  delay(2000);
  
}

void loop() {
  // sim.httpRequest("get", "http://51.178.25.133:8000/test_route");
  gps.getDateTime();
}
