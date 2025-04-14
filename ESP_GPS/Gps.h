#ifndef GPS_H
#define GPS_H

#include <Sim.h>

class Gps {
  public:
    Gps(Sim& sim);
    String sendAT(String command, unsigned int timeoutInSeconds = 1);
    void getCoordinates();
    void init();
  private:
    Sim& sim;
};

#endif
