#ifndef SIM7670_H
#define SIM7670_H

#include <HardwareSerial.h>

void checkSim(HardwareSerial *sim7670 );
String sendATCommand(HardwareSerial *sim7670, String command, String expectedResponse, int loopAmount = -1, int debug = -1);
String sendATCommandHTTP(HardwareSerial *sim7670, String command, String expectedResponse, int timeout, int debug);
int getGnssRawData(String gnssResponse, String *gnssArray);
void sendData();

#endif // GNSS_H
