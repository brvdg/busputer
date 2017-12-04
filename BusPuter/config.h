/****************************************************
   Busputer Configuration
   for detailed informations see
   https://github.com/brvdg/busputer/wiki
 ****************************************************/

/*
   Hardware configuration
*/

// Display Configuration
#define U8G2_DISPLAY
//#define DOGS102
#define OLED
//#define NOKIA

// Port defination
#ifdef U8G2_DISPLAY
//#define U8G2_DISPLAY_BG_LED 10 //turn this off if a OLED ist used
#define U8G2_DISPLAY_CS 11
#define U8G2_DISPLAY_DC 12
#define U8G2_DISPLAY_RST 10

// defination of LCD dimmer
#define DIMMER 2 //Port for the cluster dimmer source
#define DIMMER_MAX_mV 13800
#define DIMMER_MIN_mV 4000
#define DIMMER_MAX 150
#define DIMMER_MIN 20

#define CLOCK_VIEW 1 // 1 = clock and temperature / 0 = clock and date

#endif // U8G2_DISPLAY

// SD Card
#define SDCARD
#define SD_CS 4


//TinyGSM configuration
#define TinyGSM
#define SIM_APN "Provider_APN"
#define SIM_USER "Provider_USER"
#define SIM_PASS "Provider_PASSWORD"
#define SMS_Keyword "geheim"
#define BLYNK_KEY "1234567890abc..."

#define MYNUMBER "+4917123456789"

#define BLYNK_DEVICE_NAME "MyCar"

// if this is turned on
// we are online if the car is turned off
#define ONLINE_ON_STANDBY
#define ONLINE_INTERVALL 15 // interval in min
//#define GEO_FENCE_DISTANCE 50 // im m


/*
    Temperature source
    0 = disabled / auto
    1 = OneWire
    2 = SI7021
*/
#define TEMP_OUT_PORT 0

/*
   Speed source
   1 = Speedpulse
   2 = GPS
*/
#define SPEEDSOURCE 2
#define SPEED_OFFSET 0

// Speedpulse Port
#define SPEEDPULSE_PORT 6
#define SPEEDPULSEARRAY 25


// RPM gauge
#define RPM_PORT 5
#define RPM_MULTIPL 2

// Water Temp gauge
#define WATER_TEMP 4
#define SERIESRESISTOR 50 // Ohm of the original gauge
#define WATER_TEMP_WARNING 110

// Fuel gauge
#define FUEL_GAUGE_PORT 3
#define FUEL_GAUGERESISTOR 55
#define FUEL_FULL 34
#define FUEL_EMPTY 168
#define FUEL_L 60
#define FUEL_WARNING 15


// Alarm output
#define ALARM_OUT
#define ALARM_PORT 13
//#define ALARM_DISPLAY

// bord voltage
#define BORD_VOLTAGE_PORT 1

// OIL
#define OIL_TEMP_PORT 0
#define OIL_PRESSURE_PORT 0
#define OIL_PRESSURE_WARNING 2
#define OIL_TEMP_WARNING 130


// LED on Feather board for running status
#define FeatherLED8 8

// Port for Button input
#define BUTTON_PIN_1 9

// for Dalles DS18B20 Temperatursensor
#define ONEWIRE
#define ONE_WIRE_BUS 6

// I2S Sensors
#define I2C
#define SI7021

/*
 * analog input multiplicator
 * A0/A1 range 0-15.6V
 * 15.6 / 1024 = 0.015234
 * A2/A3 range 0-10V 
 * 10 / 1024 = 0.009765
 */
#define A0_MULTIPLICATOR 0.015234
#define A1_MULTIPLICATOR 0.015234
#define A2_MULTIPLICATOR 0.009765
#define A3_MULTIPLICATOR 0.009765

// Number of values for an average
#define IO_ARRAY 25


// enable Debugging
//#define OFFLINE // disable some functions for programming and testing
#define INFO
#define DEBUG
//#define TRACE

// print the status on serial port
    #define PRINT_STATUS






/*
 * Blynk virtual Ports defination
 */

#define BLYNK_VIRTUAL_terminal V0
#define BLYNK_VIRTUAL_map V1
#define BLYNK_VIRTUAL_gps_used_satellites V2
#define BLYNK_VIRTUAL_gps_view_satellites V3
#define BLYNK_VIRTUAL_gps_latitude V4
#define BLYNK_VIRTUAL_gps_longitude V5
#define BLYNK_VIRTUAL_gps_altitude V6
#define BLYNK_VIRTUAL_bord_voltage V7
#define BLYNK_VIRTUAL_stay_online V8
#define BLYNK_VIRTUAL_online_LED V9
#define BLYNK_VIRTUAL_geoalarm_modus V10
#define BLYNK_VIRTUAL_geo_fence_armed_led V11
#define BLYNK_VIRTUAL_geo_fence_distance V12
#define BLYNK_VIRTUAL_geo_fence_led V13
#define BLYNK_VIRTUAL_alarm V14
#define BLYNK_VIRTUAL_armed_led V15
#define BLYNK_VIRTUAL_alarm_led V16


#if defined (DOGS102) && defined (OLED)
#pragma message ( "Only one display can be configured" )
#error "Only one display can be configured"
#endif

#if defined (U8G2_DISPLAY_BG_LED) && defined (OLED)
#pragma message ( "U8G2_DISPLAY_BG_LED and OLDE can't used together" )
#error "U8G2_DISPLAY_BG_LED and OLED can't used together"
#endif

/*
   Timer for the seperate Tasks
*/

#define U8G2_DISPLAY_UPDATE_TIMER 40 // 200ms
#define PRINT_STATUS_TIMER 2000 //
#define UPDATE_VARS_TIMER 200 // 200ms
#define BUTTON_TIMER 200 // 200ms
#define I2C_TIMER 200 // 200ms
#define ONEWIRE_TIMER 500 // 5s
#define ANALOG_TIMER 1000 // 1000ms
#define IO_TIMER 200 // 200ms
#define ALARM_TIMER 100 // 100ms
#define LOG_TIMER 1000 // 1s
#define STATUS_CHECKER_TIMER 200 // 200ms


#define TinyGSM_GPS_TIMER 1000 // 1s
#define TinyGSM_SMS_TIMER 5000 // 5s
#define TinyGSM_BATT_TIMER 30000 // 30s
#define TinyGSM_BLYNK_TIMER 5000 // 5s
//#define TinyGSM_GPS_STATUS_TIMER 3000 // 3s
#define BLYNK_CHECK_TIMER 600000 // 10min




