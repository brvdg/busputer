/****************************************************
 * Busputer 
 * https://github.com/brvdg/busputer
 * 
 * Author: Brun
 * License: MIT 
 ****************************************************/


#define VERSION "Beta170129a"

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

//String bootmsg;
String bootmsg1;
String bootmsg2;
String bootmsg3;


float bord_voltage = 0;

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
int16_t temp_out = -1000;
int temp_in = -1000;
int hum_in = 0;
int hum_out = 0;

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
  delay(2000);
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

  analog_init();
  
  digitalWrite(X_Kontakt, HIGH);
  //digitalWrite(DIMMER, HIGH);
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
   * I2C Sensors
   */

  #ifdef I2C
  i2c_init();
  #endif // SI7021

  /*
   * OneWire Bus
   */
  #ifdef ONEWIRE
  onewire_init();
  #endif // ONEWIRE
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

  // at least custom functions
  #ifdef CUSTOM
  custom_init();
  #endif // CUSTOM

}



/*
 * The main loop
 */
void loop() {

  //float latitude, longitude, speed_kph, heading, speed_mph, altitude;
  //char gpsdata[120];
  //char buffer[23];
  //boolean success = false;

  //Serial.println("HALLOO LOOOP");
  //delay(1000);
  int val = 0;


  //resetting Watchdog
  #ifdef WD
  //Serial.println("#WD RESET");
  Watchdog.reset();
  #endif // WD
  
  //interrupts();

  //go to loops
  #ifdef FONA
  //Serial.println("#FONA");
  fona_loop();
  #endif

  //#ifdef LCD
  //Serial.println("#DISPLAY");
  //display_loop();
  //#endif //LCD

  #ifdef ONEWIRE
  //Serial.println("#ONE WIRE");
  onewire_loop();
  #endif // ONEWIRE

  #ifdef I2C
  //Serial.println("#SI7021");
  i2c_loop();
  #endif // SI7021

  #ifdef VW_WATER_TEMP
  //Serial.println("#VW_WATER_TEMP");
  vw_water_temp();
  #endif

  #ifdef LCD
  //Serial.println("#DISPLAY");
  display_set_led();
  #endif //LCD

  print_status();

  // at least custom functions
  #ifdef CUSTOM
  //Serial.println("#CUSTOM");
  custom_loop();
  #endif // CUSTOM


  //Serial.print("+");

  val = analogRead(X_Kontakt);
  bord_voltage = val * 0.0152;
  //Serial.print("BOARD VOLTAGE : ");
  //Serial.println(bord_voltage);
  if (bord_voltage > 3) {
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
    //alarmtatus_send = false;
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

