#ifndef SIM_H
#define SIM_H

#include <HardwareSerial.h>

class Sim {
  public:
    Sim(HardwareSerial& serial);
    String sendAT(String command, unsigned int timeoutInSeconds = 2);
    void init();
    String httpGetRequest(String url = "http://51.178.25.133:8000/test_route", int timeOutInSeconds = 2);
    String httpPostRequest(String url, String payload, int timeOutInSeconds = 2);
    String getICCID();
    String getDateTime();
    String serialize(String trackerIdentifier, String dateTime, String latitude, String longitude);
    int sendData(String url, String payload);
  private:
    HardwareSerial& simModule;
    String simPIN;
    String simPUK;
    void checkSim();
    void networkInit();
    String httpReadResponse();
    bool waitForDownloadPrompt(unsigned long timeoutMs = 10000);
};

#endif
