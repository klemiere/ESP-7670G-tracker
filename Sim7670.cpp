#include "Sim7670.h"



String sendATCommand(HardwareSerial *sim7670, String command, String expectedResponse, int loopAmount, int debug) {
  String response = "";
  int count = 0;
  while (loopAmount == -1 || count < loopAmount){
    String cResponse = "";
    sim7670->println(command);
      delay(2000);
      while (sim7670->available()) {
        char c = sim7670->read();
        cResponse += c;
      }
      response = cResponse;
      if (debug == 1) Serial.println(response);
      if (response.indexOf(expectedResponse) != -1) break;
      count++;
  }
  return response;
}

void checkSim(HardwareSerial *sim7670) {
  String cpin = sendATCommand(sim7670, "AT+CPIN?", "OK");
    
    if (cpin.indexOf("READY") != -1){
      Serial.println("Sim ready, skipping PIN...");
    } else if (cpin.indexOf("SIM PIN") != -1) {
      Serial.println("Entering PIN...");
      sendATCommand(sim7670, "AT+CPIN=1234", "OK", -1, 1);
    } else if (cpin.indexOf("SIM PUK") != 1){
      Serial.println("Entering PUK...");
      sendATCommand(sim7670, "AT+CPIN=30755571", "OK", -1, 1);
    } else {
      Serial.println("Unhandled condition!");
    }
}

int getGnssRawData(String gnssResponse, String *gnssArray){
  
  // Check if the reponse contains GNSS data
  if (gnssResponse.indexOf("+CGNSSINFO:") > 0) {
        Serial.println("Received GNSS info:");
        Serial.println(gnssResponse); // Print for debugging
  } 
  else {
        Serial.println("No GNSS data");
        return 1;
  }
        
  // Extract the raw GNSS data part from the response
  int start = gnssResponse.indexOf("+CGNSSINFO:") + 11;
  String gnssData = gnssResponse.substring(start);
  
  int index = 0;
  String token = "";
  
  // Split the data by commas
  for (int i = 0; i < gnssData.length(); i++) {
          char currentChar = gnssData.charAt(i);
          if (currentChar == ',') {
              gnssArray[index++] = token;  // Store the token before the comma
              token = "";  // Reset token for the next value
          } else {
              token += currentChar;  // Build the current token
          }                                                                                                                                             
  }
  // Store the last token (after the last comma)
  gnssArray[index] = token;
  
  return 0;
}

String getLatitude(String gnssArray[]){
  if (gnssArray[5] == "") return "Invalid Latitude";
  return gnssArray[5] + " " + gnssArray[6];
}

String getLongitude(String gnssArray[]){
  if (gnssArray[7] == "") return "Invalid Longitude";
  return gnssArray[7] + " " + gnssArray[8];
}

String getDate(){
  //TODO: Implement
}

String getTime(){
  //TODO: Implement
}

void sendData(){
  //TODO: Implement
}
