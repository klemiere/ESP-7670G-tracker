#ifndef SIM_H
#define SIM_H

#include <HardwareSerial.h>

class Sim {
  public:
    Sim(HardwareSerial& serial, String simPIN, String simPUK);
    String sendAT(String command, unsigned int timeoutInSeconds = 2);
    void init();
  private:
    HardwareSerial& simModule;
    const char* certificatePEM;
    String simPIN;
    String simPUK;
    void checkSim();
    void networkInit();
    void SSLConfig();
    bool uploadCertificate(const String& filename, const char* certificate);
};

#endif
