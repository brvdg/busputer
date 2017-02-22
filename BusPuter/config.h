/****************************************************
 * Busputer Configuration
 * for detailed informations see 
 * https://github.com/brvdg/busputer/wiki
 ****************************************************/

// Display Configuration
#define DISPLAY
//#define LCD
//#define LCD_LED_MAX 255
//#define LCD_LED_MIN 20

//#define DOGS102
//#define DOGS102_LED_MAX 255
//#define DOGS102_LED_MIN 20
#define OLED

// SD Card
#define SDCARD

// Fona Configuration
#define FONA

#define SMS_Keyword "PWD"
#define MyNumber "+4915141284285"
// enable Internet tracking
//#define tracking  // ???
//#define tracking_ON // enable permanent tracking ???
#define GPRS_APN "internet.t-mobile"
#define GPRS_user "t-mobile"
#define GPRS_pw "tm"
#define TRACKING_URL "brun.rhoencouch.de/gps.php?visor=false&latitude=%s&longitude=%s&altitude=%s&time=%s&satellites=%s&speedOTG=%s&course=%s"


// for Dalles DS18B20 Temperatursensor
#define ONEWIRE
#define DS18B20_AS_OUT
//#define DS18B20_AS_IN

// I2S Sensors
#define I2C
#define SI7021
#define SI7021_AS_IN
//#define SI7021_AS_OUT

// VW Tempsensorm
#define VW_WATER_TEMP A2

// enable Watchdog
//#define WD

// enable Debugging
#define INFO
//#define DEBUG
//#define TRACE


// add an offset to the real speed
#define SPEED_OFFSET 5


// define which funkction whil be used for default variables
// Time
#define GPS_TIME // use GPS Time as default time
//#define DS1303_TIME //??
//Speed
#define GPS_SPEED
//#define SPEEDPULE_SPEED
// Distance
#define GPS_DISTANCE
//#define SPEEDPULSE_DISTANCE



#if defined (GPS_TIME) && defined (DS1303_TIME)
//#pragma message ( "Debug configuration - OK" )
#error "Only one time source is possible"
#endif

/*
 * Hardware configuration
 */

#define CUSTOMBOARD


/*
 * Custom defined Pin configuration
 */
#ifdef CUSTOMBOARD
// DISPLAY configuration
//#define LCD_LED 11
//#define LCD_CLOCK 12
//#define LCD_DATA 13

#define DISPLAY_BG_LED 11
#define DISPLAY_CS 12
#define DISPLAY_DC 13
#define DISPLAY_RST 11


// FONA
#define FONA_RST 5

//SD Card
#define SD_CS 4

// OneWire
#define ONE_WIRE_BUS 6
//#define DS18B20


#define BUTTON_PIN_1 9
#define X_Kontakt A1
#define DIMMER A0
#define DIMMER_MAX_mV 13800
#define DIMMER_MIN_mV 7800


#define FeatherLED8 8

#endif // CUSTOMBOARD


/*
 * Display
 */
#ifdef DISPLAY

#include <U8g2lib.h>

#define DISPLAY_UPDATE_TIMER 200 // 200ms
unsigned long display_update_timer = 0;
boolean display_update_timer_lock = false;

int MainMenuPos = 0;

#endif // DISPLAY



/*
 * SD Card configuration
 */
#ifdef SDCARD
//#define cardSelect 4
#define KMLLOG    //???

//#include <SPI.h>
//#include <SD.h>
#include <SPI.h>
#include "SdFat.h"

SdFat SD;
SdFile logfile;

char filename[16];
boolean SDmount = false;
boolean SDerror = false;
uint16_t lastfile = 0;

#endif //SDCARD

/*
 * FONA library
 */

#ifdef FONA

int8_t gps_fixstatus;
int16_t gps_year;
uint8_t gps_day;
int8_t gps_month;
int8_t gps_hour;
int8_t gps_minute;
int8_t gps_second;
//float gps_date;
uint8_t gps_speed;
uint8_t gps_speed_max = 0;
uint8_t gps_speed_max_trip = 0;
uint8_t gps_speed_avg = 0;
uint8_t gps_speed_avg_trip = 0;
//uint8_t gps_speed_avg_start = 0;
//uint8_t gps_speed_avg_today = 0;
uint16_t gps_course;
uint16_t gps_altitude;
uint8_t gps_view_satellites;
uint8_t gps_used_satellites;
float gps_latitude, gps_longitude;
float gps_latitude_old = 0;
float gps_longitude_old = 0;
float gps_latitude_lasttrack = 0;
float gps_longitude_lasttrack = 0;
float gps_latitude_lastlog = 0;
float gps_longitude_lastlog = 0;
boolean gps_success = false;
boolean gps_fix = false;
uint32_t gps_distance_trip = 0;
uint32_t gps_distance = 0;
//uint32_t gps_distance_start = 0;
//uint32_t gps_distance_today = 0;

boolean gprs_tracking = false;

uint8_t fona_type;
char fona_time[23];
uint16_t fona_batt = 0;

#endif // FONA


/*
 * DS18B20 Temperatursensor
 */
#ifdef ONEWIRE

#ifdef DS18B20_AS_IN
#define TEMPERATURE_IN
#endif // DS18B20_AS_IN
#ifdef DS18B20_AS_OUT
#define TEMPERATURE_OUT
#endif // DS18B20_AS_OUT

#endif // ONEWIRE


/*
 * I2C Sensor
 */
#ifdef I2C

#endif // I2C

/*
 * Analog Ports
 */

int water_temp_V = 0; // Voltage of port
int water_temp_ohm = 0;
int water_temp = 0;

/*
 *  Input/Output
 */

#define BUTTON_TIMER 200 // 200ms
unsigned long button_timer = 0;
bool button_timer_lock = false;

uint8_t button_1 = 0;
uint8_t button_1_low = 0;
uint8_t button_1_high = 0;
uint8_t button_1_double = 0;








#ifdef INFO
 #define INFO_PRINT(x)  Serial.print (x)
 #define INFO_PRINTLN(x)  Serial.println (x)
#else
 #define INFO_PRINT(x)
 #define INFO_PRINTLN(x)
#endif
#ifdef DEBUG
 #define DEBUG_PRINT(x)  Serial.print (x)
 #define DEBUG_PRINTLN(x)  Serial.println (x)
#else
 #define DEBUG_PRINT(x)
 #define DEBUG_PRINTLN(x)
#endif
#ifdef TRACE
 #define TRACE_PRINT(x)  Serial.print (x)
 #define TRACE_PRINTLN(x)  Serial.println (x)
#else
 #define TRACE_PRINT(x)
 #define TRACE_PRINTLN(x)
#endif



/*
 * Compiler Checks
 */
#if defined (GPS_TIME) && defined (DS1303_TIME)
//#pragma message ( "Debug configuration - OK" )
#error "Only one time source is possible"
#endif

