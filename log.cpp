#include <Arduino.h>
#include "log.h"

void setupLog(uint32_t bps) {

  Serial.begin(bps);
  
}

void log(const char *fmt, ...) {

  char buf[128]; // resulting string limited to 128 chars
  va_list args;
  va_start (args, fmt );
  vsnprintf(buf, 128, fmt, args);
  va_end (args);
  Serial.print(buf);
  
}

