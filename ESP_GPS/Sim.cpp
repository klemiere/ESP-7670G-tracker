#include <Sim.h>
#include <Arduino.h>

Sim::Sim(HardwareSerial& serial, String simPIN, String simPUK) : simModule(serial), simPIN(simPIN), simPUK(simPUK){
}

String Sim::sendAT(String command, unsigned int timeoutInSeconds) {
  unsigned int timeoutInMillis = timeoutInSeconds * 1000;
  simModule.println(command);

  String response = "";
  unsigned long timeout = millis() + timeoutInMillis; // 5-second timeout

  while (millis() < timeout) {
    while (simModule.available()) {
      char c = simModule.read();
      response += c;
      Serial.print(c);
      timeout = millis() + 500; // extend timeout on activity
    }
  }

  Serial.println(); // finish line
  return response;
}

void Sim::checkSim(){
  String cpin = "";
  do {
    cpin = sendAT("AT+CPIN?", 2);
  } while (cpin.indexOf("ERROR") != -1);
    
    if (cpin.indexOf("READY") != -1){
      Serial.println("Sim ready, skipping PIN...");
    } else if (cpin.indexOf("SIM PIN") != -1) {
      Serial.println("Entering PIN...");
      sendATCommand("AT+CPIN=" + simPIN, "OK", 1, true);
    } else if (cpin.indexOf("SIM PUK") != -1){
      Serial.println("Entering PUK...");
      sendATCommand("AT+CPIN=" + simPUK, "OK", 1, true);
    }
}

void Sim::networkInit(){
  String response = "";
  sendAT("AT+CGDCONT=1, \"IP\", \"free\"", 2); //define pdp context
  
  do {
    Serial.println("Acquiring network...");
    response = sendAT("AT+CREG?"); // check if connected to network
} while (response.indexOf("+CREG: 0,1") == -1 && response.indexOf("+CREG: 0,5") == -1);

  /*delay(2000);
  while (simModule.available()) {
   simModule.read();  //clear any leftover data in the buffer
  }*/
  
  sendAT("AT+CGACT=1,1");  // Activate PDP context
  sendAT("AT+CGPADDR=1");  // Get assigned IP
}

void Sim::SSLConfig(){
  sendAT("AT+CSSLCFG=\"sslversion\",0,3", "OK", 1, 0, true);
  sendAT("AT+CSSLCFG=\"authmode\",0,0", "OK", 1, 0, true);
  sendAT("AT+CSSLCFG=\"cacert\",0,\"gts_root.pem\"", "OK", 1, 0, true);
  sendAT("AT+CSSLCFG=\"ignorelocaltime\",0,1", "OK", 1, 0, true);
  sendAT("AT+CSSLCFG=\"ciphersuite\",0,\"?\"", "OK", 1, 0, true);
  sendAT("AT+CSSLCFG=\"enableSNI\",0,1", "OK", 1, 0, true);
  Serial.println("Uploading SSL certificate...");
}


void Sim::init(){
  Serial.println("Checking sim status...");
  checkSim();
  Serial.println("Initializing 4G network...");
  networkInit();
  Serial.println("Network initialized!");
  Serial.println("Configuring SSL");
  SSLConfig();
}
