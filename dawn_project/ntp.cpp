#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <TimeLib.h>
#include <Timezone.h>
#include <WiFiUdp.h>
#include "ntp.h"

WiFiUDP Udp;
static const char ntp_server_name[] = "it.pool.ntp.org";
const int ntp_time_zone= 1;
uint8_t ntp_packet_buffer[NTP_PACKET_SIZE];
uint16_t ntp_local_port = 8888;

TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};     //Central European Summer Time
TimeChangeRule CET = {"CET", Last, Sun, Oct, 3, 60};       //Central European Standard Time
Timezone CE(CEST, CET);

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  
  // set all bytes in the buffer to 0
  memset(ntp_packet_buffer, 0, NTP_PACKET_SIZE);
  
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  ntp_packet_buffer[0] = 0b11100011;   // LI, Version, Mode
  ntp_packet_buffer[1] = 0;     // Stratum, or type of clock
  ntp_packet_buffer[2] = 6;     // Polling Interval
  ntp_packet_buffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  ntp_packet_buffer[12] = 49;
  ntp_packet_buffer[13] = 0x4E;
  ntp_packet_buffer[14] = 49;
  ntp_packet_buffer[15] = 52;
  
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(ntp_packet_buffer, NTP_PACKET_SIZE);
  
  Udp.endPacket();
  
}

/*
 * getNtpTime tries to retrieve the time from the internet via NTP server and returns some
 * kind of integer value that will be used by setSyncProvider() as new time reference
 */
time_t getNtpTime() {
  
  IPAddress ntp_server_ip; // NTP server's ip address
  int size;
  unsigned long secs_since_1900;
  time_t utc;
  TimeChangeRule *tcr;

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  
  Serial.println("Transmit NTP Request");
  
  // get a random server from the pool
  WiFi.hostByName(ntp_server_name, ntp_server_ip);
  
  Serial.print(ntp_server_name);
  Serial.print(": ");
  Serial.println(ntp_server_ip);
  
  sendNTPpacket(ntp_server_ip);
  
  uint32_t begin_wait = millis();
  
  while (millis() - begin_wait < NTP_TIMEOUT_MS) {
    
    size = Udp.parsePacket();
    
    if (size >= NTP_PACKET_SIZE) {
      
      Serial.println("Receive NTP Response");
      Udp.read(ntp_packet_buffer, NTP_PACKET_SIZE);  // read packet into the buffer
      
      // convert four bytes starting at location 40 to a long integer
      secs_since_1900 =  (unsigned long)ntp_packet_buffer[40] << 24;
      secs_since_1900 |= (unsigned long)ntp_packet_buffer[41] << 16;
      secs_since_1900 |= (unsigned long)ntp_packet_buffer[42] << 8;
      secs_since_1900 |= (unsigned long)ntp_packet_buffer[43];

      utc = secs_since_1900 - 2208988800UL;

      return CE.toLocal(utc, &tcr);
      
//      return secs_since_1900 - 2208988800UL + ntp_time_zone * SECS_PER_HOUR;

    }

    yield();
    
  }
  
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
  
}

void ntpSetup() {

  Serial.println("NTP: Starting UDP");
  
  Udp.begin(ntp_local_port);
  Serial.print("NTP: Local port: ");
  Serial.println(Udp.localPort());
  Serial.println("NTP: waiting for sync");

  
  setSyncProvider(getNtpTime);
  setSyncInterval(NTP_SYNC_INTERVAL);
  
}
