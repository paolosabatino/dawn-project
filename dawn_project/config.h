/*
 * Configuration options. These can be saved to EEPROM too to be retrieved on next turn on
 */
struct Config {

  uint16_t red_max_ramp;
  uint16_t green_max_ramp;
  uint16_t blue_max_ramp;

  uint32_t mood_lamp_begin_dawn;
  uint32_t mood_lamp_begin_full;
  uint32_t mood_lamp_end_full;
  uint32_t mood_lamp_end_dawn;

  char ssid[32];
  char passwd[32];

};

void storeConfig(struct Config *);
int retrieveConfig(struct Config *);
void configSetup();
