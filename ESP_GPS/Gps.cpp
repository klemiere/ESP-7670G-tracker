#include <Gps.h>

#include "Gps.h"

Gps::Gps(Sim& sim) : sim(sim) {
}

String Gps::sendATCommand(String command, String expectedResponse, int timeoutInSeconds, bool printResponse){
  return sim.sendATCommand(command, expectedResponse, printResponse);
}
