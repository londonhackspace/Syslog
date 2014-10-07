#include "syslog.h"

// Constructor
Syslog::Syslog() {
}

// set server, our ident, and the facility to use.
void Syslog::begin(const char *server, const char * ident, int fac) {
  _server = server;
  _fac = fac;
  _ident = ident;

  Udp.begin(1234);
}

// in case we need to change facility.
void Syslog::setFacility(int fac) {
  _fac = fac;
}

// send a message with a priority
void Syslog::syslog(int pri, const char *message) {
  if (_server == NULL) {
     Serial.println("server not set");
     return;
  }

  // XXX test not very good here.
  if (strlen(message) > (PS - 6)) {
     Serial.print("log message too long: ");
     Serial.println(message);
     return;
  }

  snprintf(packetBuffer, PS, "<%d>%s %s", LOG_MAKEPRI(_fac, pri), _ident, message);

  Serial.println(packetBuffer);

  Udp.beginPacket(_server, 514);
  Udp.write(packetBuffer);
  Udp.endPacket();
}

