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
          Serial.println();
          return response; // Return if successful
        }
      }
    }

    Serial.println("Attempt " + String(i + 1) + " failed.");
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

void Sim::networkInit(){
  String response = "";
  sendATCommand("AT+CGDCONT=1, \"IP\", \"free\"", "OK", 1, 5, true); //define pdp context
   do {
    Serial.println("Acquiring network");
    response = sendATCommand("AT+CREG?", "+CREG: 0,1", 20, 6, true); //Check if the sim is registered to a network
    //TODO: This only works on a home network, add roaming support
   } while (response.indexOf("+CREG: 0,1") == -1);
   delay(2000);
   while (simModule.available()) {
    simModule.read();  // Clear any leftover data in the buffer
   }
   sendATCommand("AT+CGATT?", "OK", 1, 2, true);
}

void Sim::init(){
  Serial.println("Checking sim status...");
  checkSim();
  Serial.println("Initializing HTTP...");
  networkInit();
  sendATCommand("AT+HTTPTERM", "OK"); /*send termination signal in case http is still initialized,
  this will print "Attempts failed" if it wasn't initialized but who cares*/
}
