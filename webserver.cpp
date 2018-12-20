#include <FS.h>
#include <ESP8266WebServer.h>
#include "webserver.h"
#include "dawn_project.h"
#include "config.h"
#include "log.h"

ESP8266WebServer server (80);

struct Config *pConfig;

void handleRequestColorSet() {

  uint8_t i;
  float value;
  uint8_t error = 0;

  for (i=0; i < server.args(); i++) {

    value = server.arg(i).toFloat() / (float)255;

    Serial.print("Parsing argument ");
    Serial.print(server.argName(i));
    Serial.print(" with value ");
    Serial.println(value);
    

    if (value < 0 || value > 1) {
       error = 1;
       break;
    }
      
    if (server.argName(i) == "red") {
      pConfig->red_max_ramp = (uint16_t)(value * (float)MAX_RAMP);
    } else if (server.argName(i) == "green") {
      pConfig->green_max_ramp = (uint16_t)(value * (float)MAX_RAMP);
    } else if (server.argName(i) == "blue") {
      pConfig->blue_max_ramp = (uint16_t)(value * (float)MAX_RAMP);
    } else {
      error = 1;
      break;
    }
    
  }

  storeConfig(pConfig);

  if (error == 0) {
    server.send (204, "", "");
  } else {
    server.send (400, "", "Bad Request");
  }

}

void handleRequestModeSet() {

  uint8_t i;
  uint8_t value;
  uint8_t error = 0;

  for (i=0; i < server.args(); i++) {

    value = server.arg(i).toInt();

    Serial.print("Parsing argument ");
    Serial.print(server.argName(i));
    Serial.print(" with value ");
    Serial.println(value);
    

    if (server.argName(i) == "mode") {

      working_mode = value;

      switch (value) {
        case WORKING_MODE_STATIC_DAY:
          set_light_static_day();
          break;
        case WORKING_MODE_STATIC_NIGHT:
          set_light_static_night();
          break;
        case WORKING_MODE_CYCLE_FREE:
          set_light_cycle_free();
          break;
        case WORKING_MODE_MOOD_LAMP:
          set_light_mood_lamp();
          break;
        case WORKING_MODE_FADE_TO_MOOD:
          set_light_fade_to_mood();
          break;
        default:
          set_light_off();
      }
      
    } else {
      error = 1;
      break;
    }
    
  }

  if (error == 0) {
    server.send (204, "", "");
  } else {
    server.send (400, "", "Bad Request");
  }

}

void handleRequestMoodTimes() {

  uint8_t i;
  uint32_t value;
  uint8_t error = 0;
  String argname;

  for (i=0; i < server.args(); i++) {

    value = server.arg(i).toInt();

    Serial.print("Parsing argument ");
    Serial.print(server.argName(i));
    Serial.print(" with value ");
    Serial.println(value);

    argname = server.argName(i);

    if (argname == "mood_in_start") {
       pConfig->mood_lamp_begin_dawn = value;
    } else if (argname == "mood_in_end") {
       pConfig->mood_lamp_begin_full = value;
    } else if (argname == "mood_out_start") {
      pConfig->mood_lamp_end_full = value;
    } else if (argname == "mood_out_end") {
      pConfig->mood_lamp_end_dawn = value;
    } else {
      error = 1;
      break;
    }
    
  }

  storeConfig(pConfig);
    
  if (error == 0) {
    server.send (204, "", "");
  } else {
    server.send (400, "", "Bad Request");
  }
  
}

void handleRequestMain() {

  float red;
  float green;
  float blue;

  File f = SPIFFS.open("/main.tpl", "r");

  if (!f) {
    
    server.send(500,"text/plain", "error opening main template file");
    
  } else {

    String strbuf = f.readString();

    red = pConfig->red_max_ramp / (float)MAX_RAMP;
    green = pConfig->green_max_ramp / (float)MAX_RAMP;
    blue = pConfig->blue_max_ramp / (float)MAX_RAMP;
  
    strbuf.replace("%red_value%", String( (int)(red * 255) ) );
    strbuf.replace("%green_value%", String( (int)(green * 255) ) );
    strbuf.replace("%blue_value%", String( (int)(blue * 255) ) );

    strbuf.replace("%mood_in_start%", String(pConfig->mood_lamp_begin_dawn));
    strbuf.replace("%mood_in_end%", String(pConfig->mood_lamp_begin_full));
    strbuf.replace("%mood_out_start%", String(pConfig->mood_lamp_end_full));
    strbuf.replace("%mood_out_end%", String(pConfig->mood_lamp_end_dawn));
  
    server.send(200,"text/html", strbuf);
  
  }

  f.close();
  
}

void handleNetwork() {

  File f = SPIFFS.open("/network.tpl", "r");

  if (!f) {
    
    server.send (500, "text/plain", "error opening network template file");
    
  } else {
    
    String strbuf = f.readString();

    strbuf.replace("%ssid%", String(pConfig->ssid));
    strbuf.replace("%passwd%", String(pConfig->passwd));

    server.send(200, "text/html", strbuf);
    
  }

  f.close();
  
}

void handleNetworkSet() {

  String ssid = server.arg(0);
  String passwd = server.arg(1);

  ssid.toCharArray(pConfig->ssid, 32);
  passwd.toCharArray(pConfig->passwd, 32);
  
  log ("network set %s:%s\n", pConfig->ssid, pConfig->passwd);

  storeConfig(pConfig);

  server.send(200, "text/plain", "SSID Set, please reboot");
  
}

void handleRequestCss() {

  File f = SPIFFS.open("/main.css", "r");

  if (!f) {
    
    server.send(500,"text/plain", "error opening main css file");
    
  } else {

    String strbuf = f.readString();

    server.send(200,"text/css", strbuf);
  
  }

  f.close();
  
}

void handleRequestJs() {

  File f = SPIFFS.open("/main.js", "r");

  if (!f) {
    
    server.send(500,"text/plain", "error opening main javascript file");
    
  } else {

    String strbuf = f.readString();

    server.send(200,"text/javascript", strbuf);
  
  }

  f.close();
  
}

void handleNotFound() {

  String message = "resource not found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }

  server.send ( 404, "text/plain", message );
  
}

void serverSetup(struct Config *config) {

  server.on ( "/", handleRequestMain );
  server.on ( "/network", handleNetwork );
  server.on ( "/nwset", handleNetworkSet );
  server.on ( "/main.css", handleRequestCss );
  server.on ( "/main.js", handleRequestJs );
  server.on ( "/set", handleRequestColorSet );
  server.on ( "/mode", handleRequestModeSet );
  server.on ( "/mood_settings", handleRequestMoodTimes );
  server.onNotFound ( handleNotFound );
  server.begin();

  pConfig = config;
  
}

void serverHandleClient() {
  server.handleClient();
}

