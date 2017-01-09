/*
 * Busputer Configuration
 */
#define VERSION "nulnul.nix"


/*
 * Features
 */

// Enable Display
#define LCD

// Enable SD Card
#define SDCARD

// Enable Fona Libary
#define FONA

#define SMS_Keyword "PWD"
#define MyNumber "+4915141284285"
// enable Internet tracking
#define tracking
//#define tracking_ON // enable permanent tracking
#define GPRS_APN "internet.t-mobile"
#define GPRS_user "t-mobile"
#define GPRS_pw "tm"

// enable GPS
//#define GPS //???

// Dalles DS18B20 Temperatursensor
#define DS18B20


// enable Watchdog
//#define WD

// enable Debugging
#define INFO
//#define DEBUG
#define TRACE


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

//#define DEVBOARD05
//#define CLOCKONLY
//#define MINIV06
#define CUSTOMBOARD



/*
 * Custom defined configuration
 */

#ifdef CUSTOMBOARD

// LCD configuration
#define LCD_LED 11
// U8Glib

#ifdef LCD
#ifdef ARDUINO_ARCH_SAMD
#include <U8g2lib.h>
#else
#include <U8glib.h>
#endif //ARDUINO_ARCH_SAMD

#define LCD_UPDATE_TIMER 100 // 200ms
unsigned long lcd_update_timer = 0;
boolean lcd_update_timer_lock = false;

int MainMenuPos = 0;

#endif //LCD

// SD Card configuration
#define cardSelect 4
#define KMLLOG    //???
#ifdef SDCARD
//#include <SPI.h>
//#include <SD.h>
#include <SPI.h>
#include "SdFat.h"

SdFat SD;
SdFile logfile;

char filename[16];
boolean SDmount = false;
boolean SDerror = false;

#endif //SDCARD

// FONA Libarx configuration
#define SMS_Commands //???

//#ifdef FONA
#include "Adafruit_FONA.h"
#define FONA_RST 5

#ifdef ARDUINO_ARCH_SAMD
#include <avr/dtostrf.h>
HardwareSerial *fonaSerial = &Serial1;
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);
#else //ARDUINO_ARCH_SAMD
#include <stdlib.h>
#define FONA_RX 2
#define FONA_TX 3
#define FONA_RST 4
#include <SoftwareSerial.h>
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);
#endif //ARDUINO_ARCH_SAMD

//#define FONA_LOOP_TIME 10000 // 1s ???
#define FONA_GPS_TIMER 1000 // 1s
#define FONA_SMS_TIMER 5000 // 5s
#define FONA_BATT_TIMER 30000 // 30s
#define FONA_GPS_STATUS_TIMER 3000 // 3s
//Fona Variablen
unsigned long fona_gps_timer = 0;
unsigned long fona_sms_timer = 0;
unsigned long fona_batt_timer = 0;
unsigned long fona_gps_status_timer = 0;
boolean battstatus_send = false;

//SMS Alarm
boolean alarm_active = false;
char alarm_activator[32]; //number who activates the alarm
boolean alarmtatus_send = false;


/*
 * DS18B20 Temperatursensor
 */
#define DS18B20_PIN 10
#define DS18B20_TIMER 10000 // 10s
unsigned long ds18b20_timer = 0;


/*
 *  Input/Output
 */
#define BUTTON_PIN_1 6
#define BUTTON_TIMER 200 // 200ms
unsigned long button_timer = 0;
boolean button_timer_lock = false;

#define X_Kontakt A0
#define DIMMER A1

#define FeatherLED8 8



#endif //CUSTOMBOARD




/*
 * configuration for Development Board V0.5
 */
#ifdef DEVBOARD05

// Enable Display
#define LCD
#define LCD_LED 11

// Enable SD Card
#define SDCARD
#define cardSelect 4
#define KMLLOG    //disable default logging

// Enable Fona Libary
#define FONA
#define SMS_Commands
#define SMS_Keyword "G285"
#define MyNumber "+4915141284285"
#define GPRS_APN "internet.t-mobile"
#define GPRS_user "t-mobile"
#define GPRS_pw "tm"
#define tracking
//#define tracking_ON

// enable GPS
//#define GPS

// enable Watchdog
//#define WD


/*
 *  Input
 */
#define BUTTON_PIN_1 6

#define X_Kontakt A0


// enable Debugging
//#define DEBUG

#define VERSION "nulnul.nix"

// add an offset to the real speed
#define SPEED_OFFSET 5

#endif


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

