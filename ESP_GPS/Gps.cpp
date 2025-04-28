#include <Gps.h>

#include "Gps.h"

Gps::Gps(Sim& sim) : sim(sim) {
}

String Gps::sendAT(String command, unsigned int timeoutInSeconds){
  return sim.sendAT(command, timeoutInSeconds);
}

void Gps::init(){
  sendAT("AT+CGNSSPWR=1"); //power on
  sendAT("AT+CGNSSTST=1"); //sed data from uart3 to nmea port
  sendAT("AT+CGPSCOLD"); //cold start gps
  sendAT("AT+CGNSSIPR=115200"); // Set baud rate of UART3 and GPS module
  
  sendAT("AT+CGNSSMODE=1"); //
  /*1 GPS
    3 GPS + GLONASS
    5 GPS + GALILEO
    9 GPS + BDS
    13 GPS + GALILEO + BDS
    15 GPS + GLONASS + GALILEO + BDS
    The function will take effect immediately. */
    
  sendAT("AT+CGNSSNMEA=1,0,0,0,0,0,0,0,0,0");
  /*nGGA GGA output rate,default is 1
    nGLL GLL output rate,default is 1
    nGSA GSA output rate,default is 1
    nGSV GSV output rate,default is 1
    nRMC RMC output rate,default is 1
    nVTG VTG output rate,default is 1
    nZDA ZDA output rate,default is 1
    nGRS GRS output rate,default is 0
    nGST GST output rate,default is 0
    nGNS GNS output rate,default is 0
    The function will take effect immediately. */
}

void Gps::getGnssRawData(String* gnssArray){
  String rawData = "";

  String gnssResponse = "";
  do{
    gnssResponse = sendAT("AT+CGPSINFO");
    delay(5000);
  } while (gnssResponse.indexOf(": ,") != -1);

  // Check if the reponse contains GNSS data
  if (gnssResponse.indexOf("+CGPSINFO:") > 0) {
        Serial.println("Received GNSS info: " + gnssResponse); // Print for debugging
  } 
  else {
        Serial.println("No GNSS data");
        return;
  }
        
  // Extract the raw GNSS data part from the response
  int start = gnssResponse.indexOf("+CGPSINFO:") + 11;
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
}

float convertToDecimalDegrees(String nmeaCoord) {
  int pointIndex = nmeaCoord.indexOf('.');
  int degreesLength = pointIndex == 4 ? 2 : 3;  // 2 digits for latitude, 3 digits for longitude
  float degrees = nmeaCoord.substring(0, degreesLength).toFloat();
  float minutes = nmeaCoord.substring(degreesLength).toFloat();
  return degrees + (minutes / 60.0);
}

String Gps::getLatitude(String gnssArray[]){
  if (gnssArray[0] == "") return "Invalid Latitude";
  char buffer[20];
  sprintf(buffer, "%.6f", convertToDecimalDegrees(gnssArray[0]));
  return String(buffer) + " " + gnssArray[1];
}

String Gps::getLongitude(String gnssArray[]){
  if (gnssArray[2] == "") return "Invalid Longitude";
  char buffer[20];
  sprintf(buffer, "%.6f", convertToDecimalDegrees(gnssArray[2]));
  return String(buffer) + " " + gnssArray[3];
}


