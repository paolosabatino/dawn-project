#include <Arduino.h>
#include <EEPROM.h>
#include <FS.h>
#include "config.h"
#include "dawn_project.h"

/**
 * Function to calculate the crc given a pointer in memory and the size of the data to calculate the crc on
 */
unsigned long calculateCrc(uint8_t *data, uint32_t size) {

  const unsigned long crc_table[16] = {
    0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
    0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
    0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
    0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
  };

  unsigned long crc = ~0L;

  for (unsigned int index = 0; index < size; index++) {
    crc = crc_table[(crc ^ data[index]) & 0x0f] ^ (crc >> 4);
    crc = crc_table[(crc ^ (data[index] >> 4)) & 0x0f] ^ (crc >> 4);
    crc = ~crc;
  }
  return crc;
  
}

void configSetup() {
  EEPROM.begin(4096);
}

void storeConfig(struct Config *config) {

  struct Config eConfig;

  memcpy(&eConfig, config, sizeof(struct Config));

  uint32_t crc = calculateCrc((uint8_t *)config, sizeof(struct Config));
  EEPROM.put(0, crc);
  EEPROM.put(4, eConfig);
  EEPROM.commit();

  Serial.print("Configuration stored into EEPROM. CRC:");
  Serial.println(crc, HEX);
  
}

/**
 * Retrieves the configuration from the EEPROM and checks if the CRC is valid. If the
 * CRC is valid, the function returns 0, otherwise returns 1 and the caller should initialize
 * the configuration with default values.
 */
int retrieveConfig(struct Config *config) {

  struct Config eConfig;

  uint32_t eCrc;
  uint32_t cCrc;
  
  EEPROM.get(0, eCrc);
  EEPROM.get(4, eConfig);

  cCrc = calculateCrc((uint8_t *)&eConfig, sizeof(struct Config));
  
  if (eCrc != cCrc)
    return 1;

  memcpy(config, &eConfig, sizeof(struct Config));

  return 0;
  
}
