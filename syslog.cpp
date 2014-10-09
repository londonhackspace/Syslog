#include "syslog.h"

// Constructor
Syslog::Syslog() {
  _server = NULL;
  _ident = NULL;
  _fac = LOG_USER;
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
  // XXX test not very good here.
  if (strlen(message) > (SYSLOGBUFFERSIZE - 6)) {
     Serial.print("log message too long: ");
     Serial.println(message);
     return;
  }

  snprintf(packetBuffer, SYSLOGBUFFERSIZE, "<%d>%s %s", LOG_MAKEPRI(_fac, pri), _ident, message);
  Serial.println(packetBuffer);

  if (_server == NULL) {
     Serial.println("server not set");
     return;
  }

  Udp.beginPacket(_server, 514);
  Udp.write(packetBuffer);
  Udp.endPacket();
}

