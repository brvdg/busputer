#ifdef FONA

void fona_init() {
  #ifdef FONA
  #ifdef DEBUG
  Serial.println(F("#fona_init"));
  #endif
  display_bootmsg(F("Initializing FONA"));
  //bootmsg = "Initializing FONA";
  //display_update();
  
  Serial.println(F("#Initializing FONA... (May take a few seconds)"));
  fonaSerial->begin(115200);
  if (! fona.begin(*fonaSerial)) {
    //Serial.println(F("#Couldn't find FONA"));
    display_bootmsg(F("Couldn't find FONA"));
    //bootmsg = "Couldn't find FONA";
    //display_update();
    //while (1);
    delay(1000);
  }
  else {
    type = fona.type();
  
    Serial.print(F("#Found "));
    switch (type) {
      case FONA800L:
        Serial.println(F("FONA 800L")); break;
      case FONA800H:
        Serial.println(F("FONA 800H")); break;
      case FONA808_V1:
        Serial.println(F("FONA 808 (v1)")); break;
      case FONA808_V2:
        Serial.println(F("FONA 808 (v2)")); break;
      case FONA3G_A:
        Serial.println(F("FONA 3G (American)")); break;
      case FONA3G_E:
        Serial.println(F("FONA 3G (European)")); break;
      default:
        Serial.println(F("???")); break;
    }

    //Serial.println(F("#FONA is OK"));
    display_bootmsg(F("FONA is OK"));
    //bootmsg = F("FONA is OK");
    //display_update();

    //Serial.println(F("#enable GPS..."));
    display_bootmsg(F("enable GPS"));
    //bootmsg = F("Enabling GPS");
    //display_update();

    fona.enableGPS(true);
    delay(1000);
    Serial.println(F("#enable NetworkTimeSync..."));
    if (!fona.enableNetworkTimeSync(true))
      Serial.println(F("#Failed to enable"));

    //turn GPRS off
    Serial.println("#disable proactive GPRS");
    //fona_tracking_off();
    uint16_t returncode;

    if (!fona.getGSMLoc(&returncode, replybuffer, 250))
      Serial.println(F("Failed!"));
    if (returncode == 0) {
      Serial.println(replybuffer);
      fona_tracking_off();
    } else {
      Serial.print(F("#return code #")); Serial.println(returncode);
    }

    #ifdef tracking_ON
    delay(15000);
    // turn GPRS on
    Serial.println("#enable tracking");
    tracking_on();
    #endif

    // Resever String Memory
    str_mode.reserve(2);
    str_fixstatus.reserve(2);
    str_utctime.reserve(16);
    str_latitude.reserve(12);
    str_longitude.reserve(12);
    str_altitude.reserve(8);
    str_speed.reserve(8);
    str_course.reserve(6);
    str_view_satellites.reserve(4);
    str_used_satellites.reserve(4);

    str_year.reserve(6);
    str_month.reserve(4);
    str_day.reserve(4);
    str_hour.reserve(4);
    str_minute.reserve(4);
    str_second.reserve(4);
  }
#endif
}

/*
 * Main Loop for Fona
 */
void fona_loop() {

  if ( fona_gps_timer < millis() ) {
    //Serial.println(F("#it's time for gps"));
    fona_gps_timer = millis() + FONA_GPS_TIMER;

    fona_get_gps();
  }
  if ( fona_gps_status_timer < millis() ) {
    //Serial.println(F("#it's time for sms"));
    fona_gps_status_timer = millis() + FONA_GPS_STATUS_TIMER;

    fona_gps_status();
  }
  if ( fona_sms_timer < millis() ) {
    //Serial.println(F("#it's time for sms"));
    fona_sms_timer = millis() + FONA_SMS_TIMER;

    fona_checkSMS();
  }
  if ( fona_batt_timer < millis() ) {
    //Serial.println(F("#it's time for batt"));
    fona_batt_timer = millis() + FONA_BATT_TIMER;

    fona_battcheck();
  }


}


/*
 * Get GPS coordinates
 */
void fona_get_gps() {
  float latitude, longitude, speed_kph, heading, speed_mph, altitude;
  char gpsdata[120]; //??? change to default buffer
  #ifdef DEBUG
  Serial.println(F("#gps_task_callback()"));
  #endif

  if (type == FONA808_V1) {
    //Serial.println(F("Reply in format: mode,longitude,latitude,altitude,utctime(yyyymmddHHMMSS),ttff,satellites,speed,course"));
    boolean success = fona.getGPS(&latitude, &longitude, &speed_kph, &heading, &altitude);

    if (success) {
      gps_success = true;
      gps_latitude = latitude;
      gps_longitude = longitude;
      gps_altitude = altitude;
      gps_speed = speed_kph;
    }
    else {
      gps_success = false;
    }
  }
  else {
    fona.getGPS(0, gpsdata, 120);
    DEBUG_PRINTLN(F("#Reply in format: mode,fixstatus,utctime(yyyymmddHHMMSS),latitude,longitude,altitude,speed,course,fixmode,reserved1,HDOP,PDOP,VDOP,reserved2,view_satellites,used_satellites,reserved3,C/N0max,HPA,VPA"));
    DEBUG_PRINT(F("#"));
    DEBUG_PRINTLN(gpsdata);
#ifdef DEBUG
    //Serial.println(F("#Reply in format: mode,fixstatus,utctime(yyyymmddHHMMSS),latitude,longitude,altitude,speed,course,fixmode,reserved1,HDOP,PDOP,VDOP,reserved2,view_satellites,used_satellites,reserved3,C/N0max,HPA,VPA"));
    //Serial.print(F("#"));
    //Serial.println(gpsdata);
#endif

    String gpsdata_tmp = gpsdata;
    int index_mode = gpsdata_tmp.indexOf(",");
    int index_fixstatus = gpsdata_tmp.indexOf(",", index_mode+1);
    int index_utctime = gpsdata_tmp.indexOf(",", index_fixstatus+1);
    int index_latitude = gpsdata_tmp.indexOf(",", index_utctime+1);
    int index_longitude = gpsdata_tmp.indexOf(",", index_latitude+1);
    int index_altitude = gpsdata_tmp.indexOf(",", index_longitude+1);
    int index_speed = gpsdata_tmp.indexOf(",", index_altitude+1);
    int index_course = gpsdata_tmp.indexOf(",", index_speed+1);
    int index_fixmode = gpsdata_tmp.indexOf(",", index_course+1);
    int index_reserved1 = gpsdata_tmp.indexOf(",", index_fixmode+1);
    int index_HDOP = gpsdata_tmp.indexOf(",", index_reserved1+1);
    int index_PDOP = gpsdata_tmp.indexOf(",", index_HDOP+1);
    int index_VDOP = gpsdata_tmp.indexOf(",", index_PDOP+1);
    int index_reserved2 = gpsdata_tmp.indexOf(",", index_VDOP+1);
    int index_view_satellites = gpsdata_tmp.indexOf(",", index_reserved2+1);
    int index_used_satellites = gpsdata_tmp.indexOf(",", index_view_satellites+1);
    int index_reserved3 = gpsdata_tmp.indexOf(",", index_used_satellites+1);
    int index_CN0max = gpsdata_tmp.indexOf(",", index_reserved3+1);
    int index_HPA = gpsdata_tmp.indexOf(",", index_CN0max+1);
    int index_VPA = gpsdata_tmp.indexOf(",", index_HPA+1);
    
    
    str_mode = gpsdata_tmp.substring(0, index_mode);
    str_fixstatus = gpsdata_tmp.substring(index_mode+1, index_fixstatus);
    str_utctime = gpsdata_tmp.substring(index_fixstatus+1, index_utctime);
    str_latitude = gpsdata_tmp.substring(index_utctime+1, index_latitude);
    str_longitude = gpsdata_tmp.substring(index_latitude+1, index_longitude);
    str_altitude = gpsdata_tmp.substring(index_longitude+1, index_altitude);
    str_speed = gpsdata_tmp.substring(index_altitude+1, index_speed);
    str_course = gpsdata_tmp.substring(index_speed+1, index_course);
    str_view_satellites = gpsdata_tmp.substring(index_reserved2+1, index_view_satellites);
    str_used_satellites = gpsdata_tmp.substring(index_view_satellites+1, index_used_satellites);


    gps_fixstatus = str_fixstatus.toInt();
    gps_view_satellites = str_view_satellites.toInt();
    gps_used_satellites = str_used_satellites.toInt();
    
    // don't use this values if gps ist not fix
    if ( gps_fixstatus == 1 ) {
      gps_latitude = str_latitude.toFloat();
      gps_longitude = str_longitude.toFloat();
      gps_altitude = str_altitude.toInt();
      gps_speed = str_speed.toInt();
      gps_course = str_course.toInt();
    }
    
    
    str_year = str_utctime.substring(0,4);
    str_month = str_utctime.substring(4,6);
    str_day = str_utctime.substring(6,8);
    str_hour = str_utctime.substring(8,10);
    str_minute = str_utctime.substring(10,12);
    str_second = str_utctime.substring(12,14);
    str_utctime = str_utctime.substring(0,14);

    gps_year = str_year.toInt();
    gps_month = str_month.toInt();
    gps_day = str_day.toInt();
    gps_hour = str_hour.toInt();
    gps_minute = str_minute.toInt();
    gps_second = str_second.toInt();


    if (gps_hour == 23) gps_hour = 0; else gps_hour++;
    if (summertime_EU(gps_year, gps_month, gps_day, gps_hour, 1) ) {
      if (gps_hour == 23) gps_hour = 0; else gps_hour++;
    }

    #ifdef GPS_TIME
    str_utctime.toCharArray(utc_time, 15);
    year = gps_year;
    month = gps_month;
    day = gps_day;
    hour = gps_hour;
    minute = gps_minute;
    second = gps_second;
    #endif

    
    if ( gps_fixstatus == 1 ) {
      #ifdef DEBUG
      //Serial.println(F("#GPS is Fix!"));
      #endif
      gps_success = true;
      //success = true;


      if (gps_latitude_old == 0) {
        gps_latitude_old = gps_latitude;
        gps_longitude_old = gps_longitude;
      }

      float distance = get_distance(gps_latitude, gps_longitude, gps_latitude_old, gps_longitude_old);
#ifdef DEBUG
      Serial.print(F("#Distance is: "));
      Serial.println(distance, DEC);
#endif

      if (gps_speed > 10) {
        gps_distance += (int)(distance + .5);
        gps_distance_trip += (int)(distance + .5);
        //gps_distance_today += (int)(distance + .5);
        //gps_distance_start += (int)(distance + .5);
        
        gps_latitude_old = gps_latitude;
        gps_longitude_old = gps_longitude;
      }

      /*
       * Track an Log possition
       */
#ifdef tracking
      if (gprs_tracking) {
        if (get_distance(gps_latitude, gps_longitude, gps_latitude_lasttrack, gps_longitude_lasttrack) > 100 ) {
          fona_track();
          gps_latitude_lasttrack = gps_latitude;
          gps_longitude_lasttrack = gps_longitude;
        }
      }
#endif
      if (running) {
#ifdef SDCARD

        log_to_sdcard();

        if ( ( (get_distance(gps_latitude, gps_longitude, gps_latitude_lastlog, gps_longitude_lastlog) > 10) && (gps_speed > 10) ) || (get_distance(gps_latitude, gps_longitude, gps_latitude_lastlog, gps_longitude_lastlog) > 100) ) {
          //log_to_sdcard();
          
          gps_latitude_lastlog = gps_latitude;
          gps_longitude_lastlog = gps_longitude;
          
        }
#endif
      }

      /*
       * Logging format
       * Default values
       * utctime(yyyymmddHHMMSS),fixstatus,latitude,longitude,altitude,speed,course,view_satellites,used_satellites,distance,
       * Debugging informations starting with a "#"
       * optional logging which are not standart starting with two "//" ore some otehr shit. I don't know yet.
       */
      /*if (serial_export) {
        Serial.print(">");
        Serial.print(str_utctime);
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
        Serial.println(board_voltage, DEC);
      }*/
    }
    else {
      gps_success = false;
    }

    /*
     * Logging format
     * Default values
     * utctime(yyyymmddHHMMSS),fixstatus,latitude,longitude,altitude,speed,course,view_satellites,used_satellites,distance,
     * Debugging informations starting with a "#"
     * optional logging which are not standart starting with two "//" ore some otehr shit. I don't know yet.
     */
    /*if (serial_export) {
      //Serial.print(">");
      Serial.print(str_utctime);
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
      Serial.println(engine_running_sec, DEC);
    }*/
    
  }
}


/*
 * SMS
 */


void fona_checkSMS() {
  #ifdef SMS_Commands
  #ifdef FONA
  #ifdef DEBUG
  Serial.println(F("#checkSMS()"));
  #endif
  char callerIDbuffer[32];

  int8_t smsnum = fona.getNumSMS();

  #ifdef DEBUG
  Serial.print(F("#"));
  Serial.print(smsnum);
  Serial.println(F(" SMS's on SIM card!"));
  #endif

  char buffer_long[10];
  char buffer_lat[10];
  char buffer_alt[10];
  char buffer_speed[10];
  char buffer_course[3];

  uint16_t statuscode;
  int16_t length;

  if (smsnum > 0) {
    for ( uint8_t smsn = 0; smsn < 20; smsn++) {
      #ifdef DEBUG
      //uint8_t smsn = readnumber();
      //Serial.print(F("#Reading SMS #")); Serial.println(smsn);
      #endif

      // Retrieve SMS sender address/phone number.
      if (fona.getSMSSender(smsn, callerIDbuffer, 31)) {
        Serial.print(F("#SMS FROM: ")); Serial.println(callerIDbuffer);

        // Retrieve SMS value.
        uint16_t smslen;
        if (fona.readSMS(smsn, replybuffer, 250, &smslen)) { // pass in buffer and max len!

          #ifdef DEBUG
          Serial.print(F("#***** SMS #")); Serial.print(smsn);
          Serial.print(" ("); Serial.print(smslen); Serial.println(F(") bytes *****"));
          Serial.println(replybuffer);
          Serial.println(F("*****"));
          #endif

          if (strcasestr_P(replybuffer, PSTR(SMS_Keyword))) {
            if (strcasestr_P(replybuffer, PSTR("status"))) {
              sprintf_P(replybuffer, PSTR("http://maps.apple.com/?q=%s,%s&t=m\n\nLatitude:%s\nLongitude:%s\nSpeed:%s\nCourse:%s"),
                        dtostrf(gps_latitude, 5, 6, buffer_lat),
                        dtostrf(gps_longitude, 5, 6, buffer_long),
                        buffer_lat,
                        buffer_long,
                        dtostrf(gps_speed, 3, 0, buffer_speed),
                        dtostrf(gps_course, 3, 0, buffer_course));


            }
            else if (strcasestr_P(replybuffer, PSTR("alarmon"))) {
              Serial.print(F("#activating alarm for: "));
              Serial.println(callerIDbuffer);
              alarm_active = true;
              sprintf(alarm_activator, callerIDbuffer);
              sprintf(replybuffer, PSTR("Alarm ON"));
              
            }
            else if (strcasestr_P(replybuffer, PSTR("alarmoff"))) {
              alarm_active = false;
              Serial.println(F("deactivating alarm"));
              sprintf(replybuffer, PSTR("Alarm OFF"));
            }
            #ifdef tracking
            else if (strcasestr_P(replybuffer, PSTR("trackingon"))) {
              Serial.println(F("#turn Tracking ON"));
              //fona.sendSMS(callerIDbuffer, "Tracking ON");
              if (fona_tracking_on()) {
                sprintf(replybuffer, PSTR("Tracking ON"));
                //replybuffer = "Tracking ON";
              }
              else {
                sprintf(replybuffer, PSTR("Failed to turn tracking on"));
                //replybuffer = "Failed to turn tracking on";
              }
            }
            else if (strcasestr_P(replybuffer, PSTR("trackingoff"))) {
              Serial.println(F("#turn Tracking OFF"));
              fona_tracking_off();
              sprintf(replybuffer, PSTR("Tracking OFF"));
            }
            #endif
            else if (strcasestr_P(replybuffer, PSTR("heizungon"))) {
              sprintf(replybuffer, PSTR("Heizung ON"));
            }
            else if (strcasestr_P(replybuffer, PSTR("heizungoff"))) {
              sprintf(replybuffer, PSTR("Heizung OFF"));
            }


            fona.sendSMS(callerIDbuffer, replybuffer);
          }

          // delete the original msg after it is processed
          //   otherwise, we will fill up all the slots
          //   and then we won't be able to receive SMS anymore
          fona.deleteSMS(smsn);

          smsn = 20;
        }
      }
    }
    //}
  }
  #endif
  #endif
}




void fona_sendSMS_battstatus() {
  #ifdef SMS_Commands
  #ifdef FONA
  char buffer_batt[3];

  sprintf(replybuffer, "Batterie Status=%s",
          dtostrf(vbat, 3, 0, buffer_batt));
  fona.sendSMS(MyNumber, replybuffer);
  #endif
  #endif
}

void fona_sendSMS_Alarm() {
  #ifdef SMS_Commands
  #ifdef FONA
  Serial.print(F("#send alarm SMS for: "));
  Serial.println(alarm_activator);
  //char message[100];
  char buffer_long[10];
  char buffer_lat[10];
  char buffer_alt[10];
  char buffer_speed[10];
  char buffer_course[3];

  sprintf_P(replybuffer, PSTR("ALARM!!!\n\nhttp://maps.apple.com/?q=%s,%s&t=m\n\nLatitude:%s\nLongitude:%s\nSpeed:%s\nCourse:%s"),
                        dtostrf(gps_latitude, 5, 6, buffer_lat),
                        dtostrf(gps_longitude, 5, 6, buffer_long),
                        buffer_lat,
                        buffer_long,
                        dtostrf(gps_speed, 3, 0, buffer_speed),
                        dtostrf(gps_course, 3, 0, buffer_course));

  fona.sendSMS(MyNumber, replybuffer);
  if (MyNumber == alarm_activator) {
    fona.sendSMS(alarm_activator, replybuffer);
  }
  #endif
  #endif
}


/*
 * Tracking
 */

boolean fona_tracking_on() {
  #ifdef tracking
  #ifdef FONA
  // Optionally configure a GPRS APN, username, and password.
  // You might need to do this to access your network's GPRS/data
  // network.  Contact your provider for the exact APN, username,
  // and password values.  Username and password are optional and
  // can be removed, but APN is required.
  fona.setGPRSNetworkSettings(F(GPRS_APN), F(GPRS_user), F(GPRS_pw));

  // Optionally configure HTTP gets to follow redirects over SSL.
  // Default is not to follow SSL redirects, however if you uncomment
  // the following line then redirects over SSL will be followed.
  fona.setHTTPSRedirect(true);


  //delay(5000);
  // turn GPRS on
  if (!fona.enableGPRS(true)) {
    Serial.println(F("#Failed to turn on GPRS"));
    delay(5000);
    if (!fona.enableGPRS(true)) {
      Serial.println(F("#Failed"));
      delay(5000);
      if (!fona.enableGPRS(true)) {
        Serial.println(F("Failed"));
        delay(5000);
      }
    }
    return false;
  }
  else {
    gprs_tracking = true;
    Serial.println(F("#GPRS turned ON"));

    //#ifdef LCD
    //init_display();

    display_bootmsg(F("GPRS turned ON"));
    //bootmsg = F("#GPRS turned ON");
    //display_update();
    INFO_PRINTLN(F("#GPRS turned ON"));
    delay(5000);
    //#endif
    return true;
  }
  #endif
  #endif
}

void fona_tracking_off() {
  #ifdef tracking
  #ifdef FONA
  // turn GPRS off
  if (!fona.enableGPRS(false)) {
    delay(5000);
    if (!fona.enableGPRS(false)) {
      delay(5000);
      if (!fona.enableGPRS(false)) {
        delay(5000);
        if (!fona.enableGPRS(false)) {
          Serial.println(F("#Failed to turn off"));
        }
      }
    } 
  }
  #endif
  #endif        
}


void fona_track() {
  #ifdef tracking
  #ifdef FONA
  //if (gprs_tracking) {
  char buffer_long[10];
  char buffer_lat[10];
  char buffer_alt[10];
  char buffer_speed[10];
  char buffer_course[3];
  char buffer_time[24];

  uint16_t statuscode;
  int16_t length;

  sprintf(buffer_time, "%04d%02d%02d%02d%02d%02d", gps_year, gps_month, gps_day, gps_hour, gps_minute, gps_second);
  sprintf_P(replybuffer, PSTR("brun.rhoencouch.de/gps.php?visor=false&latitude=%s&longitude=%s&altitude=%s&time=%s&satellites=%s&speedOTG=%s&course=%s"), 
                        dtostrf(gps_latitude, 5, 6, buffer_lat), 
                        dtostrf(gps_longitude, 5, 6, buffer_long), 
                        dtostrf(gps_altitude, 5, 0, buffer_alt), 
                        buffer_time,
                        "10", 
                        dtostrf(gps_speed, 3, 0, buffer_speed), 
                        "13");

  Serial.println(replybuffer);

  //Serial.println(F("\n****"));
  if (!fona.HTTP_GET_start(replybuffer, &statuscode, (uint16_t *)&length)) {
    Serial.println("#Failed to track possition!");
  }
  Serial.println(statuscode, DEC);

  fona.HTTP_GET_end();
//}        
  #endif        
  #endif        
}

/*
 * Check Backup Batterie
 * and send a SMS if level is <20%
 */
void fona_battcheck() {
  #ifdef DEBUG
  Serial.println(F("#fona_battcheck()"));
  #endif
  if (! fona.getBattPercent(&vbat)) {
    Serial.println(F("Failed to read Batt"));
  } 
  DEBUG_PRINT(F("Batt: "));
  DEBUG_PRINTLN(vbat);
  if (vbat > 0 ) {
    if (vbat < 20 ) {
      if (!battstatus_send) {
        #ifdef SMS_Commands
        fona_sendSMS_battstatus();
        #endif
        battstatus_send = true;
      }
    }
    else if (vbat > 98 ) {
      battstatus_send = false;
    }
  }
}


void fona_gps_status() {
  #ifdef DEBUG
  Serial.println(F("#status_task_callback()"));
  #endif

  fona_calculate_statistics();
  
  if (serial_export) {
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
    
  }
}

void fona_calculate_statistics() {
  #ifdef DEBUG
  Serial.println(F("#fona_calculate_statistics()"));
  #endif
  // calculating speed
  if ( gps_speed_max < gps_speed ) gps_speed_max = gps_speed;
  if ( gps_speed_max_trip < gps_speed ) gps_speed_max_trip = gps_speed;
  
  if ( engine_running ) {
    engine_running_sec = gps_second - engine_start_time_sec;
    engine_running_sec += (gps_minute - engine_start_time_min) * 60;
    engine_running_sec += (gps_hour - engine_start_time_h) * 3600;
    engine_running_sec += (gps_day - engine_start_time_day) * 86400;
    
    engine_running_total = engine_running_total_last + engine_running_sec;
  }
  else {
    engine_running_total = engine_running_total_last;
  }
  

  //float speed_avg = float(gps_distance) / float(engine_running_sec_last_all + engine_running_sec);
  //float speed_avg_start = float(gps_distance_start) / float(engine_running_sec);
  //float speed_avg_today = float(gps_distance_today) / float(engine_running_sec_last_today + engine_running_sec);
  
  //Serial.println(speed_avg, DEC);
  //speed_avg *= 3,6;
  //speed_avg_start *= 3,6;
  //speed_avg_today *= 3,6;
  //gps_speed_avg = (int)(speed_avg);
  //gps_speed_avg_start = (int)(speed_avg_start);
  //gps_speed_avg_today = (int)(speed_avg_today);

  gps_speed_avg_trip = float(float(gps_distance_trip) / float(engine_running_trip_last + engine_running_sec)) * 3,6;
  
  #ifdef DEBUG
  //Serial.print(F("#speed avg "));
  //Serial.print(gps_speed_avg, DEC);
//  Serial.print(speed_avg, DEC);
//  Serial.print(F(", speed avg start "));
  //Serial.print(gps_speed_avg_start, DEC);
//  Serial.print(speed_avg_start, DEC);
//  Serial.print(F(", speed avg today "));
  //Serial.print(gps_speed_avg_today, DEC);
//  Serial.print(speed_avg_today, DEC);
//  Serial.println();
  #endif
}
#endif

