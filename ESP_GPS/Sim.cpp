#include <Sim.h>
#include <Arduino.h>
#include <algorithm> // For std::transform
#include <cctype>    // For ::tolower

Sim::Sim(HardwareSerial& serial) : simModule(serial){
}

String Sim::sendAT(String command, unsigned int timeoutInSeconds) {
  simModule.println(command);

  String response = "";
  int timeOutInMillis = timeoutInSeconds * 1000;
  unsigned long startMillis = millis();
  unsigned long lastRead = millis();

  while ((millis() - lastRead) < 1000 && (millis() - startMillis) < timeoutInMillis) {
    while (simModule.available()) {
      char c = simModule.read();
      response += c;
      Serial.print(c);
      lastRead = millis(); // reset timer
    }
    delay(10); // give time for data to arrive (maybe useless??)
  }

  Serial.println(); // newline for neat output
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

String Sim::httpReadResponse(){
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
          String responseData = dataResponse.substring(dataStart, dataEnd);  // Extract and trim the data
          responseData.trim();
          return responseData;
      } else {
          return "Failed to parse response data";
      }

    }
  }
  return "No response";
}

bool Sim::waitForDownloadPrompt(unsigned long timeoutMs) {
  unsigned long startTime = millis();
  String response = "";

  while (millis() - startTime < timeoutMs) {
    while (simModule.available()) {
      char c = simModule.read();
      response += c;

      if (response.indexOf("DOWNLOAD") != -1) {
        return true;
      }
    }
  }

  return false; // Timeout
}

String Sim::httpPostRequest(String url, String payload, int timeOutInSeconds){
  int payloadLength = payload.length();
  String responseData = "";

  sendAT("AT+HTTPINIT");
  sendAT("AT+HTTPPARA=\"URL\", \"" + url + "\"");
  sendAT("AT+HTTPPARA=\"CONTENT\", \"application/json\"");
  simModule.print("AT+HTTPDATA=");
  simModule.print(payloadLength);
  simModule.println(",10000");

  if (waitForDownloadPrompt()) {
    simModule.print(payload);  // Send JSON string
  } else {
    Serial.println("Timeout waiting for DOWNLOAD prompt");
  }

  delay(100); // may be useless

  sendAT("AT+HTTPACTION=1", timeOutInSeconds);
  String response = httpReadResponse();

  return response;
  //sendAT("AT+HTTPACTION=" + method, timeOutInSeconds);
}

String Sim::httpGetRequest(String url, int timeOutInSeconds){
  sendAT("AT+HTTPINIT");
  sendAT("AT+HTTPPARA=\"URL\", \"" + url + "\"");
  sendAT("AT+HTTPACTION=0", timeOutInSeconds);

  String responseData = httpReadResponse();

  sendAT("AT+HTTPTERM");
  return responseData;
}

String Sim::getICCID(){
  String response = sendAT("AT+CICCID");
  String iccid = "";

  // Start at ICCID
  int index = response.indexOf(": ");
  if (index != -1) {
    iccid = response.substring(index + 2);

    // Remove trailing F
    iccid.trim();
    while (!isdigit(iccid.charAt(iccid.length() - 1))) {
      iccid.remove(iccid.length() - 1);
    }

    if (iccid.length() > 15) {
      iccid = iccid.substring(iccid.length() - 15); // return only what's printed on the sim
    }
  }

  return iccid;
}

String Sim::getDateTime(){
  String response = sendAT("AT+CCLK?");
  int start = response.indexOf("\"") + 1;
  int end = response.indexOf("+", start);
  String timeData = response.substring(start, end);

  return timeData;
  
}

String Sim::serialize(String trackerIdentifier, String dateTime, String latitude, String longitude){
  String jsonPayload = "{";
  jsonPayload += "\"tracker_identifier\":\"" + trackerIdentifier + "\",";
  jsonPayload += "\"timestamp\":\"" + dateTime + "\",";
  jsonPayload += "\"latitude\":\"" + latitude + "\",";
  jsonPayload += "\"longitude\":\"" + longitude + "\"";
  jsonPayload += "}";
  return jsonPayload;
}

int Sim::sendData(String url, String payload){
  String response = httpPostRequest(url, payload);
  return 0;
}
