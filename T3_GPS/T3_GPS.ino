#include <OneWire.h>
// DS18S20 Temperature chip i/o
OneWire ds(10);  // on pin 10





// include configuration file
#include "config.h"


#include <Arduino.h>
#include <Adafruit_SleepyDog.h>
#include <stdarg.h>
#include <stdio.h>


uint8_t lcd_led_max = 200;
String bootmsg;
String bootmsg1;
String bootmsg2;
String bootmsg3;


float board_voltage = 0;

uint8_t button_1 = 0;
uint8_t button_1_low = 0;
uint8_t button_1_high = 0;
uint8_t button_1_double = 0;
boolean running = false;


//#include <Wire.h>
//uint8_t i2c_register = 0;


/*
   Task Sheduler
*/
#define _TASK_TIMECRITICAL
#define _TASK_SLEEP_ON_IDLE_RUN
#include <TaskScheduler.h>

#ifdef LCD
//void display_task_callback();
//Task display_task(200, TASK_FOREVER, &display_task_callback);
#endif
//void input_task_callback();
//Task input_task(100, TASK_FOREVER, &input_task_callback);

#ifdef FONA

//void gps_task_callback();
//Task gps_task(1000, TASK_FOREVER, &gps_task_callback);
//void sms_task_callback();
//Task sms_task(10000, TASK_FOREVER, &sms_task_callback);
//void battcheck_task_callback();
//Task battcheck_task(60000, TASK_FOREVER, &battcheck_task_callback);
#endif

//void status_task_callback();
//Task status_task(5000, TASK_FOREVER, &status_task_callback);

//Scheduler runner;

// FONA variablen
uint8_t type;

String str_mode;
String str_fixstatus;
String str_utctime;
String str_latitude;
String str_longitude;
String str_altitude;
String str_speed;
String str_course;
String str_view_satellites;
String str_used_satellites;

String str_year;
String str_month;
String str_day;
String str_hour;
String str_minute;
String str_second;

    
char utc_time[15];

int16_t year;
uint8_t day;
int8_t month;
int8_t hour;
int8_t minute;
int8_t second;

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

char fona_time[23];

uint8_t engine_start_time_h = 0;
uint8_t engine_start_time_min = 0;
uint8_t engine_start_time_sec = 0;
uint8_t engine_start_time_day = 0;
uint8_t engine_start_time_month = 0;
uint8_t engine_start_time_year = 0;
uint8_t engine_stop_day = 0;
long engine_running_sec = 0;
long engine_running_total = 0;
long engine_running_total_last = 0;
//long engine_running_trip = 0;
long engine_running_trip_last = 0;
//long engine_running_sec_last_all = 0;
//long engine_running_sec_last_today = 0;
boolean engine_running = false;

boolean gprs_tracking = false;
uint32_t gprs_lasttrack = 0; //???

uint16_t vbat = 0;





// Global Variables
int16_t temp_out = 0;

// this is a large buffer for replies
char replybuffer[255];



//unsigned long currentTime;
//unsigned long oneTime;
//unsigned long speedTimer;


boolean serial_export = true;
boolean SPI_lock = false;



void setup() {

  int i;
  uint32_t tmp = 0;
  uint32_t tmp2 = 0;
  uint32_t old_val = 0;

  /*
   * LCD Display
   */
  #ifdef LCD
  init_display();
  //bootmsg = F("Booting...");
  //display_update();
  #endif

  display_bootmsg(F("Booting..."));
  
  /*
   * IO
   */
  #ifdef FeatherLED8
  pinMode(FeatherLED8, OUTPUT);
  digitalWrite(FeatherLED8, LOW);
  #endif //FeatherLED8
  
  digitalWrite(X_Kontakt, HIGH);
  digitalWrite(DIMMER, HIGH);
  pinMode(BUTTON_PIN_1, INPUT_PULLUP);

  if (digitalRead(BUTTON_PIN_1) == LOW) {
    display_bootmsg(F("Waiting for Serial"));
    //bootmsg = "Waiting for Serial";
    //display_update();
    
    digitalWrite(8, HIGH);
    //digitalWrite(13, HIGH);
    while (! Serial);
    Serial.println( F("#Serial export is active"));
    
    serial_export = true;

    #ifdef SDCARD
    display_bootmsg(F("Hold Button to dump"));
    //bootmsg = "Hold Button to dump";
    //display_update();

    Serial.println( F("#Hold Button to dump data"));
    
    delay(3000);

    if (digitalRead(BUTTON_PIN_1) == LOW) {
      dump_sd_card();
    }
    #endif //SDCARD
  }

  Serial.begin(250000);
  Serial.print( F("#Compiled: "));
  Serial.print( F(__DATE__));
  Serial.print( F(", "));
  Serial.print( F(__TIME__));
  Serial.print( F(", "));
  Serial.println( F(__VERSION__));

  /*
   * FONA Libary
   */
  #ifdef FONA
  fona_init();
  #endif


  /*
     Interrupt
  */
  //pinMode(5, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);
  //attachInterrupt(5, speed_interrupt, RISING);


  /*
   *  SD Card
   */
  #ifdef SDCARD
  enable_sdcard();
  get_last_log();
  #endif //SDCARD

  delay(3000);

  /*
   *  Timer
   *  https://github.com/maxbader/arduino_tools/blob/master/libraries/timer_m0_tc_interrupt/timer_m0_tc_interrupt.ino
   */

  #ifdef ARDUINO_ARCH_SAMD
  // Enable clock for TC
  REG_GCLK_CLKCTRL = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID ( GCM_TCC2_TC3 ) ) ;
  while ( GCLK->STATUS.bit.SYNCBUSY == 1 ); // wait for sync
  // The type cast must fit with the selected timer mode
  TcCount16* TC = (TcCount16*) TC3; // get timer struct
  TC->CTRLA.reg &= ~TC_CTRLA_ENABLE;   // Disable TCx
  while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync
  //TC->CTRLA.reg |= TC_CTRLA_MODE_COUNT16;  // Set Timer counter Mode to 16 bits
  TC->CTRLA.reg |= TC_CTRLA_MODE_COUNT8;  // Set Timer counter Mode to 16 bits
  while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync
  TC->CTRLA.reg |= TC_CTRLA_WAVEGEN_NFRQ; // Set TC as normal Normal Frq
  while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync
  TC->CTRLA.reg |= TC_CTRLA_PRESCALER_DIV1024;   // Set perscaler
  while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync
  // TC->PER.reg = 0xFF;   // Set counter Top using the PER register but the 16/32 bit timer counts allway to max
  // while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync
  TC->CC[0].reg = 0xFFF;
  while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync
  // Interrupts
  TC->INTENSET.reg = 0;              // disable all interrupts
  TC->INTENSET.bit.OVF = 1;          // enable overfollow
  //TC->INTENSET.bit.MC0 = 1;          // enable compare match to CC0
  // Enable InterruptVector
  NVIC_EnableIRQ(TC3_IRQn);
  // Enable TC
  TC->CTRLA.reg |= TC_CTRLA_ENABLE;
  while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync
  #endif

  
  /*
   *  Initialized scheduler
   */
  //Serial.println("#Initialized scheduler");
  //runner.init();

  #ifdef LCD
  //runner.addTask(display_task);
  //display_task.enable();
  #endif
  //runner.addTask(input_task);
  //input_task.enable();
  #ifdef FONA
  //runner.addTask(gps_task);
  //gps_task.enable();
  //runner.addTask(sms_task);
  //sms_task.enable();
  //runner.addTask(battcheck_task);
  //battcheck_task.enable();
  #endif
  //runner.addTask(status_task);
  //status_task.enable();


  /*
   * Watchdog
   */
  #ifdef WD
  Serial.println("#Activate Watchdog");
  int countdownMS = Watchdog.enable();
  Serial.print("#Enabled the watchdog with max countdown of ");
  Serial.print(countdownMS, DEC);
  Serial.println(" milliseconds!");
  #endif
  
  #ifdef LCD  
  MainMenuPos = 1;
  #endif //LCD

  display_bootmsg(F("Ready"));
  //Serial.println("#Ready!");


}

void loop() {

  //float latitude, longitude, speed_kph, heading, speed_mph, altitude;
  //char gpsdata[120];
  //char buffer[23];
  //boolean success = false;

  int val = 0;


  //resetting Watchdog
  Watchdog.reset();

  interrupts();
  //runner.execute();


  //go to custom loops
  #ifdef FONA
  fona_loop();
  #endif

  #ifdef LCD
  //display_loop();
  #endif //LCD

  #ifdef DS18B20
  ds18b20_loop();
  #endif


  //Serial.print("+");

  val = analogRead(X_Kontakt);
  board_voltage = val * 0.0152;
  if (board_voltage > 3) {
    if (!running) {
      #ifdef LCD
      MainMenuPos = 2;
      #endif
      running = true;
      start_engine();
    }
    #ifdef FONA
    if (alarm_active) {
      if (!alarmtatus_send) {
        alarmtatus_send = true;
    #ifdef SMS_Commands
        fona_sendSMS_Alarm();
    #endif
      }
    }
    #endif
  }
  else {
    if (running) {
      #ifdef LCD
      MainMenuPos = 1;
      #endif //LCD
      running = false;
      stop_engine();
    }
    alarmtatus_send = false;
  }



  /*byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];

  ds.reset_search();
  if ( !ds.search(addr)) {
      Serial.print("No more addresses.\n");
      ds.reset_search();
      return;
  }

  Serial.print("R=");
  for( i = 0; i < 8; i++) {
    Serial.print(addr[i], HEX);
    Serial.print(" ");
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
      Serial.print("CRC is not valid!\n");
      return;
  }

  if ( addr[0] == 0x10) {
      Serial.print("Device is a DS18S20 family device.\n");
  }
  else if ( addr[0] == 0x28) {
      Serial.print("Device is a DS18B20 family device.\n");
  }
  else {
      Serial.print("Device family is not recognized: 0x");
      Serial.println(addr[0],HEX);
      return;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1);         // start conversion, with parasite power on at the end

  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.

  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  Serial.print("P=");
  Serial.print(present,HEX);
  Serial.print(" ");
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.print(" CRC=");
  Serial.print( OneWire::crc8( data, 8), HEX);
  Serial.println();*/



    
}

boolean summertime_EU(int year, byte month, byte day, byte hour, byte tzHours)
// European Daylight Savings Time calculation by "jurs" for German Arduino Forum
// input parameters: "normal time" for year, month, day, hour and tzHours (0=UTC, 1=MEZ)
// return value: returns true during Daylight Saving Time, false otherwise
{
  if (month < 3 || month > 10) return false; // keine Sommerzeit in Jan, Feb, Nov, Dez
  if (month > 3 && month < 10) return true; // Sommerzeit in Apr, Mai, Jun, Jul, Aug, Sep
  if (month == 3 && (hour + 24 * day) >= (1 + tzHours + 24 * (31 - (5 * year / 4 + 4) % 7)) || month == 10 && (hour + 24 * day) < (1 + tzHours + 24 * (31 - (5 * year / 4 + 1) % 7)))
    return true;
  else
    return false;
}

/*
   caclulating distance
*/
float get_distance(float latitude1, float longitude1, float latitude2, float longitude2) {
  float delLat = (latitude2 - latitude1) * 111194.9;
  float delLong = 111194.9 * (longitude2 - longitude1) * cos(radians((latitude2 + latitude1) / 2));
  float distance = sqrt(pow(delLat, 2) + pow(delLong, 2));

  return distance;
}


/*
 *   Internal Timer
 */
#ifdef ARDUINO_ARCH_SAMD
void TC3_Handler()
{
  
  TcCount16* TC = (TcCount16*) TC3; // get timer struct
  if (TC->INTFLAG.bit.OVF == 1) {  // A overflow caused the interrupt
    //Serial.print(".");
    TC->INTFLAG.bit.OVF = 1;    // writing a one clears the flag ovf flag
    
    #ifdef LCD
    display_loop();
    #endif //LCD

    button();
  }

}
#endif


// blink out an error code
/*void error(uint8_t errno) {
  while (1) {
    uint8_t i;
    for (i = 0; i < errno; i++) {
      //digitalWrite(13, HIGH);
      delay(100);
      //digitalWrite(13, LOW);
      delay(100);
    }
    for (i = errno; i < 10; i++) {
      delay(200);
    }
  }
}*/


void start_engine() {
  

  if (engine_start_time_day != gps_day) {
    //gps_distance_today = 0;
    //engine_running_sec_last_today = 0;
  }
  //gps_distance_start = 0;

  engine_running = true;
  
  engine_start_time_h = gps_hour;
  engine_start_time_min = gps_minute;
  engine_start_time_sec = gps_second;
  engine_start_time_day = gps_day;
  engine_start_time_month = gps_month;
  engine_start_time_year = gps_year;

  #ifdef SDCARD
  open_file();
  delay(1000);
  log_to_sdcard();
  #endif //SDCARD

  #ifdef LCD
  for (int i = 0; i <= lcd_led_max; i++) {
    analogWrite(LCD_LED, i);
    delay(10);
  }
  #endif //LCD

}

void stop_engine() {

  //engine_running_sec_last_all += engine_running_sec;
  //engine_running_sec_last_today += engine_running_sec;

  engine_running = false;

  engine_running_total_last = engine_running_total_last;

  #ifdef SDCARD
  close_file();
  #endif //SDCARD

  
  #ifdef LCD
  for (int i = lcd_led_max; i >= 0; i--) {
    analogWrite(LCD_LED, i);
    delay(10);
  }
  #endif

}

/*
 * check button state and debounce it
 *
 * Funktions...
 * 1 = short pressed
 * 2 = long pressed
 * 3 = long pressed release
 * 4 = long pressed repeat
 * 5 = double press?
 */
 
void button() {

  if ( button_timer < millis() ) {
    button_timer = millis() + BUTTON_TIMER;
    if ( !button_timer_lock ) {
      button_timer_lock = true;

      if (digitalRead(BUTTON_PIN_1) == LOW) {
        digitalWrite(8, HIGH);
        button_1_low++;
        button_1_high = 0;
        #ifdef DEBUG
        Serial.println(F("#Button pressed "));
        Serial.println(button_1_low, DEC);
        Serial.println(button_1_high, DEC);
        #endif
      }
      else {
        digitalWrite(8, LOW);
        button_1_high++;
      }
      // long press 
      if ((button_1_low == 10) && (button_1_high == 0)) {
        button_1 = 2;
        button_1_high = 0;
        #ifdef DEBUG
        Serial.println(F("#long press"));
        #endif
      }
      // long press released
      else if ((button_1_low >= 10) && (button_1_high >= 1)) {
        button_1 = 3;
        button_1_high = 0;
        button_1_low = 0;
        #ifdef DEBUG
        Serial.println(F("#long press rleased"));
        #endif
      }
      // long press repeat
      else if ((button_1_low >= 15) && (button_1_high == 0)) {
        button_1 = 4;
        button_1_high = 0;
        #ifdef DEBUG
        Serial.println(F("#long press repeat"));
        #endif
      }
      
      /*else if ((button_1_low >= 1) && (button_1_high == 1)) {
        if ( button_1_double == 1 ) {
          button_1 = 6;
          button_1_high=0;
          button_1_low=0;
          button_1_double = 0;
          Serial.println(F("double press"));
    
        }
        else {
          button_1 = 5;
          button_1_double = 1;
          Serial.println(F("double press??"));
        }
      }*/
      else if ((button_1_low >= 1) && (button_1_high >= 1)) {
        button_1 = 1;
        button_1_high = 0;
        button_1_low = 0;
        button_1_double = 0;
        //Serial.println(F("#short press"));
      }
      
      button_timer_lock = false;
    }
    else {
      Serial.println(F("#button locked..."));
    }
  }
}


#ifdef ARDUINO_ARCH_SAMD
extern "C" char *sbrk(int i);
 
int freeRam () {
  char stack_dummy = 0;
  return &stack_dummy - sbrk(0);
}
#else //ARDUINO_ARCH_SAMD
int freeRam() {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
#endif //ARDUINO_ARCH_SAMD
