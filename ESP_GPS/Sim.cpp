#include <Sim.h>
#include <Arduino.h>
#include <algorithm> // For std::transform
#include <cctype>    // For ::tolower

Sim::Sim(HardwareSerial& serial, String simPIN, String simPUK) : simModule(serial), simPIN(simPIN), simPUK(simPUK){
}


String Sim::sendAT(String command, unsigned int timeoutInSeconds) {
  unsigned int timeoutInMillis = timeoutInSeconds * 1000;
  simModule.println(command);

  String response = "";
  unsigned long startMillis = millis(); // Start time for timeout

  while (millis() - startMillis < timeoutInMillis) {
    while (simModule.available()) {
      char c = simModule.read();
      response += c;
      Serial.print(c);
    }
  }

  Serial.println(); // Finish line
  return response;
}

void Sim::checkSim(){
  String cpin = "";
  do {
    cpin = sendAT("AT+CPIN?", 2);
  } while (cpin.indexOf("ERROR") != -1);
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

void Sim::init(){
  Serial.println("Checking sim status...");
  checkSim();
  Serial.println("Initializing 4G network...");
  networkInit();
  Serial.println("Network initialized!");
}

String Sim::httpRequest(String method, String url, int timeOutInSeconds){
  String responseData = "";
  for (char &c : method) {
        c = std::tolower(static_cast<unsigned char>(c));
    }

  if (method == "get") method = "0";
  else if (method == "post") method = "1";
  else if (method == "head") method = "2";
  else if (method == "delete") method = "3";
  else if (method == "put") method = "4";
  else {
    Serial.println("Invalid http method");
    return "Invalid http method";
  }

  sendAT("AT+HTTPINIT");
  sendAT("AT+HTTPPARA=\"URL\", \"" + url + "\"");
  sendAT("AT+HTTPACTION=" + method, timeOutInSeconds);

  String responseLength = sendAT("AT+HTTPREAD?");
  int commaIndex = responseLength.indexOf(",");
  if (commaIndex != -1) {
    // Extract the number after the comma and before any other character
    String lengthStr = responseLength.substring(commaIndex + 1);
    int length = lengthStr.toInt();

    // Debug: Check the parsed length
    // Serial.print("Parsed length: " + length);

    if (length > 0) {
      String dataResponse = sendAT("AT+HTTPREAD=0," + String(length));

      // Extract the actual response data
      int dataStart = dataResponse.indexOf("\"") + 1;
      int dataEnd = dataResponse.indexOf("\"", dataStart);

      if (dataStart != -1 && dataEnd != -1) {
          responseData = dataResponse.substring(dataStart, dataEnd);  // Extract and trim the data
          responseData.trim();
      } else {
          return "Failed to parse response data";
      }

    }
  }

  sendAT("AT+HTTPTERM");
  return responseData;

}
