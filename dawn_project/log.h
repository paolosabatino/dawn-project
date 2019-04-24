void setupLog(uint32_t);

//void log(const char *, ...);

#define log(FMT, ...) do { Serial.printf(FMT, ##__VA_ARGS__); } while(0);

