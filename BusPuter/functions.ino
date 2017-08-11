/***************************************************
 *  This scetch is used to hold all functions to keep the main sketch clean
 *
 *  Author: Brun
 *  
 ****************************************************/


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
        //#ifdef DEBUG
        TRACE_PRINTLN(F("#Button pressed "));
        //TRACE_PRINTLN(button_1_low, DEC);
        //TRACE_PRINTLN(button_1_high, DEC);
        display_update();
        //#endif
      }
      else {
        digitalWrite(8, LOW);
        button_1_high++;
      }
      // long press 
      if ((button_1_low == 10) && (button_1_high == 0)) {
        button_1 = 2;
        button_1_high = 0;
        //#ifdef DEBUG
        TRACE_PRINTLN(F("#long press"));
        //#endif
      }
      // long press released
      else if ((button_1_low >= 10) && (button_1_high >= 1)) {
        button_1 = 3;
        button_1_high = 0;
        button_1_low = 0;
        //#ifdef DEBUG
        TRACE_PRINTLN(F("#long press rleased"));
        //#endif
      }
      // long press repeat
      else if ((button_1_low >= 15) && (button_1_high == 0)) {
        button_1 = 4;
        button_1_high = 0;
        //#ifdef DEBUG
        TRACE_PRINTLN(F("#long press repeat"));
        //#endif
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
    //else {
    //  Serial.println(F("#button locked..."));
    //}
  }
}


void update_vars() {
  if ( update_vars_timer < millis() ) {
    update_vars_timer = millis() + UPDATE_VARS_TIMER;

    //speed = gps_speed;
  
    /* ??? Rebuild UNIX Time function
    if ( !I2C_lock ) {
      I2C_lock = true;
      DateTime now = rtc.now();
      I2C_lock = false;

      if ( engine_running ) {
        engine_running_sec = now.unixtime() - engine_start;
      }
    }
    */


    if ( engine_running ) {
      engine_running_sec = unixTime(rtc.getHours(), rtc.getMinutes(), rtc.getSeconds(), rtc.getDay(), rtc.getMonth(), rtc.getYear()) - engine_start;
    }
    
  } 

  
}

void print_status() {
#ifdef PRINT_STATUS
  //char buf[16];

  if ( print_status_timer < millis() ) {
    TRACE_PRINTLN(F("#print_task_callback()"));
  
    //Serial.println(F("#it's time for gps"));
    print_status_timer = millis() + PRINT_STATUS_TIMER;

    //if ( !I2C_lock ) {
      //I2C_lock = true;
      //DateTime now = rtc.now();
      //I2C_lock = false;
    
    
      // print header
      Serial.print(F("Time,"));

      //#ifdef FONA
      Serial.print(F("GPS fix,"));
      Serial.print(F("latitide,"));
      Serial.print(F("longitude,"));
      Serial.print(F("altitide,"));
      Serial.print(F("speed,"));
      Serial.print(F("course,"));
      Serial.print(F("viewed stellites,"));
      Serial.print(F("uesed satellites,"));
      Serial.print(F("GPS distance,"));
      //#endif //FONA

      //#ifdef KL15
      Serial.print(F("bord voltage,"));
      //#endif // KL15

      //#ifdef TEMPERATURE_OUT
      Serial.print(F("temperature/10 out,"));
      //#endif // TEMPERATURE_OUT
  
      //#ifdef HUMIDITY_OUT
      Serial.print(F("hunidity out,"));
      //#endif // HUMIDITY_OUT

      #ifdef TEMPERATURE_IN
      Serial.print(F("temperature/10 in,"));
      #endif // TEMPERATURE_IN

      #ifdef HUMIDITY_IN
      Serial.print(F("humidity in,"));
      #endif // HUMIDITY_OUT

      Serial.println(F(","));


      // Serial.print(F("DATE,"));
      //sprintf(buf, "%04d%02d%02d%02d%02d%02d", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());
      //Serial.print(buf);
      //Serial.print(F(","));


      //#ifdef FONA
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
      //#endif //FONA

      //#ifdef KL15
      Serial.print(bord_voltage, DEC);
      Serial.print(F(","));
      //#endif // KL15

      //#ifdef TEMPERATURE_OUT
      Serial.print(temp_out, DEC);
      Serial.print(F(","));
      //#endif // TEMPERATURE_OUT

      #ifdef HUMIDITY_OUT
      Serial.print(hum_out, DEC);
      Serial.print(F(","));
      #endif // HUMIDITY_OUT

      #ifdef TEMPERATURE_IN
      Serial.print(temp_in, DEC);
      Serial.print(F(","));
      #endif // TEMPERATURE_IN

      #ifdef HUMIDITY_IN
      Serial.print(hum_in, DEC);
      Serial.print(F(","));
      #endif // HUMIDITY_OUT
    

      Serial.print(F("...#freeRam:"));
      Serial.print(freeRam(), DEC);
      Serial.print(F(" bytes"));
      //Serial.println(F(" "));
    
      Serial.println(F(";"));
    //}
  }
//  fona_calculate_statistics();

#endif // PRINT_STATUS
}







/*
 * Starting the engine
 */
void start_engine() {
  INFO_PRINTLN(F("#--->Start Engine"));

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

  //I2C_lock = true;
  /*
  while(I2C_lock);
  DateTime now = rtc.now();
  engine_start = now.unixtime() - 3605;
  */
  //I2C_lock = false;

  engine_start = unixTime(rtc.getHours(), rtc.getMinutes(), rtc.getSeconds(), rtc.getDay(), rtc.getMonth(), rtc.getYear());

  #ifdef SIM808
  //sim808_wakeup();
  #endif
  
  #ifdef SDCARD
  open_file();
  //delay(1000);
  log_to_sdcard();
  #endif //SDCARD

  #ifdef U8G2_DISPLAY
  /*for (int i = 0; i <= LCD_LED_MAX; i++) {
    analogWrite(LCD_LED, i);
    delay(10);
  }*/
  MainMenuPos = 2;
  #endif //U8G2_DISPLAY

  

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


/*
 * Stop the engine
 */
void stop_engine() {
  INFO_PRINTLN(F("#--->Stop Engine"));

  //engine_running_sec_last_all += engine_running_sec;
  //engine_running_sec_last_today += engine_running_sec;

  engine_running = false;

  engine_running_total_last = engine_running_total_last;

  #ifdef SDCARD
  close_file();
  #endif //SDCARD

  #ifdef SIM808
  //sim808_sleep();
  #endif

  //rtc.standbyMode();
  
  #ifdef U8G2_DISPLAY
  /*for (int i = LCD_LED_MAX; i >= 0; i--) {
    analogWrite(LCD_LED, i);
    delay(10);
  }*/
  MainMenuPos = 1;
  #endif //U8G2_DISPLAY

  if ( dimmer_V > 2 ) {
    set_alarm(600,200,5, true);
  }

}


/*
 * time functions
 */

void set_time(int year, int month, int day, int hour, int minute, int second) {
  boolean secdiff = false;
  if ((second-2) >= rtc.getSeconds()) {
    secdiff = true;
  }
  else if ((second+2) <= rtc.getSeconds()) {
    secdiff = true;
  }
  if ( summer_time ) {
    if ( summertime_EU(year, month, day, hour, 1) ) {
      hour++;
    }
  }

  hour+=timezone;
  if ( hour >= 24 ) {
    hour-=24;
    day++;
  }
  
  if ( secdiff || (minute != rtc.getMinutes()) || (hour != rtc.getHours()) || (day != rtc.getDay()) || (month != rtc.getMonth()) || ((year - 2000) != rtc.getYear()) ) {
    INFO_PRINTLN(F("#--->Adjust RTC"));
    //rtc.adjust(DateTime(gps_year, gps_month, gps_day, gps_hour, gps_minute, gps_second));
    // Set the time
    rtc.setHours(hour);
    rtc.setMinutes(minute);
    rtc.setSeconds(second);

    // Set the date
    rtc.setDay(day);
    rtc.setMonth(month);
    rtc.setYear(year - 2000);
   }
}

/*
 * Summer time Yes or No
 */
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
 * converting a normal time into unix time
 * based on http://christianscode.blogspot.de/2015/07/arduino-timestamp-function.html
 *
 * Function extracted from the library http://playground.arduino.cc/Code/Time
 * if you just need the function without compiling few kbs and you need to save 
 * space, this is the way to go
 * 
 */

long unixTime(int hr,int min,int sec,int dy, int mnth, int yr){   
  // assemble time elements into time_t 
  // note year argument is offset from 1970 (see macros in time.h to convert to other formats)
  // previous version used full four digit year (or digits since 2000),i.e. 2009 was 2009 or 9
  
  // year can be given as full four digit year or two digts (2010 or 10 for 2010);  
  //it is converted to years since 1970
  if( yr > 99)
      yr = yr - 1970;
  else
      yr += 30;  

  int i;
  uint32_t seconds;

  // seconds from 1970 till 1 jan 00:00:00 of the given year
  seconds= yr*(SECS_PER_DAY * 365);
  for (i = 0; i < yr; i++) {
    if (LEAP_YEAR(i)) {
      seconds +=  SECS_PER_DAY;   // add extra days for leap years
    }
  }
  
  // add days for this year, months start from 1
  for (i = 1; i < mnth; i++) {
    if ( (i == 2) && LEAP_YEAR(yr)) { 
      seconds += SECS_PER_DAY * 29;
    } else {
      seconds += SECS_PER_DAY * monthDays[i-1];  //monthDay array starts from 0
    }
  }
  seconds+= (dy-1) * SECS_PER_DAY;
  seconds+= hr * SECS_PER_HOUR;
  seconds+= min * SECS_PER_MIN;
  seconds+= sec;
  TRACE_PRINT(F("#UnixTime: "));
  TRACE_PRINTLN(seconds);
  return (long)seconds; 
}

/*
 *  caclulating distance
 */
float get_distance(float latitude1, float longitude1, float latitude2, float longitude2) {
  float delLat = (latitude2 - latitude1) * 111194.9;
  float delLong = 111194.9 * (longitude2 - longitude1) * cos(radians((latitude2 + latitude1) / 2));
  float distance = sqrt(pow(delLat, 2) + pow(delLong, 2));

  return distance;
}


void open_config() {
#ifdef SDCARD
  sdcard_open_config();
#else
  unsigned int displayInt;
  float tmp;
  int k;    // counter variable

  DEBUG_PRINTLN(F("#reading config from FLASH..."));

  for (k = 0; k < 4; k++)
  {
    displayInt = pgm_read_word_near(config_in_flash + k);
    Serial.println(displayInt);

    delay(500);
  }

  DEBUG_PRINT(F("#SPEEDSOURCE_in_flash:"));
  //displayInt = pgm_read_word_near(SPEEDSOURCE_in_flash);
  DEBUG_PRINTLN(displayInt);
  DEBUG_PRINT(F("#RPM_MULTIP_in_flash:"));
  tmp = pgm_read_float_near(RPM_MULTIP_in_flash);
  DEBUG_PRINTLN(tmp);
  

  delay(500);
  
#endif // SDCARD
}


void save_config() {
#ifdef SDCARD
  sdcard_save_config();
#else

#endif // SDCARD
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


/*
 * Get a Value from s String splitted by a seperator
 */
String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void TC5_Handler (void) {
  //YOUR CODE HERE 
  //if(state == true) {
    //digitalWrite(LED_PIN,HIGH);
  //} else {
    //digitalWrite(LED_PIN,LOW);
  //}
  //state = !state;
  // END OF YOUR CODE

    /*if ( display_update_timer < millis() ) {
      display_update_timer = millis() + U8G2_DISPLAY_UPDATE_TIMER;
      //noInterrupts();
      display_loop();
      //interrupts();
    }*/

    
  TC5->COUNT16.INTFLAG.bit.MC0 = 1; //don't change this, it's part of the timer code
}

/* 
 *  TIMER SPECIFIC FUNCTIONS FOLLOW
 *  you shouldn't change these unless you know what you're doing
 */

//Configures the TC to generate output events at the sample frequency.
//Configures the TC in Frequency Generation mode, with an event output once
//each time the audio sample frequency period expires.
 void tcConfigure(int sampleRate)
{
 // Enable GCLK for TCC2 and TC5 (timer counter input clock)
 GCLK->CLKCTRL.reg = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID(GCM_TC4_TC5)) ;
 while (GCLK->STATUS.bit.SYNCBUSY);

 tcReset(); //reset TC5

 // Set Timer counter Mode to 16 bits
 TC5->COUNT16.CTRLA.reg |= TC_CTRLA_MODE_COUNT16;
 // Set TC5 mode as match frequency
 TC5->COUNT16.CTRLA.reg |= TC_CTRLA_WAVEGEN_MFRQ;
 //set prescaler and enable TC5
 TC5->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV1 | TC_CTRLA_ENABLE;
 //set TC5 timer counter based off of the system clock and the user defined sample rate or waveform
 TC5->COUNT16.CC[0].reg = (uint16_t) (SystemCoreClock / sampleRate - 1);
 while (tcIsSyncing());
 
 // Configure interrupt request
 NVIC_DisableIRQ(TC5_IRQn);
 NVIC_ClearPendingIRQ(TC5_IRQn);
 NVIC_SetPriority(TC5_IRQn, 0);
 NVIC_EnableIRQ(TC5_IRQn);

 // Enable the TC5 interrupt request
 TC5->COUNT16.INTENSET.bit.MC0 = 1;
 while (tcIsSyncing()); //wait until TC5 is done syncing 
} 

//Function that is used to check if TC5 is done syncing
//returns true when it is done syncing
bool tcIsSyncing()
{
  return TC5->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY;
}

//This function enables TC5 and waits for it to be ready
void tcStartCounter()
{
  TC5->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE; //set the CTRLA register
  while (tcIsSyncing()); //wait until snyc'd
}

//Reset TC5 
void tcReset()
{
  TC5->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
  while (tcIsSyncing());
  while (TC5->COUNT16.CTRLA.bit.SWRST);
}

//disable TC5
void tcDisable()
{
  TC5->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;
  while (tcIsSyncing());
}
