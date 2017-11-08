/****************************************************
 * Busputer 
 * https://github.com/brvdg/busputer
 * 
 * Author: Brun
 * License: MIT 
 ****************************************************/


#define VERSION "0.8"
#define BUILD "171108a"

// include configuration file
#include "config.h"
#include "vars.h"


#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include <avr/dtostrf.h>


#include <SDU.h>


/*
 * Blynk definations
 */
#ifdef TinyGSM
// Select your modem:
//#define TINY_GSM_MODEM_SIM800
#define TINY_GSM_MODEM_SIM808
//#define TINY_GSM_MODEM_SIM900
//#define TINY_GSM_MODEM_A6
//#define TINY_GSM_MODEM_M590
#include <TinyGsmClient.h>
#include <BlynkSimpleSIM800.h>
#endif // TinyGSM




#ifdef INFO
 #define INFO_PRINT(x)  Serial.print (x)
 #define INFO_PRINTLN(x)  Serial.println (x)
 #define INFO_PRINTHEX(x)  Serial.print (x, HEX)
#else
 #define INFO_PRINT(x)
 #define INFO_PRINTLN(x)
 #define INFO_PRINTHEX(x)
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
  delay(5000);
  
  /*
   * IO
   */
  #ifdef FeatherLED8
  pinMode(FeatherLED8, OUTPUT);
  digitalWrite(FeatherLED8, LOW);
  #endif //FeatherLED8

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
   *  SD Card
   */
  #ifdef SDCARD
  enable_sdcard();
  //get_last_log();
  #endif //SDCARD

  /*
   * open the configuration
   */
  open_config();
  
  /*
   *  Check the logs
   */
  #ifdef SDCARD
  get_last_log();
  #endif //SDCARD

  /*
   * Simcom TinyGSM
   */
  #ifdef TinyGSM
  tinygsm_init();
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
    INFO_PRINTLN(F("LM75 for temp. out"));
  }
  else if ( onewire_available ) {
    temp_out_port = 2;
    INFO_PRINTLN(F("DS18B20 for temp. out"));
  }
  else {
    INFO_PRINTLN(F("nothing for temp. out"));
  }

  
  #ifdef U8G2_DISPLAY 
  MainMenuPos = 1;
  #endif // LCD

  

  /*
   * RTC
   */
  rtc.begin();


  
  
  /*
   * enable Timer
   */
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
  

  // at least custom functions
  #ifdef CUSTOM
  custom_init();
  #endif // CUSTOM


  


  display_bootmsg(F("Ready"));

  set_alarm(100,50,2, false);



}



/*
 * The main loop
 */
void loop() {

  #ifdef FeatherLED8
  digitalWrite(FeatherLED8, HIGH);
  #endif FeatherLED8
  
  #ifdef TinyGSM
  //if (engine_running) {
    tinygsm_loop();
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
    
    #ifdef U8G2_DISPLAY
    /*  if ( display_update_timer < millis() ) {
      display_update_timer = millis() + U8G2_DISPLAY_UPDATE_TIMER;
      noInterrupts();
      display_loop();
      interrupts();
    }*/
    #endif // U8G2_DISPLAY
    if ( alarm_timer < millis() ) {
      alarm_timer = millis() + ALARM_TIMER;
      alarm_loop();
   
    }
    
    button();
  }

}







