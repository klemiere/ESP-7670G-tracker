#ifndef GPS_H
#define GPS_H

#include <Sim.h>

class Gps {
  public:
    Gps(Sim& sim);
    String sendATCommand(String command, String expectedResponse, int timeout = 1, bool printResponse = false);
    void getCoordinates();
  private:
    Sim& sim;
};

#endif
