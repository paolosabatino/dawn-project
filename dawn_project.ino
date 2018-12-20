#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <TimeLib.h>
#include <FS.h>
#include "dawn_project.h"
#include "webserver.h"
#include "config.h"
#include "ntp.h"
#include "log.h"

extern "C" {
  #include "pwm.h"
}

/*
  const char *ssid = "Alice-48151623";
  const char *password = "fg24.asw24-";
*/

const uint32_t pwm_period = 15000; // * 200ns ^= 1 kHz
const uint16_t gamma10[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9, 10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11, 12, 12, 12, 12, 12, 12, 13, 13, 13, 13, 13, 14, 14, 14, 14, 14, 15, 15, 15, 15, 15, 16, 16, 16, 16, 16, 17, 17, 17, 17, 18, 18, 18, 18, 18, 19, 19, 19, 19, 20, 20, 20, 20, 21, 21, 21, 21, 22, 22, 22, 22, 23, 23, 23, 23, 24, 24, 24, 24, 25, 25, 25, 26, 26, 26, 26, 27, 27, 27, 28, 28, 28, 28, 29, 29, 29, 30, 30, 30, 31, 31, 31, 31, 32, 32, 32, 33, 33, 33, 34, 34, 34, 35, 35, 35, 36, 36, 36, 37, 37, 37, 38, 38, 39, 39, 39, 40, 40, 40, 41, 41, 41, 42, 42, 43, 43, 43, 44, 44, 44, 45, 45, 46, 46, 46, 47, 47, 48, 48, 48, 49, 49, 50, 50, 50, 51, 51, 52, 52, 53, 53, 53, 54, 54, 55, 55, 56, 56, 56, 57, 57, 58, 58, 59, 59, 60, 60, 60, 61, 61, 62, 62, 63, 63, 64, 64, 65, 65, 66, 66, 67, 67, 68, 68, 69, 69, 70, 70, 71, 71, 72, 72, 73, 73, 74, 74, 75, 75, 76, 76, 77, 77, 78, 78, 79, 79, 80, 81, 81, 82, 82, 83, 83, 84, 84, 85, 86, 86, 87, 87, 88, 88, 89, 90, 90, 91, 91, 92, 92, 93, 94, 94, 95, 95, 96, 97, 97, 98, 98, 99, 100, 100, 101, 102, 102, 103, 103, 104, 105, 105, 106, 107, 107, 108, 109, 109, 110, 111, 111, 112, 113, 113, 114, 114, 115, 116, 117, 117, 118, 119, 119, 120, 121, 121, 122, 123, 123, 124, 125, 125, 126, 127, 128, 128, 129, 130, 130, 131, 132, 133, 133, 134, 135, 136, 136, 137, 138, 139, 139, 140, 141, 142, 142, 143, 144, 145, 145, 146, 147, 148, 148, 149, 150, 151, 152, 152, 153, 154, 155, 156, 156, 157, 158, 159, 160, 160, 161, 162, 163, 164, 165, 165, 166, 167, 168, 169, 170, 170, 171, 172, 173, 174, 175, 175, 176, 177, 178, 179, 180, 181, 182, 182, 183, 184, 185, 186, 187, 188, 189, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 270, 272, 273, 274, 275, 276, 277, 278, 279, 280, 282, 283, 284, 285, 286, 287, 288, 290, 291, 292, 293, 294, 295, 297, 298, 299, 300, 301, 302, 304, 305, 306, 307, 308, 310, 311, 312, 313, 314, 316, 317, 318, 319, 320, 322, 323, 324, 325, 326, 328, 329, 330, 331, 333, 334, 335, 336, 338, 339, 340, 341, 343, 344, 345, 347, 348, 349, 350, 352, 353, 354, 356, 357, 358, 359, 361, 362, 363, 365, 366, 367, 369, 370, 371, 373, 374, 375, 377, 378, 379, 381, 382, 383, 385, 386, 387, 389, 390, 392, 393, 394, 396, 397, 398, 400, 401, 403, 404, 405, 407, 408, 410, 411, 412, 414, 415, 417, 418, 419, 421, 422, 424, 425, 427, 428, 430, 431, 432, 434, 435, 437, 438, 440, 441, 443, 444, 446, 447, 449, 450, 452, 453, 455, 456, 458, 459, 461, 462, 464, 465, 467, 468, 470, 471, 473, 474, 476, 477, 479, 480, 482, 483, 485, 487, 488, 490, 491, 493, 494, 496, 497, 499, 501, 502, 504, 505, 507, 509, 510, 512, 513, 515, 517, 518, 520, 521, 523, 525, 526, 528, 530, 531, 533, 534, 536, 538, 539, 541, 543, 544, 546, 548, 549, 551, 553, 554, 556, 558, 559, 561, 563, 565, 566, 568, 570, 571, 573, 575, 576, 578, 580, 582, 583, 585, 587, 589, 590, 592, 594, 596, 597, 599, 601, 603, 604, 606, 608, 610, 611, 613, 615, 617, 619, 620, 622, 624, 626, 628, 629, 631, 633, 635, 637, 638, 640, 642, 644, 646, 648, 649, 651, 653, 655, 657, 659, 660, 662, 664, 666, 668, 670, 672, 674, 675, 677, 679, 681, 683, 685, 687, 689, 691, 693, 694, 696, 698, 700, 702, 704, 706, 708, 710, 712, 714, 716, 718, 720, 722, 723, 725, 727, 729, 731, 733, 735, 737, 739, 741, 743, 745, 747, 749, 751, 753, 755, 757, 759, 761, 763, 765, 767, 769, 771, 773, 775, 778, 780, 782, 784, 786, 788, 790, 792, 794, 796, 798, 800, 802, 804, 806, 809, 811, 813, 815, 817, 819, 821, 823, 825, 827, 830, 832, 834, 836, 838, 840, 842, 845, 847, 849, 851, 853, 855, 857, 860, 862, 864, 866, 868, 870, 873, 875, 877, 879, 881, 884, 886, 888, 890, 892, 895, 897, 899, 901, 904, 906, 908, 910, 913, 915, 917, 919, 922, 924, 926, 928, 931, 933, 935, 937, 940, 942, 944, 947, 949, 951, 953, 956, 958, 960, 963, 965, 967, 970, 972, 974, 977, 979, 981, 984, 986, 988, 991, 993, 995, 998, 1000, 1003, 1005, 1007, 1010, 1012, 1014, 1017, 1019, 1022};

uint32_t stp;

uint16_t srcRed;
uint16_t srcGreen;
uint16_t srcBlue;

uint16_t destRed;
uint16_t destGreen;
uint16_t destBlue;

uint16_t working_mode;

uint8_t ap_started = 0;
uint8_t wifi_connected = 0;

/*
   fade to mood mode variables
*/
uint32_t ftm_ts_begin;
uint32_t ftm_ts_end;

uint32_t pwm_skips;

struct Config config;

void setupPwm() {

  // PWM setup
  uint32 io_info[PWM_CHANNELS][3] = {
    // MUX, FUNC, PIN
    {PERIPHS_IO_MUX_GPIO4_U, FUNC_GPIO4, RED_PIN},
    {PERIPHS_IO_MUX_MTCK_U,  FUNC_GPIO13, GREEN_PIN},
    {PERIPHS_IO_MUX_MTDI_U,  FUNC_GPIO12, BLUE_PIN}

  };

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  //pinMode(10, INPUT);

  // initial duty: all off
  uint32_t pwm_duty_init[PWM_CHANNELS] = {0, 0, 0};

  pwm_init(pwm_period, pwm_duty_init, PWM_CHANNELS, io_info);
  pwm_start();

}

/**
   Initializes the configuration to default values, to be used in case retrieveConfig() fails
*/
void defaultConfig() {

  config.red_max_ramp = MAX_RAMP;
  config.green_max_ramp = MAX_RAMP;
  config.blue_max_ramp = MAX_RAMP;

  config.mood_lamp_begin_dawn = 12 * SECS_PER_HOUR + 0 * SECS_PER_MINUTE + 0;
  config.mood_lamp_begin_full = 12 * SECS_PER_HOUR + 15 * SECS_PER_MINUTE + 0;
  config.mood_lamp_end_full = 12 * SECS_PER_HOUR + 30 * SECS_PER_MINUTE + 0;
  config.mood_lamp_end_dawn = 12 * SECS_PER_HOUR + 45 * SECS_PER_MINUTE + 0;

  memset(config.ssid, 0, sizeof(config.ssid));
  memset(config.passwd, 0, sizeof(config.passwd));

}

void waitLoop(int dl) {
  int lt = millis();
  while (lt - millis() < dl) {
    delay(10);
    if (ap_started == 1 || wifi_connected == 1)
      serverHandleClient();
  }
}

void networkLoop() {

  static uint32_t last_cycle = 0;
  static uint32_t softap_last_check = millis();

  if (wifi_connected == 1 || ap_started == 1) {
    serverHandleClient();

  }



  if (wifi_connected == 1) {

    if (ap_started == 1) {
      WiFi.softAPdisconnect(true);
      ap_started = 0;
    }

  } else {

    if (ap_started == 0) {
      if (millis() - softap_last_check > SOFTAP_TIMEOUT_SPAWN) {
        log("Spawning mood_lamp network SSID\n");
        WiFi.softAP("mood_lamp");
        ap_started = 1;
      }
    } else {
      softap_last_check = millis();
    }

    if (millis() - last_cycle > NETWORK_PEEK_TIME_MS) {

      if (WiFi.status() == WL_CONNECTED) {

        char localIp[16];
        WiFi.localIP().toString().toCharArray(localIp, 16);
        log("Connected to %s\nIP Address: %s\n", config.ssid, localIp);

        if (MDNS.begin ("mood_lamp"))
          log("MDNS responder started\n");

        ntpSetup();

        wifi_connected = 1;

      }

      last_cycle = millis();

    }

  }

}

void setup() {

  setupLog(115200);
  //analogWriteFreq(1000);
  setupPwm();
  configSetup();

  log("Boot\n");

  if (retrieveConfig(&config) == 1) {
    log("Configuration CRC does not match, using defaults\n");
    defaultConfig();
  }

  srcRed = 0;
  srcGreen = 0;
  srcBlue = 0;

  destRed = MAX_RAMP;
  destGreen = MAX_RAMP;
  destBlue = 0;

  stp = 0;

  if (SPIFFS.begin()) {
    log("SPIFFS mounted\n");
  } else {
    log("SPIFFS error\n");
  }

  /*
     Connect to network at this point or just skip the step. SoftAP mode will be
     enabled later if we are not a client of a network yet.
  */
  if (strlen(config.ssid) > 0 && strlen(config.passwd) > 0) {
    log("Connecting to network %s\n", config.ssid);
    WiFi.begin (config.ssid, config.passwd);
  } else {
    log("No network SSID or password stored\n");
  }

  WiFi.begin();
  WiFi.setOutputPower(OUTPUT_POWER_DB);
  WiFi.softAPdisconnect(true);

  serverSetup(&config);
  log("HTTP server started\n");

  pinMode(ANALOG_PIN, INPUT);

  //working_mode = WORKING_MODE_MOOD_LAMP;
  //set_light_mood_lamp();
  working_mode = WORKING_MODE_CYCLE_FREE;
  //set_light_

}

void do_light_cycle() {

  time_t curtime;

  stp++;

  if (stp >= MAX_RAMP) {

    srcRed = destRed;
    srcGreen = destGreen;
    srcBlue = destBlue;

    destRed = random(0, config.red_max_ramp);
    destGreen = random(0, config.green_max_ramp);
    destBlue = random(0, config.blue_max_ramp);

    curtime = now();

    log("Time:%d:%d:%d\tNew dest color:%d\t%d\t%d, pin10:%d\n",
        hour(curtime),
        minute(curtime),
        second(curtime),
        destRed,
        destGreen,
        destBlue,
        digitalRead(10)
       );

    stp = 0;

  }

}

void set_light_off() {

  srcRed = 0;
  srcGreen = 0;
  srcBlue = 0;

  destRed = 0;
  destGreen = 0;
  destBlue = 0;

  stp = 0;

}

void set_light_static_day() {

  srcRed = config.red_max_ramp;
  srcGreen = config.green_max_ramp;
  srcBlue = config.blue_max_ramp;

  destRed = 0;
  destGreen = 0;
  destBlue = 0;

  stp = 0;

}

void set_light_static_night() {

  srcRed = config.red_max_ramp;
  srcGreen = (config.green_max_ramp >> 2) + (config.green_max_ramp >> 3);
  srcBlue = 0;

  destRed = 0;
  destGreen = 0;
  destBlue = 0;

  stp = 0;

}

void set_light_cycle_free() {

  // Do nothing

}

void set_light_mood_lamp() {

  destRed = config.red_max_ramp;
  destGreen = config.green_max_ramp;
  destBlue = config.blue_max_ramp;

  srcRed = 0;
  srcGreen = 0;
  srcBlue = 0;

  stp = 0;

}

void set_light_fade_to_mood() {

  uint32_t red;
  uint32_t green;
  uint32_t blue;
  uint32_t cur_time;
  time_t now_time;

  red = (srcRed * (MAX_RAMP - stp) + destRed * stp) >> RAMP_BITS;
  green = (srcGreen * (MAX_RAMP - stp) + destGreen * stp) >> RAMP_BITS;
  blue = (srcBlue * (MAX_RAMP - stp) + destBlue * stp) >> RAMP_BITS;

  srcRed = red;
  srcGreen = green;
  srcBlue = blue;

  destRed = 0;
  destGreen = 0;
  destBlue = 0;

  now_time = now();
  ftm_ts_begin = time_to_day_timestamp(now_time);
  ftm_ts_end = ftm_ts_begin + (5 * SECS_PER_MINUTE);

  log("Fade to Mood starts at %d:%d:%d, ends in 5 minutes. Current rgb %d %d %d\n",
      hour(now_time),
      minute(now_time),
      second(now_time),
      red,
      green,
      blue
     );

}

uint32_t time_to_day_timestamp(time_t time_value) {

  return hour(time_value) * SECS_PER_HOUR + minute(time_value) * SECS_PER_MINUTE + second(time_value);

}

uint8_t is_time_between(uint32_t ts_begin, uint32_t ts_end, uint32 ts_test) {

  if (ts_end < ts_begin) {
    if (ts_test <= ts_end || ts_test >= ts_begin)
      return 1;
  } else {
    if (ts_test >= ts_begin && ts_test <= ts_end)
      return 1;
  }

  return 0;

}

uint16_t get_ramp_time_diff(uint32_t ts_begin, uint32_t ts_end, uint32_t ts_value) {

  int32_t interval_seconds = ts_end - ts_begin;
  int32_t pos_in_interval = ts_value - ts_begin;

  if (interval_seconds < 0)
    interval_seconds += 24 * SECS_PER_HOUR;

  if (pos_in_interval < 0)
    pos_in_interval += 24 * SECS_PER_HOUR;

  log("interval seconds:%d pos: %d\n", interval_seconds, pos_in_interval);

  if (pos_in_interval < 0)
    return 0;

  if (pos_in_interval > interval_seconds)
    return MAX_RAMP;

  return (pos_in_interval * (uint32_t)(MAX_RAMP)) / interval_seconds;

}

void do_off() {

  waitLoop(1000);

}

void do_static_light() {

  waitLoop(1000);

}

void do_mood_lamp() {

  uint32_t cur_time;
  time_t now_time;

  now_time = now();
  cur_time = time_to_day_timestamp(now_time);

  log("Time:%d:%d:%d\n",
      hour(now_time),
      minute(now_time),
      second(now_time)
     );

  if (is_time_between(config.mood_lamp_begin_dawn, config.mood_lamp_begin_full, cur_time)) {

    log("Time is between dawn and full\n");
    stp = get_ramp_time_diff(config.mood_lamp_begin_dawn, config.mood_lamp_begin_full, cur_time);

  } else if (is_time_between(config.mood_lamp_begin_full, config.mood_lamp_end_full, cur_time)) {

    log("Time is full\n");
    stp = MAX_RAMP;

  } else if (is_time_between(config.mood_lamp_end_full, config.mood_lamp_end_dawn, cur_time)) {

    log("Time is between full and dawn\n");
    stp = MAX_RAMP - get_ramp_time_diff(config.mood_lamp_end_full, config.mood_lamp_end_dawn, cur_time);

  }

  waitLoop(1000);

}

void do_fade_to_mood() {

  uint32_t cur_time;
  time_t now_time;

  now_time = now();
  cur_time = time_to_day_timestamp(now_time);

  stp = get_ramp_time_diff(ftm_ts_begin, ftm_ts_end, cur_time);

  if (stp == MAX_RAMP) {
    log("fade to mood complete, set mood lamp working mode\n");
    set_light_mood_lamp();
    working_mode = WORKING_MODE_MOOD_LAMP;
  }

  waitLoop(250);

}

void loop() {

  uint16_t red, green, blue;
  
  // put your main code here, to run repeatedly:
  networkLoop();

  switch (working_mode) {
    case WORKING_MODE_OFF:
    default:
      do_off();
      break;
    case WORKING_MODE_STATIC_DAY:
      set_light_static_day();
      do_static_light();
      break;
    case WORKING_MODE_STATIC_NIGHT:
      set_light_static_night();
      do_static_light();
      break;
    case WORKING_MODE_CYCLE_FREE:
      do_light_cycle();
      break;
    case WORKING_MODE_MOOD_LAMP:
      do_mood_lamp();
      break;
    case WORKING_MODE_FADE_TO_MOOD:
      do_fade_to_mood();
      break;
  }

  red = (srcRed * (MAX_RAMP - stp) + destRed * stp) >> RAMP_BITS;
  green = (srcGreen * (MAX_RAMP - stp) + destGreen * stp) >> RAMP_BITS;
  blue = (srcBlue * (MAX_RAMP - stp) + destBlue * stp) >> RAMP_BITS;

  /*
    analogWrite(RED_PIN, gamma10[red]);
    analogWrite(GREEN_PIN, gamma10[green]);
    analogWrite(BLUE_PIN, gamma10[blue]);
  */

  red = (gamma10[red] * pwm_period) >> RAMP_BITS;
  green = (gamma10[green] * pwm_period) >> RAMP_BITS;
  blue = (gamma10[blue] * pwm_period) >> RAMP_BITS;

  //log("r:%d\tg:%d\tb:%d\n", red, green, blue);

  pwm_set_duty(red, 0);
  pwm_set_duty(green, 1);
  pwm_set_duty(blue, 2);
  pwm_start(); // commit
    
  delay(10);

}
