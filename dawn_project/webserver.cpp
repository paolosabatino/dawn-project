#include <FS.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "webserver.h"
#include "dawn_project.h"
#include "config.h"
#include "log.h"

AsyncWebServer server(80);

struct Config *pConfig;

String cbRequestMain(const String &var) {

  String result;

  if (var == "red_value") {
    float red = pConfig->red_max_ramp / (float)MAX_RAMP;
    result = (String)((int)(red * 255));
  } else if (var == "green_value") {
    float green = pConfig->green_max_ramp / (float)MAX_RAMP;
    result = (String)((int)(green * 255));
  } else if (var == "blue_value") {
    float blue = pConfig->blue_max_ramp / (float)MAX_RAMP;
    result = (String)((int)(blue * 255));
  } else if (var == "mood_in_start") {
    result = String(pConfig->mood_lamp_begin_dawn);
  } else if (var == "mood_in_end") {
    result = String(pConfig->mood_lamp_begin_full);
  } else if (var == "mood_out_start") {
    result = String(pConfig->mood_lamp_end_full);
  } else if (var == "mood_out_end") {
    result = String(pConfig->mood_lamp_end_dawn);
  }

  return result;
  
}

void handleRequestMain(AsyncWebServerRequest *request) {

  request->send(SPIFFS, "/main.tpl", "text/html", false, cbRequestMain);
    
}

void handleNetwork(AsyncWebServerRequest *request) {

  File f = SPIFFS.open("/network.tpl", "r");

  if (!f) {
    
    request->send(500, "text/plain", "error opening network template file");
    
  } else {
    
    String strbuf = f.readString();

    strbuf.replace("%ssid%", String(pConfig->ssid));
    strbuf.replace("%passwd%", String(pConfig->passwd));

    request->send(200, "text/html", strbuf);
    
  }

  f.close();
  
}

void handleNetworkSet(AsyncWebServerRequest *request) {

  AsyncWebParameter* p;

  p = request->getParam(0);
  String ssid = p->value();

  log("parameter 0: name %s value %s\n", p->name().c_str(), p->value().c_str());

  p = request->getParam(1);
  String passwd = p->value();

  log("parameter 1: name %s value %s\n", p->name().c_str(), p->value().c_str());

  ssid.toCharArray(pConfig->ssid, 32);
  passwd.toCharArray(pConfig->passwd, 32);
  
  log("network set %s:%s\n", pConfig->ssid, pConfig->passwd);

  storeConfig(pConfig);

  request->send(200, "text/plain", "SSID Set, please reboot");
  
}

void handleNotFound(AsyncWebServerRequest *request) {

  String message = "resource not found\n";

  request->send( 404, "text/plain", message );
  
}

void handleRequestColorSet(AsyncWebServerRequest *request) {

  uint8_t i;
  float value;
  uint8_t error = 0;
  AsyncWebParameter* p;

  for (i=0; i < request->params(); i++) {

    p = request->getParam(i);
    value = p->value().toFloat() / (float)255;

    log("Parsing argument %d: name %s value %s\n", i, p->name().c_str(), p->value().c_str());
    
    if (value < 0 || value > 1) {
       error = 1;
       break;
    }

    if (p->name() == "red") {
      pConfig->red_max_ramp = (uint16_t)(value * (float)MAX_RAMP);
    } else if (p->name() == "green") {
      pConfig->green_max_ramp = (uint16_t)(value * (float)MAX_RAMP);
    } else if (p->name() == "blue") {
      pConfig->blue_max_ramp = (uint16_t)(value * (float)MAX_RAMP);
    } else {
      error = 1;
      break;
    }
    
  }

  storeConfig(pConfig);

  if (error == 0) {
    request->send(204, "", "");
  } else {
    request->send(400, "", "Bad Request");
  }

}

void handleRequestModeSet(AsyncWebServerRequest *request) {

  uint8_t i;
  uint8_t value;
  uint8_t error = 0;
  AsyncWebParameter* p;

  for (i=0; i < request->params(); i++) {

    p = request->getParam(i);
    
    value = p->value().toInt();

    log("Parsing argument %d: name %s value %s\n", i, p->name().c_str(), p->value().c_str());

    if (p->name() == "mode") {

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
    request->send(204, "", "");
  } else {
    request->send(400, "", "Bad Request");
  }

}

void handleRequestMoodTimes(AsyncWebServerRequest *request) {

  uint8_t i;
  uint32_t value;
  uint8_t error = 0;
  String argname;
  AsyncWebParameter *p;

  for (i=0; i < request->params(); i++) {

    p = request->getParam(i);
    value = p->value().toInt();

    log("Parsing argument %d: name %s value %s\n", i, p->name().c_str(), p->value().c_str());

    if (p->name() == "mood_in_start") {
       pConfig->mood_lamp_begin_dawn = value;
    } else if (p->name() == "mood_in_end") {
       pConfig->mood_lamp_begin_full = value;
    } else if (p->name() == "mood_out_start") {
      pConfig->mood_lamp_end_full = value;
    } else if (p->name() == "mood_out_end") {
      pConfig->mood_lamp_end_dawn = value;
    } else {
      error = 1;
      break;
    }
    
  }

  storeConfig(pConfig);
    
  if (error == 0) {
    request->send(204, "", "");
  } else {
    request->send(400, "", "Bad Request");
  }
  
}

void serverSetup(struct Config *config) {

  //server.on("/", HTTP_GET, handleRequestMain);
  server.on("/network", HTTP_GET, handleNetwork);
  server.on("/nwset", handleNetworkSet);
  server.on("/set", handleRequestColorSet);
  server.on("/mode", handleRequestModeSet);
  server.on("/mood_settings", handleRequestMoodTimes);

  server.on("/", HTTP_GET, handleRequestMain);
  //server.serveStatic("/index.html", SPIFFS, "/main.tpl").setTemplateProcessor(cbRequestMain);
  
  server.serveStatic("/main.css", SPIFFS, "/main.css");
  server.serveStatic("/main.js", SPIFFS, "/main.js");
  server.serveStatic("/bootstrap.min.css", SPIFFS, "/bootstrap.min.css");
  server.serveStatic("/bootstrap.min.js", SPIFFS, "/bootstrap.min.js");
  server.serveStatic("/jquery-3.3.1.slim.min.js", SPIFFS, "/jquery-3.3.1.slim.min.js");
  
  server.onNotFound ( handleNotFound );
  server.begin();

  pConfig = config;
  
}
