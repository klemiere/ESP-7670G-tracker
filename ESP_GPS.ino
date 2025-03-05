#include "Sim7670.h"

#define RX_PIN 17 //SIM7670 TX
#define TX_PIN 18 //SIM7670 RX

HardwareSerial sim7670(1);  // UART1

void setup() {
    Serial.begin(115200);
    sim7670.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);
    sendATCommand(&sim7670, "AT+CRESET", "OK"); // reset the 7670 module to prevent issues
    delay(5000);  // Ensure the SIM7670G has booted properly

    
    Serial.println("Checking sim...");
    checkSim(&sim7670);

    //sendATCommand(&sim7670, "AT+CPING=\"www.google.com\", 1, 1", "OK", 1, 1); // ping to check if we have internet
    if (sendATCommand(&sim7670, "AT+HTTPINIT", "OK", 1, 1).indexOf("OK") == -1) ESP.restart(); // attempt to start the HTTP service, if it fails restart the device
    
    Serial.println("HTTP service started!");
    
    

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

    /*sim7670.println("AT+CGNSSINFO");
    delay(500); //
    String response = "";

    while (sim7670.available()) {
        char c = sim7670.read();
        response += c;  // Append the character to the response string
    }

    String gnssData[20];
    getGnssRawData(&sim7670, response, gnssData);
    String latitude = getLatitude(gnssData);  // Latitude comes after the 5th comma
    String longitude = getLongitude(gnssData);  // Longitude comes after the 7th comma
    Serial.println("Latitude: " + latitude + "\nLongitude: " + longitude + "\n");
        
    //TODO: Get the time with LTE through ntp
    //TODO: Format the coordinates and timestamp
    //TODO: Send the coordinates to a server*/

    delay(1000);
}
