#ifndef SIM_H
#define SIM_H

#include <HardwareSerial.h>

class Sim {
  public:
    Sim(HardwareSerial& serial, String simPIN, String simPUK);
    String sendATCommand(String command, String expectedResponse, int timeout = 1, int retryAttempts = 0, bool printResponse = false);
    void init();
  private:
    HardwareSerial& simModule;
    String simPIN;
    String simPUK;
    void checkSim();
    void networkInit();
    
};

#endif
