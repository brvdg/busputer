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

#endif // U8G2_DISPLAY

// SD Card
#define SDCARD
#define SD_CS 4


//SIM808 configuration
#define SIM808
#define SIM_APN "Provider_APN"
#define SIM_USER "Provider_USER"
#define SIM_PASS "Provider_PASSWORD"
#define SMS_Keyword "geheim"
#define BLYNK_KEY "1234567890abc..."

#define BLYNK_DEVICE_NAME "TEST"

// if this is turned on
// we are only if the car is turned off
//#define ONLINE_ON_STANDBY
#define ONLINE_INTERVALL 15 // interval in min


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

// Speedpulse Port
#define SPEEDPULSE_PORT 6
#define SPEEDPULSEARRAY 25


// RPM gauge
#define RPM_PORT 5
#define RPM_MULTIPL 2

// Water Temp gauge
#define WATER_TEMP 4
#define SERIESRESISTOR 50 // Ohm of the original gauge

// Fuel gauge
#define FUEL_GAUGE 3
#define FUEL_GAUGERESISTOR 55
#define FUEL_FULL 34
#define FUEL_EMPTY 168
#define FUEL_L 60

// analog/digital input
#define A0_MULTIPLICATOR 0.0152
#define A1_MULTIPLICATOR 0.0152
//#define A2_MULTIPLICATOR 0.0152
//#define A3_MULTIPLICATOR 0.0152
#define A2_MULTIPLICATOR 0.009765
#define A3_MULTIPLICATOR 0.009765


#define BUTTON_PIN_1 9

// Alarm output
#define ALARM_OUT
#define ALARM_PORT 13
//#define ALARM_DISPLAY

// bord voltage
#define BORD_VOLTAGE_PORT 1


// LED on Feather board for running status
#define FeatherLED8 8


// for Dalles DS18B20 Temperatursensor
#define ONEWIRE
#define ONE_WIRE_BUS 6

// I2S Sensors
#define I2C
#define SI7021

// enable Debugging
//#define OFFLINE // disable some functions for programming and testing
#define INFO
#define DEBUG
//#define TRACE

// print the status on serial port
//#define PRINT_STATUS






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

#define BLYNK_VIRTUAL_alarm_modus V10
#define BLYNK_VIRTUAL_alarm_led V11
#define BLYNK_VIRTUAL_geofancy_distance V12
#define BLYNK_VIRTUAL_geofancy_led V13


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
#define IO_TIMER 100 // 100ms
#define ALARM_TIMER 100 // 100ms


//#define FONA_LOOP_TIME 10000 // 1s ???
//#define FONA_GPS_TIMER 1000 // 1s
//#define FONA_SMS_TIMER 5000 // 5s
//#define FONA_BATT_TIMER 30000 // 30s
//#define FONA_GPS_STATUS_TIMER 3000 // 3s


#define SIM808_GPS_TIMER 1000 // 1s
#define SIM808_SMS_TIMER 5000 // 5s
#define SIM808_BATT_TIMER 30000 // 30s
#define SIM808_BLYNK_TIMER 5000 // 5s
//#define SIM808_GPS_STATUS_TIMER 3000 // 3s
#define BLYNK_CHECK_TIMER 600000 // 10min




