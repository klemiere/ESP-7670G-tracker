#include <Sim.h>
#include <Arduino.h>
#include <ssl_web.h>

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
      sendAT("AT+CPIN=" + simPIN);
    } else if (cpin.indexOf("SIM PUK") != -1){
      Serial.println("Entering PUK...");
      sendAT("AT+CPIN=" + simPUK);
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
  sendAT("AT+CSSLCFG=\"sslversion\",0,3");
  sendAT("AT+CSSLCFG=\"authmode\",0,0");
  sendAT("AT+CSSLCFG=\"cacert\",0,\"root_ca.pem\"");
  sendAT("AT+CSSLCFG=\"ignorelocaltime\",0,1");
  // sendAT("AT+CSSLCFG=\"ciphersuite\",0,\"?\"");
  sendAT("AT+CSSLCFG=\"enableSNI\",0,1");

  Serial.println("Setting up SSL certificate...");
  sendAT("AT+CCERTDELE");
  int certLength = strlen(root_ca);
  simModule.print("AT+CCERTDOWN=\"gts4.pem\",");
  simModule.println(certLength);
  
  if (simModule.find(">")) {
    Serial.println("Prompt received, sending certificate...");
    simModule.print(root_ca);
    // read back the "OK"
    while (simModule.available()) {
      Serial.write(simModule.read());
    }
  } else {
    Serial.println("No prompt received, certificate not sent.");
  }

  sendAT("AT+CCERTLIST");
  
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
