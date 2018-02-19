/***************************************************
    This scetch is used to hold all functions to keep the main sketch clean

    Author: Brun

 ****************************************************/






void trip() {

  if ( trip_timer < millis() ) {
    trip_timer = millis() + 1000;
    TRACE_PRINTLN(F("#run trip calculation "));


    if ( engine_running ) {
      trip_time = unixTime(rtc.getHours(), rtc.getMinutes(), rtc.getSeconds(), rtc.getDay(), rtc.getMonth(), rtc.getYear()) - engine_start;
      DEBUG_PRINT(F("Trip time: "));
      DEBUG_PRINTLN(trip_time);
      trip_distance = gps_distance;
      DEBUG_PRINT(F("Trip distance: "));
      DEBUG_PRINTLN(trip_distance);
    }
    else {
      trip_time = trip_time_last;
    }
  }
}


/*
   check button state and debounce it

   Funktions...
   1 = short pressed
   2 = long pressed
   3 = long pressed release
   4 = long pressed repeat
   5 = double press?
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
        display_loop();
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

    if ( engine_running ) {
      engine_running_sec = unixTime(rtc.getHours(), rtc.getMinutes(), rtc.getSeconds(), rtc.getDay(), rtc.getMonth(), rtc.getYear()) - engine_start;
    }
  }
}

void print_status() {
#ifdef PRINT_STATUS
  if ( print_status_timer < millis() ) {
    TRACE_PRINTLN(F("#print_task_callback()"));
    print_status_timer = millis() + PRINT_STATUS_TIMER;

    Serial.write(27);       // ESC command
    Serial.print("[2J");    // clear screen command
    Serial.write(27);
    Serial.print("[H");     // cursor to home command

    Serial.println(F(""));
    Serial.println(F("================"));

    Serial.print(F("DATE: "));
    sprintf(buf, "%02d:%02d:%02d", rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());
    Serial.println(buf);

    Serial.print(F("GPS Fix: "));
    Serial.println(gps_fixstatus, DEC);
    Serial.print(F("Latitude: "));
    Serial.println(gps_latitude, DEC);
    Serial.print(F("Longitude: "));
    Serial.println(gps_longitude, DEC);
    Serial.print(F("Altitude: "));
    Serial.println(gps_altitude, DEC);
    Serial.print(F("Speed: "));
    Serial.println(gps_speed, 2);
    Serial.print(F("Course: "));
    Serial.println(gps_course, DEC);
    Serial.print(F("view Satallites: "));
    Serial.println(gps_view_satellites, DEC);
    Serial.print(F("used Satellites: "));
    Serial.println(gps_used_satellites, DEC);
    Serial.print(F("Distance: "));
    Serial.println(gps_distance, DEC);

    Serial.print(F("Port 1 (V): "));
    Serial.println(a0_V, 2);
    Serial.print(F("Port 2 (V): "));
    Serial.println(a1_V, 2);
    Serial.print(F("Port 3 (V): "));
    Serial.println(a2_V, 2);
    Serial.print(F("Port 4 (V): "));
    Serial.println(a3_V, 2);
    Serial.print(F("Port 5 (Hz): "));
    Serial.println(a4_hz, 2);
    Serial.print(F("Port 6 (Hz): "));
    Serial.println(a5_hz, 2);

    Serial.print(F("Speed (km/h): "));
    Serial.println(speed, DEC);

    for (int i = 0; i <= (sizeof(values) / sizeof(values[0])) - 1; i++) {
      Serial.print(values[i].name);
      Serial.print(F(": "));
      Serial.println(*values[i].value);
    }

    Serial.println(F("================"));
  }
#endif
}


/*void print_status() {
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
  }*/







/*
   Starting the engine
*/
/*void start_engine() {
  INFO_PRINTLN(F("#--->Start Engine"));

  #ifdef U8G2_DISPLAY
  MainMenuPos = 2;
  display_update();
  #endif //U8G2_DISPLAY

  //if (engine_start_time_day != day) {
    //gps_distance_today = 0;
    //engine_running_sec_last_today = 0;
  //}
  //gps_distance_start = 0;

  engine_running = true;

  //engine_start_time_h = hour;
  //engine_start_time_min = minute;
  //engine_start_time_sec = second;
  //engine_start_time_day = day;
  //engine_start_time_month = month;
  //engine_start_time_year = year;


  engine_start = unixTime(rtc.getHours(), rtc.getMinutes(), rtc.getSeconds(), rtc.getDay(), rtc.getMonth(), rtc.getYear());

  #ifdef TinyGSM

  if ( blynk_alarm ) {
    tinygsm_alarm();
  }
  else {
    if ( tinygsm_go_offline() ) {
      online = false;
    }
  }


  #endif

  #ifdef SDCARD
  open_file();
  //delay(1000);
  log_to_sdcard();
  #endif //SDCARD



  }*/


/*
   Stop the engine
*/
/*void stop_engine() {
  INFO_PRINTLN(F("#--->Stop Engine"));

  //engine_running_sec_last_all += engine_running_sec;
  //engine_running_sec_last_today += engine_running_sec;


  #ifdef U8G2_DISPLAY
  MainMenuPos = 1;
  display_update();
  #endif //U8G2_DISPLAY


  engine_running = false;
  engine_running_total += engine_running_sec;

  //engine_running_total_last = engine_running_total_last;

  #ifdef SDCARD
  close_file();
  #endif //SDCARD


  if ( dimmer_V > 2 ) {
    set_alarm(600, 200, 5, true);
  }

  #ifdef TinyGSM
  if ( tinygsm_go_online() ) {
    online = true;
  }
  #endif

  }*/


/*
   time functions
*/

void set_time(int year, int month, int day, int hour, int minute, int second) {
  boolean secdiff = false;
  if ((second - 2) >= rtc.getSeconds()) {
    secdiff = true;
  }
  else if ((second + 2) <= rtc.getSeconds()) {
    secdiff = true;
  }
  if ( summer_time ) {
    if ( summertime_EU(year, month, day, hour, 1) ) {
      hour++;
    }
  }

  hour += timezone;
  if ( hour >= 24 ) {
    hour -= 24;
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
   Summer time Yes or No
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
   converting a normal time into unix time
   based on http://christianscode.blogspot.de/2015/07/arduino-timestamp-function.html

   Function extracted from the library http://playground.arduino.cc/Code/Time
   if you just need the function without compiling few kbs and you need to save
   space, this is the way to go

*/

long unixTime(int hr, int min, int sec, int dy, int mnth, int yr) {
  // assemble time elements into time_t
  // note year argument is offset from 1970 (see macros in time.h to convert to other formats)
  // previous version used full four digit year (or digits since 2000),i.e. 2009 was 2009 or 9

  // year can be given as full four digit year or two digts (2010 or 10 for 2010);
  //it is converted to years since 1970
  if ( yr > 99)
    yr = yr - 1970;
  else
    yr += 30;

  int i;
  uint32_t seconds;

  // seconds from 1970 till 1 jan 00:00:00 of the given year
  seconds = yr * (SECS_PER_DAY * 365);
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
      seconds += SECS_PER_DAY * monthDays[i - 1]; //monthDay array starts from 0
    }
  }
  seconds += (dy - 1) * SECS_PER_DAY;
  seconds += hr * SECS_PER_HOUR;
  seconds += min * SECS_PER_MIN;
  seconds += sec;
  TRACE_PRINT(F("#UnixTime: "));
  TRACE_PRINTLN(seconds);
  return (long)seconds;
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


void open_config() {
#ifdef SDCARD

  read_virtual_eeprom();

  if ( enable_sd == 1 ) {
    sdcard_open_config();
  }
  else {
    display_bootmsg(F("SD is disabled"));
  }


  char PROGMEM sim_apn_flash[] = {SIM_APN};

  for (int k = 0; k < strlen_P(sim_apn_flash); k++)
  {
    char myChar =  pgm_read_byte_near(sim_apn_flash + k);
    Serial.print(myChar);
  }

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

  for (k = 0; k < strlen_P(sim_apn_flash); k++)
  {
    myChar =  pgm_read_byte_near(sim_apn_flash + k);
    Serial.print(myChar);
  }


  delay(500);

#endif // SDCARD
}


void save_config() {
//#ifdef SDCARD
  sdcard_save_config();
  write_virtual_eeprom();

  //char tmp[24];
  //sim_apn.toCharArray(tmp, sim_apn.length());
  //char tmp[] = toCharArray(sim_apn, sim_apn.length)
  //char PROGMEM sim_apn_flash[24] = tmp;
  //char tmp2[] = toCharArray(blynk_key, 12);
  //char PROGMEM blynk_key_flash[] = tmp2;
  // BLYNK KKEY
  //for (int k = 0; k < strlen_P(blynk_key_flash); k++)
  //{
  //  blynk_key = pgm_read_byte_near(blynk_key_flash + k);
  //  Serial.println(blynk_key);
  //}

//#else

  /*for (int i = 0; i <= (sizeof(port_config) / sizeof(port_config[0])) - 1; i++){
    logfile.print(F("# "));
    logfile.println(port_config[i].desc);
    logfile.print(port_config[i].name);
    logfile.print(F("="));
    logfile.println(*port_config[i].port, DEC);


  }*/

//#endif // SDCARD
}

void read_virtual_eeprom() {
  //delay(10000);

  if (EEPROM.isValid()) {

    Serial.println("EEPROM has been written.");
    Serial.println("Here is the content of the first 20 bytes:");

    Serial.print("->");
    for (int i=0; i<20; i++) {
      Serial.print(" ");
      Serial.print(EEPROM.read(i));
    }
    Serial.println();
    for (int i = 0; i <= (sizeof(config) / sizeof(config[0])) - 1; i++){
      //Serial.println(port_config[i].name);
      //Serial.println(port_config[i].desc);
      //Serial.println(*port_config[i].port);
      /*if ( tmp.startsWith(config[i].name) ) {
        SD_DEBUG_PRINT(F("found "));
        SD_DEBUG_PRINTLN(config[i].desc);
        tmp = getValue( tmp, '=', 1 );
        *config[i].config = tmp.toInt();
        //delay(1500);
      }*/
      *config[i].config = EEPROM.read(i);
    }


    char_config = flash_char_config.read();

    sim_apn = String(char_config.sim_apn);
    sim_user = String(char_config.sim_user);
    sim_pass = String(char_config.sim_pass);
    blynk_key = String(char_config.blynk_key);


    Serial.println(char_config.sim_apn);
    Serial.println(char_config.sim_user);
    Serial.println(char_config.sim_pass);
    Serial.println(char_config.blynk_key);


    /*sim_apn = String(sim_apn_flash.read());
    sim_user = sim_user_flash.read();
    sim_pass = sim_pass_flash.read();
    blynk_key = blynk_key_flash.read();*/

    // SIM APN
    /*for (int k = 0; k < strlen_P(sim_apn_flash); k++)
    {
      sim_apn = pgm_read_byte_near(sim_apn_flash + k);
      Serial.println(sim_apn);
    }
    // BLYNK KKEY
    for (int k = 0; k < strlen_P(blynk_key_flash); k++)
    {
      blynk_key = pgm_read_byte_near(blynk_key_flash + k);
      Serial.println(blynk_key);
    }*/


  } else {
    display_bootmsg(F("config flash is empty"));
    //delay(3000);
    //write_virtual_eeprom();
  }
  //delay(30000);
}

void write_virtual_eeprom() {
  Serial.println(F("#Writing config to virtual EEPROM"));
  Serial.print("->");
  //delay(10000);
  for (int i = 0; i <= (sizeof(config) / sizeof(config[0])) - 1; i++){
    Serial.print(config[i].name);
    //Serial.println(config[i].desc);
    Serial.print(F(": "));
    Serial.println(*config[i].config);
    EEPROM.write(i, *config[i].config);

  }

  //char tmp_apn[36];
  //sim_apn.toCharArray(tmp_apn, sim_apn.length());
  //sim_apn_flash.write(tmp_apn);
  char_config = flash_char_config.read();
  sim_apn.toCharArray( char_config.sim_apn, 36);
  sim_user.toCharArray( char_config.sim_user, 12);
  sim_pass.toCharArray( char_config.sim_pass, 8);
  blynk_key.toCharArray( char_config.blynk_key, 36);


  flash_char_config.write(char_config);

  char_config = flash_char_config.read();
  Serial.println(char_config.sim_apn);
  Serial.println(char_config.sim_user);
  Serial.println(char_config.sim_pass);
  Serial.println(char_config.blynk_key);


  /*sim_user_flash.write(sim_user);
  sim_pass_flash.write(sim_pass);
  blynk_key_flash.write(blynk_key);


  Serial.println(String(sim_apn_flash.read()));
  Serial.println(sim_user_flash.read());
  Serial.println(sim_pass_flash.read());
  Serial.println(blynk_key_flash.read());*/


  /*for (int i=0; i<20; i++) {
    EEPROM.write(i, 100+i);
    Serial.print(" ");
    Serial.print(100+i);
  }
  Serial.println();*/

  // commit() saves all the changes to EEPROM, it must be called
  // every time the content of virtual EEPROM is changed to make
  // the change permanent.
  // This operation burns Flash write cycles and should not be
  // done too often. See readme for details:
  // https://github.com/cmaglie/FlashStorage#limited-number-of-writes
  EEPROM.commit();
  Serial.println("Done!");

  Serial.print(F("#isValid() returns "));
  Serial.println(EEPROM.isValid());
  //delay(10000);
}


/*
   Calculating free RAM
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
   Get a Value from s String splitted by a seperator
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
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
