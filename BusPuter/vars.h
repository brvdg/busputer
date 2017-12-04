/****************************************************
 * Busputer Variables
 * 
 * for detailed informations see 
 * https://github.com/brvdg/busputer/wiki
 ****************************************************/



/*
 * global vars
 */
//String bootmsg;
String bootmsg1;
String bootmsg2;
String bootmsg3;



// this is a large buffer for replies
char replybuffer[255];
// a smal buffer
char buf[24]; //

boolean serial_export = true;
boolean SPI_lock = false;
boolean I2C_lock = false;
byte saved_config = 0;

/*
 * Variables ...
 */
// Temperature / Humidity
float temp_out = 0;
byte temp_out_port = TEMP_OUT_PORT;
float temp_in = -127;
float hum_in = -127;
int hum_out = 0;
int ds18b20_temp = -127;


// Voltage
float bord_voltage = 0;
//float bord_voltage_blynk = 0;
byte bord_voltage_port = BORD_VOLTAGE_PORT;

// Dimmer
byte dimmer_port = DIMMER;
float dimmer_V = 0;
//byte mood_port = 2;
byte dimmer_min = 0;
byte dimmer_max = 10;

// Fuel
float fuel_V = 0;
float fuel_l = 43.8;
float fuel_ohm = 0;
byte fuel_port = FUEL_GAUGE_PORT;
int fuel_gaugeresistor = FUEL_GAUGERESISTOR;
byte fuel_warning = FUEL_WARNING;

// Water Temperatur
float water_temp_V = 0; // Voltage of port
float water_temp_ohm = 0;
float water_temp = 0;
byte water_temp_port = WATER_TEMP;
byte water_temp_warning = WATER_TEMP_WARNING;

// RPM
float rpm = 0;
byte rpm_port = RPM_PORT;
float rpm_multipl = RPM_MULTIPL;

// Speed
uint8_t speed = 0;
int speedpulse = 0;
byte speed_offset = SPEED_OFFSET;
int speedpulse_tmp[SPEEDPULSEARRAY];
byte speedpulse_port = SPEEDPULSE_PORT;
byte speed_source = SPEEDSOURCE;

// Door
byte door_port = 0;

// OIL
float oil_temp = 0;
byte oil_temp_port = 0;
float oil_pressure = 0;
byte oil_pressure_port = 0;
byte oil_press_warning = OIL_PRESSURE_WARNING;
byte oil_temp_warning = OIL_TEMP_WARNING;

// Alarm
byte alarm = 0;
unsigned long alarm_timer = 0;

// OneWire 
bool onewire_available = false;

//I2C
boolean si7021_available = false;
boolean bmp280_available = false;
boolean lm75_1_available = false;
boolean lm75_2_available = false;
boolean ht16k33_available = false;

int si7021_temp = -127;
int si7021_hum = 0;
double lm75_1_temp = 0;
double lm75_2_temp = 0;

byte i2c_led_disp_clock = 1;

// TinyGSM
String sim_apn = SIM_APN;
String sim_user = SIM_USER;
String sim_pass = SIM_PASS;
String blynk_key = BLYNK_KEY;
boolean online = false;

byte blynk_offline_counter = 0;
boolean geo_fence_enabled = false;
boolean geo_fence_armed = false;
boolean geo_fence_alarm = false;
boolean geo_fence_alarm_system = false;
boolean stay_online = false;
int geo_fence_distance = 100;
boolean blynk_geo_fence_alarmed = false;
unsigned long online_intervall_timer = 0;
boolean blynk_alarm = false;

boolean running = false;


// TIME
boolean summer_time = true;
int timezone = 1;
int16_t year;
uint8_t day;
int8_t month;
int8_t hour;
int8_t minute;
int8_t second;


//Statistics
long engine_start = 0;
//uint8_t engine_start_time_h = 0;
//uint8_t engine_start_time_min = 0;
//uint8_t engine_start_time_sec = 0;
//uint8_t engine_start_time_day = 0;
//uint8_t engine_start_time_month = 0;
//uint8_t engine_start_time_year = 0;
//uint8_t engine_stop_day = 0;
long engine_running_sec = 0;
//long engine_running = 0;
long engine_running_total = 0;
long engine_running_total_last = 0;
//long engine_running_trip = 0;
long engine_running_trip_last = 0;
//long engine_running_sec_last_all = 0;
//long engine_running_sec_last_today = 0;
boolean engine_running = false;




// Port vars
float a0_V = 0;
float a1_V = 0;
float a2_V = 0;
float a3_V = 0;
int a4_int = 0;
int a5_int = 0;
float a4_hz = 0;
float a5_hz = 0;
unsigned long a4_time = 0;
unsigned long a5_time = 0;

float a0_tmp[IO_ARRAY];
float a1_tmp[IO_ARRAY];
float a2_tmp[IO_ARRAY];
float a3_tmp[IO_ARRAY];



 /*
 * Display
 */
#ifdef U8G2_DISPLAY

#include <U8g2lib.h>

unsigned long display_update_timer = 0;
boolean display_update_timer_lock = false;

int MainMenuPos = 0;
int MenuValuesPos = 0;
int MenuConfigPos = 0;
boolean MenuEdit = false;

byte clock_view = CLOCK_VIEW;

//void display_init();
//void display_draw();

#endif // U8G2_DISPLAY


/*
 * RTC
 */
#include <RTCZero.h>
RTCZero rtc;
//RTC_Millis rtc;


/*
 * SD Card configuration
 */
#ifdef SDCARD
//#define cardSelect 4
#define KMLLOG    //???

unsigned long log_timer = 0;
char filename[16];
boolean SDmount = false;
boolean SDerror = false;
int lastfile = 0;
byte lastfile_config = 0;


#endif //SDCARD

/*
 * FONA library
 */

int8_t gps_fixstatus;
int gps_year;
int gps_day;
int gps_month;
int gps_hour;
int gps_minute;
int gps_second;
//float gps_date;
float gps_speed;
uint8_t gps_speed_max = 0;
uint8_t gps_speed_max_trip = 0;
uint8_t gps_speed_avg = 0;
uint8_t gps_speed_avg_trip = 0;
//uint8_t gps_speed_avg_start = 0;
//uint8_t gps_speed_avg_today = 0;
uint16_t gps_course;
int gps_altitude;
int gps_view_satellites;
int gps_used_satellites;
int gps_altitude_blynk;
int gps_view_satellites_blynk;
int gps_used_satellites_blynk;
float gps_latitude, gps_longitude;
float gps_latitude_old = 0;
float gps_longitude_old = 0;
//float gps_latitude_lasttrack = 0;
//float gps_longitude_lasttrack = 0;
float gps_latitude_lastlog = 0;
float gps_longitude_lastlog = 0;
// for distance check in blynk function
float gps_latitude_blynk = 0;
float gps_longitude_blynk = 0;
//float gps_latitude_blynk_geo_fence = 0;
//float gps_longitude_blynk_geo_fence = 0;
float gps_latitude_geo_fence = 0;
float gps_longitude_geo_fence = 0;
boolean gps_success = false;
boolean gps_fix = false;
uint32_t gps_distance_trip = 0;
uint32_t gps_distance = 0;
//uint32_t gps_distance_start = 0;
//uint32_t gps_distance_today = 0;

boolean gprs_tracking = false;


#ifdef FONA



//uint8_t fona_type;
//char fona_time[23];
//uint16_t fona_batt = 0;

#endif // FONA


/*
 * DS18B20 Temperatursensor
 */
//#ifdef ONEWIRE

//#ifdef DS18B20_AS_IN
//#define TEMPERATURE_IN
//#endif // DS18B20_AS_IN
//#ifdef DS18B20_AS_OUT
//#define TEMPERATURE_OUT
//#endif // DS18B20_AS_OUT

//#endif // ONEWIRE


/*
 * I2C Sensor
 */
#ifdef I2C

#endif // I2C

/*
 * Analog Ports
 */
#ifdef VW_WATER_TEMP
//int water_temp_V = 0; // Voltage of port
//int water_temp_ohm = 0;
//int water_temp = 0;
#endif // VW_WATER_TEMP

/*
 *  Input/Output
 */

unsigned long button_timer = 0;
bool button_timer_lock = false;

uint8_t button_1 = 0;
uint8_t button_1_low = 0;
uint8_t button_1_high = 0;
uint8_t button_1_double = 0;


unsigned long update_vars_timer = 0;

/*
 * Status Checker
 */
unsigned long status_checker_timer = 0;
boolean alarm_system_armed = false;
boolean alarm_system_triggered = false;


/*
 * Pointer to the programm variables and to the config variables
 */

#define MAX_PORTS 10
#define MIN_CONFIG 1
#define DEFAULT_STEPS 0 // if "0" -> menue is off
struct struct_port_config {
  char name[20];
  char desc[25];
  byte *port;
  byte steps;
  byte max;
  byte min;
};

const struct_port_config port_config[] = {
  {"bord_voltage_port", "Bord Voltage Port", &bord_voltage_port, DEFAULT_STEPS, MAX_PORTS, MIN_CONFIG},
  {"dimmer_port", "Dimmer Port", &dimmer_port, DEFAULT_STEPS, MAX_PORTS, MIN_CONFIG},
  {"fuel_port", "Fuel Gauge Port", &fuel_port, DEFAULT_STEPS, MAX_PORTS, MIN_CONFIG},
  {"water_temp_port", "Water Gauge Port", &water_temp_port, DEFAULT_STEPS, MAX_PORTS, MIN_CONFIG},
  {"rpm_port", "RPM Port", &rpm_port, DEFAULT_STEPS, MAX_PORTS, MIN_CONFIG},
//  {"rpm_multipl", "RPM Multiplikator", &rpm_multipl, DEFAULT_STEPS, 255, 1},
  {"speedpulse_port", "GALA Port", &speedpulse_port, DEFAULT_STEPS, MAX_PORTS, MIN_CONFIG},
  {"speed_source", "Speed Source", &speed_source, DEFAULT_STEPS, MAX_PORTS, MIN_CONFIG},
  {"door_port", "Door Port", &door_port, DEFAULT_STEPS, MAX_PORTS, MIN_CONFIG},
  {"oil_temp_port", "Oil Temp. Port", &oil_temp_port, DEFAULT_STEPS, MAX_PORTS, MIN_CONFIG},
  {"oil_pressure_port", "Oil Press. Port", &oil_pressure_port, DEFAULT_STEPS, MAX_PORTS, MIN_CONFIG},
  {"i2c_led_disp_clock", "LED Clock", &i2c_led_disp_clock, DEFAULT_STEPS, MAX_PORTS, MIN_CONFIG},
  {"lastfile_config", "Last Log File (x10)", &lastfile_config, 1, 99, 1},
  {"dimmer_max", "Dimmer Max.", &dimmer_max, 10, 100},
  {"dimmer_min", "Dimmer Min.", &dimmer_min, 10, 100},
  {"clock_view", "Clock and Temp.", &clock_view, 1, 1, 0},
  {"speed_offset", "Speed Offset", &speed_offset, 1, 25, 0},
  {"water_temp_warning", "Water Temp Warning", &water_temp_warning, 5, 130, 80},
  {"oil_temp_warning", "Oil Temp Warning", &oil_temp_warning, 5, 150, 80},
  {"oil_press_warning", "Oil Press. Warning (/10)", &oil_press_warning, 1, 10, 1}
};




struct struct_values {
  char name[20];
  char desc[20];
  float *value;
  byte digits;
  char suffix[6];
  boolean show_off; // show this if engine is stopped
  byte *port;
};

const struct_values values[] {
  {"bord_voltage", "Bordspannung", &bord_voltage, 1, "V", false, &bord_voltage_port},
  {"fuel_l", "Tank", &fuel_l, 0, "l", false, &fuel_port}, 
  {"water_temp", "Kuehlwasser", &water_temp, 0, "\xb0 C", false, &water_temp_port},
  {"rpm", "RPM", &rpm, 0, "U/min", false, &rpm_port},
  {"oil_temp", "Oel Temp.", &oil_temp, 0, "\xb0 C", false, &oil_temp_port},
  {"oil_pressure", "Oeldruck", &oil_pressure, 1, "bar", false, &oil_pressure_port}
};


/*
 * saving the configuration at the flash
 */

const PROGMEM uint16_t config_in_flash[]  = { SPEEDSOURCE, SPEEDPULSE_PORT, RPM_PORT, RPM_MULTIPL};
const float RPM_MULTIP_in_flash[] PROGMEM = { RPM_MULTIPL };

//const PROGMEM uint16_t SPEEDSOURCE_in_flash  = SPEEDSOURCE;


/*
 * converting a normal time into unix time
 * based on http://christianscode.blogspot.de/2015/07/arduino-timestamp-function.html
 *
 * Function extracted from the library http://playground.arduino.cc/Code/Time
 * if you just need the function without compiling few kbs and you need to save 
 * space, this is the way to go
 * 
 */

#define SECS_PER_MIN  (60UL)
#define SECS_PER_HOUR (3600UL)
#define SECS_PER_DAY  (SECS_PER_HOUR * 24UL)

// leap year calulator expects year argument as years offset from 1970
#define LEAP_YEAR(Y)     ( ((1970+Y)>0) && !((1970+Y)%4) && ( ((1970+Y)%100) || !((1970+Y)%400) ) )
static  const uint8_t monthDays[]={31,28,31,30,31,30,31,31,30,31,30,31}; // API starts months from 1, this array starts from 0


/*
 * Print the actual Status on the serial interface
 */

#ifdef PRINT_STATUS

unsigned long print_status_timer = 0;
#endif // PRINT_STATUS


