/***************************************************
 *  This scetch is used to hold all functions to keep the main sketch clean
 *
 *  Author: Brun
 *  
 ****************************************************/

void print_status() {
  char buf[16];

  if ( print_status_timer < millis() ) {
    TRACE_PRINTLN(F("#print_task_callback()"));
  
    //Serial.println(F("#it's time for gps"));
    print_status_timer = millis() + PRINT_STATUS_TIMER;

    
    // ptinz header
    Serial.print(F("Time,"));

    #ifdef FONA
    Serial.print(F("GPS fix,"));
    Serial.print(F("latitide,"));
    Serial.print(F("longitude,"));
    Serial.print(F("altitide,"));
    Serial.print(F("speed,"));
    Serial.print(F("course,"));
    Serial.print(F("viewed stellites,"));
    Serial.print(F("uesed satellites,"));
    Serial.print(F("GPS distance,"));
    #endif //FONA

    #ifdef X_Kontakt
    Serial.print(F("bord voltage,"));
    #endif // X_Kontakt

    #ifdef TEMPERATURE_OUT
    Serial.print(F("temperature/10 out,"));
    #endif // TEMPERATURE_OUT

    #ifdef HUMIDITY_OUT
    Serial.print(F("hunidity out,"));
    #endif // HUMIDITY_OUT

    #ifdef TEMPERATURE_IN
    Serial.print(F("temperature/10 in,"));
    #endif // TEMPERATURE_IN

    #ifdef HUMIDITY_IN
    Serial.print(F("humidity in,"));
    #endif // HUMIDITY_OUT

    Serial.println(F(","));


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

    #ifdef X_Kontakt
    Serial.print(bord_voltage, DEC);
    Serial.print(F(","));
    #endif // X_Kontakt

    #ifdef TEMPERATURE_OUT
    Serial.print(temp_out, DEC);
    Serial.print(F(","));
    #endif // TEMPERATURE_OUT

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
  }
//  fona_calculate_statistics();
  
}







/*
 * Starting the engine
 */
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
  /*for (int i = 0; i <= LCD_LED_MAX; i++) {
    analogWrite(LCD_LED, i);
    delay(10);
  }*/
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


/*
 * Stop the engine
 */
void stop_engine() {

  //engine_running_sec_last_all += engine_running_sec;
  //engine_running_sec_last_today += engine_running_sec;

  engine_running = false;

  engine_running_total_last = engine_running_total_last;

  #ifdef SDCARD
  close_file();
  #endif //SDCARD

  
  #ifdef LCD
  /*for (int i = LCD_LED_MAX; i >= 0; i--) {
    analogWrite(LCD_LED, i);
    delay(10);
  }*/
  MainMenuPos = 1;
  #endif //LCD

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
