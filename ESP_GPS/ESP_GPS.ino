#include "Sim7670.h"
#include "Sim.h"
#include "Gps.h"

#define RX_PIN 17 //SIM7670 TX
#define TX_PIN 18 //SIM7670 RX
#define SIM_PIN "1234"
#define SIM_PUK "30755571"

HardwareSerial sim7670(1);  // UART1
Sim sim(sim7670, SIM_PIN, SIM_PUK);
Gps gps(sim);
void setup() {
    
    Serial.begin(115200);
    sim7670.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);
    delay(2000);
    Serial.println("Initializing sim module");
    sim.init();

    
    /*Serial.println("Checking sim...");
    checkSim(&sim7670);

    sendATCommand(&sim7670, "AT+CGDCONT=1, \"IP\", \"free\"", "OK", 1, 1);
    sendATCommand(&sim7670, "AT+CGPADDR=?", "OK", 1, 1);
    sendATCommand(&sim7670, "AT+CREG?", "+CREG: 0,3", 10, 1);
    sendATCommand(&sim7670, "AT+COPS?", "OK", 1, 1);
    sendATCommand(&sim7670, "AT+CPING=\"www.google.com\", 1, 1", "OK", 1, 1); // ping to check if we have internet
    if (sendATCommand(&sim7670, "AT+HTTPINIT", "OK", 1, 1).indexOf("OK") == -1) {
      if (sendATCommand(&sim7670, "AT+HTTPTERM", "OK", 1, 1).indexOf("OK") == 1){
        if (sendATCommand(&sim7670, "AT+HTTPINIT", "OK", 1, 1).indexOf("OK") == -1) ESP.restart(); // attempt to start the HTTP service, if it fails restart the device
      } else{
        ESP.restart();
      }
    }
    
    Serial.println("HTTP service started!");

    sendATCommand(&sim7670, "AT+HTTPPARA=\"URL\",\"https://esp-7670g-tracker.onrender.com/test_route\"", "OK", 1, 1);
    String httpResponse = sendATCommandHTTP(&sim7670, "AT+HTTPACTION=0", "+HTTPACTION:", 60000, 1);
    Serial.println("Final Response: " + httpResponse);
    String responseSize = sendATCommandHTTP(&sim7670, "AT+HTTPREAD?", "+HTTPREAD:", 5000, 1);
    String httpData = sendATCommandHTTP(&sim7670, "AT+HTTPREAD=0,1024", "+HTTPREAD:", 60000, 1);*/
    
    /*Serial.println("Initializing GPS...");
    sendATCommand(&sim7670, "AT+CGNSSPWR=1", "OK"); // power on
    sendATCommand(&sim7670, "AT+CGNSSPWR?", "1"); // power check
    sendATCommand(&sim7670, "AT+CGNSSTST=1", "OK"); // Send data from UART3 to NMEA port
    //sendATCommand(&sim7670, "AT+CGPSCOLD", "OK"); // Cold start GPS
    sendATCommand(&sim7670, "AT+CGNSSIPR=115200", "OK"); // Set baud rate of UART3 and GPS module
    sendATCommand(&sim7670, "AT+CGNSSMODE=1", "OK"); // 15 = GPS + GLONASS + GALILEO + BDS
    sendATCommand(&sim7670, "AT+CGNSSMODE?", "1");
    sendATCommand(&sim7670, "AT+CGNSSNMEA=1,1,1,1,1,1,1,0,0,0", "OK"); //
    Serial.println("GPS: OK");*/
}

void loop() {

    sim7670.println("AT+CGNSSINFO");
    delay(500); //
    String response = "";

    while (sim7670.available()) {
        char c = sim7670.read();
        response += c;  // Append the character to the response string
    }

    /*String gnssData[20];
    getGnssRawData(response, gnssData);
    String latitude = getLatitude(gnssData);  // Latitude comes after the 5th comma
    String longitude = getLongitude(gnssData);  // Longitude comes after the 7th comma
    Serial.println("Latitude: " + latitude + "\nLongitude: " + longitude + "\n");*/
        
    //TODO: Get the time with LTE through ntp
    //TODO: Format the coordinates and timestamp
    //TODO: Send the coordinates to a server

    delay(1000);
}
