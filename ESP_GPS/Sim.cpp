#include <Sim.h>
#include <Arduino.h>

Sim::Sim(HardwareSerial& serial, String simPIN, String simPUK) : simModule(serial), simPIN(simPIN), simPUK(simPUK){
}

String Sim::sendATCommand(String command, String expectedResponse, int timeoutInSeconds, int retryAttempts, bool printResponse) {
  String response = "";
  unsigned long timeoutInMilliseconds = timeoutInSeconds * 1000;
  unsigned long startTime;

  for (int i = 0; i <= retryAttempts; i++) {
    delay(500);
    simModule.println(command);
    startTime = millis();
    response = ""; // Clear response for each attempt

    while (millis() - startTime < timeoutInMilliseconds) {
      if (simModule.available()) {
        char c = simModule.read();
        response += c;
        if (printResponse) Serial.print(c);

        if (response.indexOf(expectedResponse) != -1) {
          return response; // Return if successful
        }
      }
    }

    Serial.println("Attempt " + String(i + 1) + " failed. Retrying...");
  }

  Serial.println("All attempts failed.");
  return response;
}

void Sim::checkSim(){
  String cpin = sendATCommand("AT+CPIN?", "OK", 1, 5);
    
    if (cpin.indexOf("READY") != -1){
      Serial.println("Sim ready, skipping PIN...");
    } else if (cpin.indexOf("SIM PIN") != -1) {
      Serial.println("Entering PIN...");
      sendATCommand("AT+CPIN=" + simPIN, "OK", 1, true);
    } else if (cpin.indexOf("SIM PUK") != -1){
      Serial.println("Entering PUK...");
      sendATCommand("AT+CPIN=" + simPUK, "OK", 1, true);
    } else {
      Serial.println("Sim initialization failed, restarting device.");
      ESP.restart();
    }
}

void Sim::HttpInit(){
  sendATCommand("AT+CGDCONT=1, \"IP\", \"free\"", "OK", 1, 5, 1);
}

void Sim::init(){
  Serial.println("Checking sim status...");
  checkSim();
  Serial.println("Initializing HTTP...");
  HttpInit();
}
