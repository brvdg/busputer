/****************************************************
 * Busputer 
 * https://github.com/brvdg/busputer
 * 
 * Author: Brun
 * License: MIT 
 ****************************************************/


#define VERSION "Beta170111b"

// include configuration file
#include "config.h"


//#include <Arduino.h>
#include <Adafruit_SleepyDog.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>

#ifdef ARDUINO_ARCH_SAMD
#include <avr/dtostrf.h>
#else //ARDUINO_ARCH_SAMD
#include <stdlib.h>
#endif //ARDUINO_ARCH_SAMD


//uint8_t lcd_led_max = 200; //???

String bootmsg;
String bootmsg1;
String bootmsg2;
String bootmsg3;


float board_voltage = 0;

boolean running = false;


int16_t year;
uint8_t day;
int8_t month;
int8_t hour;
int8_t minute;
int8_t second;

#define PRINT_STATUS_TIMER 2000 //
unsigned long print_status_timer = 0;


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




// Global Variables
int16_t temp_out = 0;

// this is a large buffer for replies
char replybuffer[255];

boolean serial_export = true;
boolean SPI_lock = false;



void setup() {

  int i;
  uint32_t tmp = 0;
  uint32_t tmp2 = 0;
  uint32_t old_val = 0;

  Serial.begin(250000);
  Serial.print( F("#Compiled: "));
  Serial.print( F(__DATE__));
  Serial.print( F(", "));
  Serial.print( F(__TIME__));
  Serial.print( F(", "));
  Serial.println( F(__VERSION__));
  
  /*
   * LCD Display
   */
  #ifdef LCD
  init_display();
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
    #ifdef FeatherLED8
    digitalWrite(FeatherLED8, HIGH);
    #endif FeatherLED8
    //digitalWrite(13, HIGH);
    while (! Serial);
    INFO_PRINTLN( F("#Serial export is active"));
    
    serial_export = true;

    #ifdef SDCARD
    display_bootmsg(F("Hold Button to dump"));

    INFO_PRINTLN( F("#Hold Button to dump data"));
    
    delay(3000);

    if (digitalRead(BUTTON_PIN_1) == LOW) {
      dump_sd_card();
    }
    #endif //SDCARD
  }



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

  //delay(3000);


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

  /*
   * enable Timer
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

  //go to loops
  #ifdef FONA
  fona_loop();
  #endif

  #ifdef LCD
  //display_loop();
  #endif //LCD

  #ifdef ONEWIRE
  onewire_loop();
  #endif // ONEWIRE

  print_status();


  //Serial.print("+");

  val = analogRead(X_Kontakt);
  board_voltage = val * 0.0152;
  if (board_voltage > 3) {
    if (!running) {
      running = true;
      start_engine();
    }
  }
  else {
    if (running) {
      running = false;
      stop_engine();
    }
    #ifdef FONA
    alarmtatus_send = false;
    #endif // FONA
  }


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
    if ( lcd_update_timer < millis() ) {
      //Serial.println(F("#it's time for display update"));
      lcd_update_timer = millis() + LCD_UPDATE_TIMER;
      display_loop();
    }
    #endif //LCD

    button();
  }

}
#endif


void start_engine() {
  

  if (engine_start_time_day != day) {
    //gps_distance_today = 0;
    //engine_running_sec_last_today = 0;
  }
  //gps_distance_start = 0;

  engine_running = true;
  
  engine_start_time_h = hour;
  engine_start_time_min = minute;
  engine_start_time_sec = second;
  engine_start_time_day = day;
  engine_start_time_month = month;
  engine_start_time_year = year;

  #ifdef SDCARD
  open_file();
  delay(1000);
  log_to_sdcard();
  #endif //SDCARD

  #ifdef LCD
  for (int i = 0; i <= LCD_LED_MAX; i++) {
    analogWrite(LCD_LED, i);
    delay(10);
  }
  MainMenuPos = 2;
  #endif //LCD

  // Send a alarm SMS
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

void stop_engine() {

  //engine_running_sec_last_all += engine_running_sec;
  //engine_running_sec_last_today += engine_running_sec;

  engine_running = false;

  engine_running_total_last = engine_running_total_last;

  #ifdef SDCARD
  close_file();
  #endif //SDCARD

  
  #ifdef LCD
  for (int i = LCD_LED_MAX; i >= 0; i--) {
    analogWrite(LCD_LED, i);
    delay(10);
  }
  MainMenuPos = 1;
  #endif //LCD

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


void print_status() {
  TRACE_PRINTLN(F("#status_task_callback()"));
  char buf[16];

  if ( print_status_timer < millis() ) {
    //Serial.println(F("#it's time for gps"));
    print_status_timer = millis() + PRINT_STATUS_TIMER;


    // Serial.print(F("DATE,"));
    sprintf(buf, "%04d%02d%02d%02d%02d%02d", year, month, day, hour, minute, second);
    Serial.print(buf);
    Serial.print(F(","));


    #ifdef FONA
    Serial.print(gps_fixstatus, DEC);
    Serial.print(F(","));
    Serial.print(gps_latitude, DEC);
    Serial.print(F(","));
    Serial.print(gps_longitude, DEC);
    Serial.print(F(","));
    Serial.print(gps_altitude, DEC);
    Serial.print(F(","));
    Serial.print(gps_speed, DEC);
    Serial.print(F(","));
    Serial.print(gps_course, DEC);
    Serial.print(F(","));
    Serial.print(gps_view_satellites, DEC);
    Serial.print(F(","));
    Serial.print(gps_used_satellites, DEC);
    Serial.print(F(","));
    Serial.print(gps_distance, DEC);
    Serial.print(F(","));
    #endif //FONA


    Serial.print(F("...#freeRam:"));
    Serial.print(freeRam(), DEC);
    Serial.print(F(" bytes"));
    //Serial.println(F(" "));
    
    Serial.println(F(";"));
  }
//  fona_calculate_statistics();
  
  /*if (serial_export) {
    Serial.print(utc_time);
    Serial.print(F(","));
    Serial.print(gps_fixstatus, DEC);
    Serial.print(F(","));
    Serial.print(gps_latitude, DEC);
    Serial.print(F(","));
    Serial.print(gps_longitude, DEC);
    Serial.print(F(","));
    Serial.print(gps_altitude, DEC);
    Serial.print(F(","));
    Serial.print(gps_speed, DEC);
    Serial.print(F(","));
    Serial.print(gps_course, DEC);
    Serial.print(F(","));
    Serial.print(gps_view_satellites, DEC);
    Serial.print(F(","));
    Serial.print(gps_used_satellites, DEC);
    Serial.print(F(","));
    Serial.print(gps_distance, DEC);
    Serial.print(F(","));
    Serial.print(board_voltage, 1);
    Serial.print(F(","));
    Serial.print(engine_running_total, DEC);
    Serial.print(F("...#freeRam:"));
    Serial.print(freeRam(), DEC);
    Serial.print(F(" bytes"));
    Serial.println(F(" "));
    
  }*/
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
 * Calculating free RAM
 */
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
