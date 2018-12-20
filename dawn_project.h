#define RED_PIN 13
#define GREEN_PIN 4
#define BLUE_PIN 12
#define ANALOG_PIN A0

#define MAX_RAMP 1023
#define RAMP_BITS 10
#define NETWORK_PEEK_TIME_MS 500
#define SOFTAP_TIMEOUT_SPAWN 10000
#define POWER_TIMEOUT_RECTIFY 10000

#define WORKING_MODE_OFF 0x0
#define WORKING_MODE_STATIC_DAY 0x1
#define WORKING_MODE_STATIC_NIGHT 0x2
#define WORKING_MODE_CYCLE_FREE 0x3
#define WORKING_MODE_MOOD_LAMP 0x4
#define WORKING_MODE_FADE_TO_MOOD 0x5

#define SECS_PER_MINUTE 60

#define PWM_CHANNELS 3

#define OUTPUT_POWER_DB 1


void set_light_static_day();
void set_light_static_night();
void set_light_mood_lamp();
void set_light_cycle_free();
void set_light_fade_to_mood();
void set_light_off();


