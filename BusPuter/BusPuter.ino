/****************************************************
 * Busputer
 * https://github.com/brvdg/busputer
 *
 * Author: Brun
 * License: MIT
 ****************************************************/


#define VERSION "0.8"
#define BUILD "180219a"


#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include <avr/dtostrf.h>

// timer
//#include <Adafruit_SleepyDog.h>
//#include <Adafruit_ZeroTimer.h>
//Adafruit_ZeroTimer zt3 = Adafruit_ZeroTimer(3);

#include <SDU.h>
/*
 * Copy the .bin file to the SD card and rename
 * the file to UPDATE.bin.
 */


// Include EEPROM-like API for FlashStorage
#include <FlashAsEEPROM.h>
#include <FlashStorage.h>


// include configuration file
#include "config.h"
#include "vars.h"





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
   * Display
   */
  #ifdef U8G2_DISPLAY
  display_init();
  #endif


  display_bootmsg(F("Booting..."));
  display_bootmsg(F(BUILD));
  //delay(5000);

  /*
   * IO
   */
  #ifdef FeatherLED8
  pinMode(FeatherLED8, OUTPUT);
  digitalWrite(FeatherLED8, LOW);
  #endif //FeatherLED8

  /*
   * read config from FlashStorage
   */
  display_bootmsg(F("READ FLASH"));
  read_virtual_eeprom();


  // initialize the IO Ports
  IO_init();

  // Button
  pinMode(BUTTON_PIN_1, INPUT_PULLUP);

  // Dumping SDCard
  if (digitalRead(BUTTON_PIN_1) == LOW) {
    display_bootmsg(F("Waiting for Serial"));
    #ifdef FeatherLED8
    digitalWrite(FeatherLED8, HIGH);
    #endif FeatherLED8

    while (! Serial);
    INFO_PRINTLN( F("#Serial export is active"));

    serial_export = true;

    //#ifdef SDCARD
    display_bootmsg(F("Hold Button to dump"));

    INFO_PRINTLN( F("#Hold Button to dump data"));

    delay(3000);

    if (digitalRead(BUTTON_PIN_1) == LOW) {
      dump_sd_card();
    }
    //#endif //SDCARD
  }

  /*
   *  SD Card
   */
  //#ifdef SDCARD
  if ( enable_sd == 1 ) {
    enable_sdcard();
    //get_last_log();
    //#endif //SDCARD
    if ( SDmount ) {
      //open the configuration from SD
      sdcard_open_config();
      //Check the logs
      get_last_log();
      //delay(3000);
    }
  }
  else {
    display_bootmsg(F("SD is disabled"));
    delay(10000);
  }

  /*
   * Simcom TinyGSM
   */
  #ifdef TinyGSM
  if ( enable_tinygsm == 1 ) {
    TRACE_PRINTLN(F("#tinygsm_init()"));
    tinygsm_init();
  }
  #endif

  /*
   * OneWire Bus
   */
  #ifdef ONEWIRE
  onewire_init();
  #endif // ONEWIRE

  /*
   * I2C Sensors
   */
  #ifdef I2C
  i2c_init();
  #endif // I2C

  /*
   * decide the temp sensors
   */
  if ( lm75_1_available ) {
    temp_out_port = 1;
    INFO_PRINTLN(F("#LM75 for temp. out"));
  }
  else if ( onewire_available ) {
    temp_out_port = 2;
    INFO_PRINTLN(F("#DS18B20 for temp. out"));
  }
  else {
    INFO_PRINTLN(F("#nothing for temp. out"));
  }


  /*
   * RTC
   */
  rtc.begin();




  /*
   * enable Timer
   */
  // set time for Watchdog
  watchdog_timer = millis() + WATCHDOG_TIMER*10;

  // Enable clock for TC
  REG_GCLK_CLKCTRL = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID ( GCM_TCC2_TC3 ) ) ;
  while ( GCLK->STATUS.bit.SYNCBUSY == 1 ); // wait for sync
  // The type cast must fit with the selected timer mode
  TcCount16* TC = (TcCount16*) TC3; // get timer struct
  TC->CTRLA.reg &= ~TC_CTRLA_ENABLE;   // Disable TCx
  while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync
  TC->CTRLA.reg |= TC_CTRLA_MODE_COUNT16;  // Set Timer counter Mode to 16 bits
  //TC->CTRLA.reg |= TC_CTRLA_MODE_COUNT8;  // Set Timer counter Mode to 16 bits
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


  //
  /********************* Timer #3, 8 bit, one callback with adjustable period = 350KHz ~ 2.86us for DAC updates */
  /*zt3.configure(TC_CLOCK_PRESCALER_DIV1, // prescaler
                TC_COUNTER_SIZE_8BIT,   // bit width of timer/counter
                TC_WAVE_GENERATION_MATCH_PWM  // match style
                );

  zt3.setPeriodMatch(150, 1, 0); // ~350khz, 1 match, channel 0
  zt3.setCallback(true, TC_CALLBACK_CC_CHANNEL0, Timer3Callback0);  // set callback
  zt3.enable(true);*/

  // at least custom functions
  #ifdef CUSTOM
  custom_init();
  #endif // CUSTOM


  //read_flash();


  display_bootmsg(F("Ready"));

  set_alarm(1000,50,10, false);



  // First a normal example of using the watchdog timer.
  // Enable the watchdog by calling Watchdog.enable() as below.  This will turn
  // on the watchdog timer with a ~4 second timeout before reseting the Arduino.
  // The estimated actual milliseconds before reset (in milliseconds) is returned.
  // Make sure to reset the watchdog before the countdown expires or the Arduino
  // will reset!
  /*int countdownMS = Watchdog.enable(65000);
  Serial.print("#Enabled the watchdog with max countdown of ");
  Serial.print(countdownMS, DEC);
  Serial.println(" milliseconds!");
  Serial.println();*/


  // Watchdog
  watchdog_timer = millis() + WATCHDOG_TIMER*10;


  #ifdef U8G2_DISPLAY
  MainMenuPos = 1;
  display_update_timer_lock = true;
  display_update();
  display_update_timer_lock = false;

  #endif // LCD

}



/*
 * The main loop
 */
void loop() {

  // Reset the watchdog with every loop to make sure the sketch keeps running.
  // If you comment out this call watch what happens after about 4 iterations!
  watchdog_timer = millis() + WATCHDOG_TIMER;
  //Serial.print(F("WD: "));
  //Serial.println(watchdog_timer, DEC);

  #ifdef FeatherLED8
  digitalWrite(FeatherLED8, HIGH);
  #endif FeatherLED8

  #ifdef TinyGSM
  //if (engine_running) {
  if ( tinygsminit ) {
    tinygsm_loop();
  }
  //}
  #endif

  #ifdef FeatherLED8
  digitalWrite(FeatherLED8, LOW);
  #endif FeatherLED8


  #ifdef ONEWIRE
  onewire_loop();
  #endif // ONEWIRE

  #ifdef I2C
  i2c_loop();
  #endif // SI7021

  update_vars();

  // calculating trip informations
  trip();

  #ifdef SDCARD
  if ( engine_running ) {
    log_to_sdcard();
  }
  #endif

  #ifdef PRINT_STATUS
  print_status();
  #endif // PRINT_STATUS



  IO_loop();

  #ifdef U8G2_DISPLAY
  if ( display_update_timer < millis() ) {
    display_update_timer = millis() + U8G2_DISPLAY_UPDATE_TIMER;
    display_loop();
  }
  #endif // U8G2_DISPLAY

  button();

  // Status Checker
  status_checker();

  alarm = 1;
  //make_alarm();
  alarm_loop();

  // at least custom functions
  #ifdef CUSTOM
  custom_loop();
  #endif // CUSTOM


  delay(100);
}


/*
 *   Internal Timer
 */
void TC3_Handler()
{

  TcCount16* TC = (TcCount16*) TC3; // get timer struct
  if (TC->INTFLAG.bit.OVF == 1) {  // A overflow caused the interrupt
    //Serial.print(".");
    TC->INTFLAG.bit.OVF = 1;    // writing a one clears the flag ovf flag

    if ( alarm_timer < millis() ) {
      alarm_timer = millis() + ALARM_TIMER;
      alarm_loop();

    }

    // Wagchdog
    if ( watchdog_timer < millis() ) {
        // reset the system
        Serial.println(F("#Watchdog reset"));
        //wdreset PROGMEM = true;
        flash_watchdog_reset.write(true);

        NVIC_SystemReset();
    }

    button();
  }

}
