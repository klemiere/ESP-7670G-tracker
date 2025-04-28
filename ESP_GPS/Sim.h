#ifndef SIM_H
#define SIM_H

#include <HardwareSerial.h>

class Sim {
  public:
    Sim(HardwareSerial& serial);
    String sendAT(String command, unsigned int timeoutInSeconds = 2);
    void init();
    String httpRequest(String method = "get", String url = "http://51.178.25.133:8000/test_route", int timeOutInSeconds = 2);
    String getDateTime();
    String serialize(String latitude, String longitude, String dateTime);
  private:
    HardwareSerial& simModule;
    String simPIN;
    String simPUK;
    void checkSim();
    void networkInit();
};

#endif
