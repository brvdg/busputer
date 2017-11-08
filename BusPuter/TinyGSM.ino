/***************************************************
    This sketch is for TinyGSM and BLYNK communication.
    https://github.com/vshymanskyy/TinyGSM

    Author: Brun

 ****************************************************/


#ifdef TinyGSM

// Default heartbeat interval for GSM is 60
// If you want override this value, uncomment and set this option:
//#define BLYNK_HEARTBEAT 30

//#define BLYNK_PRINT Serial // Defines the object that is used for printing
//#define BLYNK_DEBUG        // Optional, this enables more detailed prints

// Select your modem:
//#define TINY_GSM_MODEM_SIM800
#define TINY_GSM_MODEM_SIM808
//#define TINY_GSM_MODEM_SIM900
//#define TINY_GSM_MODEM_A6
//#define TINY_GSM_MODEM_M590

// Hardware Serial on Mega, Leonardo, Micro
#define SerialAT Serial1

TinyGsm modem(SerialAT);

WidgetTerminal terminal(BLYNK_VIRTUAL_terminal);
WidgetMap myMap(BLYNK_VIRTUAL_map);
WidgetLED geo_fence_armed_led(BLYNK_VIRTUAL_geo_fence_armed_led);
WidgetLED geo_fence_led(BLYNK_VIRTUAL_geo_fence_led);
WidgetLED online_LED(BLYNK_VIRTUAL_online_LED);
WidgetLED armed_led(BLYNK_VIRTUAL_armed_led);
WidgetLED alarm_led(BLYNK_VIRTUAL_alarm_led);

boolean blynk = false;
byte i = 0;

//unsigned long timer = 0;
//unsigned long timer2 = 0;

unsigned long tinygsm_gps_timer = 0;
unsigned long tinygsm_sms_timer = 0;
unsigned long tinygsm_blynk_timer = 0;





void tinygsm_init()
{

  // Set GSM module baud rate
  SerialAT.begin(115200);

  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  display_bootmsg(F("Initializing TinyGSM"));
  modem.restart();

  // Unlock your SIM card with a PIN
  //modem.simUnlock("1234");
  //delay(3000);

  modem.begin();

  display_bootmsg(F("enable GPS"));
  if ( !modem.enableGPS() ) {
    INFO_PRINTLN(F("can't enable GPS"));
  }
  else {
    DEBUG_PRINTLN(F("GPS enabled"));
  }
  modem.getSimStatus();

  // go online if we defined it
#ifdef ONLINE_ON_STANDBY
  stay_online = true;
#endif

  //if ( stay_online ) {
    if ( tinygsm_go_online() ) {
      //sprintf(buf, "%02d:%02d:%02d", rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());
      //terminal.print(buf);
      //terminal.print(F(" - "));
      //terminal.println(F("Blynk is initialized"));
      //terminal.flush();

      blynk_msg(F("Blynk is initialized"));
      online = true;
    }
  //}

  //online_intervall_timer = millis() + 10000;

}



void tinygsm_loop()
{
  //TRACE_PRINTLN(F("#->tinygsm_loop"));

  //#ifdef ONLINE_INTERVALL
  /*if ( !stay_online ) {
    if ( online_intervall_timer < millis() ) {
      if (!online) {
        DEBUG_PRINTLN(F("#going online "));
        if (tinygsm_go_online()) {
          online = true;
          online_intervall_timer = millis() + 30000;
        }
      }
      else {
        //if (!blynk_geo_fence_alarmed) {
        if (!alarm_system_triggered) {
          DEBUG_PRINTLN(F("#going offline "));
          if (tinygsm_go_offline()) {
            online = false;
            online_intervall_timer = millis() + ONLINE_INTERVALL * 60000;
          }
        }
      }
    }
  }*/
  //#endif



  if ( tinygsm_sms_timer < millis() ) {
    tinygsm_sms_timer = millis() + TinyGSM_SMS_TIMER;
    DEBUG_PRINTLN(F("#checking sms"));
    tinygsm_sms();
  }



  if ( tinygsm_gps_timer < millis() ) {
    tinygsm_gps_timer = millis() + TinyGSM_GPS_TIMER;
    TRACE_PRINTLN(F("#get location"));
    gps_fixstatus = modem.getGPS(&gps_latitude, &gps_longitude, &gps_speed, &gps_altitude, &gps_view_satellites, &gps_used_satellites);
    if ( gps_fixstatus ) {

      /*DEBUG_PRINT(F("#GPS Location: LAT: "));
        DEBUG_PRINT(gps_latitude);
        DEBUG_PRINT(F(" LONG: "));
        DEBUG_PRINT(gps_longitude);
        DEBUG_PRINT(F(" SPEED: "));
        DEBUG_PRINT(gps_speed);
        DEBUG_PRINT(F(" ALTITUDE: "));¨
        DEBUG_PRINT(gps_altitude);
        DEBUG_PRINT(F(" VIEWED SATELITES: "));
        DEBUG_PRINT(gps_view_satellites);
        DEBUG_PRINT(F(" USED STELITES: "));
        DEBUG_PRINTLN(gps_used_satellites);*/

      int year, month, day, hour, minute, second;
      if ( modem.getGPSTime(&gps_year, &gps_month, &gps_day, &gps_hour, &gps_minute, &gps_second) ) {
        //Sync time if it's diffrent.
        set_time(gps_year, gps_month, gps_day, gps_hour, gps_minute, gps_second);
      }

      if (gps_latitude_old == 0) {
        gps_latitude_old = gps_latitude;
        gps_longitude_old = gps_longitude;
      }

      float distance = get_distance(gps_latitude, gps_longitude, gps_latitude_old, gps_longitude_old);
#ifdef OFFLINE
      gps_distance += (int)(distance + .5);
      gps_distance_trip += (int)(distance + .5);
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
         geofance
      */
      /*if ( geo_fence_enabled ) {
        if ( get_distance(gps_latitude_blynk, gps_longitude_blynk, gps_latitude_blynk_geo_fence, gps_longitude_blynk_geo_fence) >= geo_fence_distance ) {
          if (!blynk_geo_fence_alarmed) {
            blynk_geo_fence_alarmed = true;
            if (!online) {
              if (tinygsm_go_online()) {
                online = true;
              }
              else {
                //Send an SMS
              }
            }
            Blynk.notify(F("ALARM!!! Geo-fance exceeded!"));
            geo_fence_led.on();
            sprintf(buf, "%02d:%02d:%02d", rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());
            terminal.print(buf);
            terminal.print(F(" - "));
            terminal.print(F("Geo-fance exceeded!!! "));
            terminal.print(get_distance(gps_latitude_blynk, gps_longitude_blynk, gps_latitude_blynk_geo_fence, gps_longitude_blynk_geo_fence));
            terminal.println(F("m"));
            terminal.flush();
          }
        }
        }*/



      if (online) {
        //Blynk.run();
        if ( tinygsm_blynk_timer < millis() ) {
          DEBUG_PRINTLN(F("#set location"));

          // Update position if it's more then 10m
          if ( get_distance(gps_latitude, gps_longitude, gps_latitude_blynk, gps_longitude_blynk) >= 10 ) {
            //myMap.location(1, 52.4161, 9.66569, BLYNK_DEVICE_NAME);
            myMap.location(1, gps_latitude, gps_longitude, BLYNK_DEVICE_NAME);
            DEBUG_PRINTLN(F("#location is set"));
            Blynk.virtualWrite(BLYNK_VIRTUAL_gps_used_satellites, gps_used_satellites);
            Blynk.virtualWrite(BLYNK_VIRTUAL_gps_view_satellites, gps_view_satellites);

            //Blynk.virtualWrite(BLYNK_VIRTUAL_gps_latitude, 52.4161);
            //Blynk.virtualWrite(BLYNK_VIRTUAL_gps_longitude, 9.6656);
            Blynk.virtualWrite(BLYNK_VIRTUAL_gps_latitude, gps_latitude);
            Blynk.virtualWrite(BLYNK_VIRTUAL_gps_longitude, gps_longitude);
            Blynk.virtualWrite(BLYNK_VIRTUAL_gps_altitude, gps_altitude);
            
            gps_latitude_blynk = gps_latitude;
            gps_longitude_blynk = gps_longitude;
            //terminal.print(F("distance was: "));
            //terminal.println(get_distance(gps_latitude, gps_longitude, gps_latitude_old, gps_longitude_old));
            //terminal.flush();
          }


          if (gps_used_satellites != gps_used_satellites_blynk) {
            Blynk.virtualWrite(BLYNK_VIRTUAL_gps_used_satellites, gps_used_satellites);
            gps_used_satellites_blynk = gps_used_satellites;
          }
          if (gps_view_satellites != gps_view_satellites_blynk) {
            Blynk.virtualWrite(BLYNK_VIRTUAL_gps_view_satellites, gps_view_satellites);
            gps_view_satellites_blynk = gps_view_satellites;
          }
          if (gps_altitude != gps_altitude_blynk) {
            Blynk.virtualWrite(BLYNK_VIRTUAL_gps_altitude, gps_altitude);
            gps_altitude_blynk = gps_altitude;
          }

          Blynk.run();

          tinygsm_blynk_timer = millis() + TinyGSM_BLYNK_TIMER;
        }
      }
    }
    else {
      DEBUG_PRINTLN(F("#gps not fix"));
      // set the old values
      //gps_latitude = gps_latitude_old;
      //gps_longitude = gps_longitude_old;
      if (online) {
        if (gps_used_satellites != gps_used_satellites_blynk) {
          Blynk.virtualWrite(BLYNK_VIRTUAL_gps_used_satellites, gps_used_satellites);
          gps_used_satellites_blynk = gps_used_satellites;
        }
        if (gps_view_satellites != gps_view_satellites_blynk) {
          Blynk.virtualWrite(BLYNK_VIRTUAL_gps_view_satellites, gps_view_satellites);
          gps_view_satellites_blynk = gps_view_satellites;
        }
        Blynk.run();
      }
    }
  }


  /*if (online) {
    Blynk.run();
    if (Blynk.connected()) {
      if ( geo_fence_armed ) geo_fence_armed_led.on();

      if (gps_used_satellites != gps_used_satellites_blynk) {
        Blynk.virtualWrite(BLYNK_VIRTUAL_gps_used_satellites, gps_used_satellites);
        gps_used_satellites_blynk = gps_used_satellites;
      }
      if (gps_view_satellites != gps_view_satellites_blynk) {
        Blynk.virtualWrite(BLYNK_VIRTUAL_gps_view_satellites, gps_view_satellites);
        gps_view_satellites_blynk = gps_view_satellites;
      }

      
    }
    // reset the connection
    else {
      DEBUG_PRINTLN(F("BLYNK is offline"));
      blynk_offline_counter++;
      if ( blynk_offline_counter >= 100 ) {
        tinygsm_go_offline();
        delay(2000);
        tinygsm_go_online();
        blynk_offline_counter = 0;
        sprintf(buf, "%02d:%02d:%02d", rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());
        terminal.print(buf);
        terminal.print(F(" - "));
        terminal.println(F("BLYNK connection was lost"));
        terminal.flush();
      }
    }
  }*/
}


void tinygsm_sms() {

  int numofsms = modem.getNumSMS();
  //Serial.println(numofsms, DEC);

  char sender[18];

  DEBUG_PRINTLN(F("#Read SMS"));
  for (int i = 1; i < numofsms + 1; i++) {
    DEBUG_PRINTLN(F("Read SMS# "));
    DEBUG_PRINTLN(i);
    String smsmsg = modem.readSMS(i, sender);
    smsmsg.toLowerCase();
    //    sender.replace('"', 0);
    DEBUG_PRINT(F("#Sender: "));
    DEBUG_PRINTLN(sender);
    DEBUG_PRINTLN(F("Message: "));
    DEBUG_PRINTLN(F("##############################"));
    DEBUG_PRINTLN(smsmsg);
    DEBUG_PRINTLN(F("##############################"));
    //DEBUG_PRINTLN(smsmsg.indexOf("on"));

    // Check if keyword present
    if ( smsmsg.indexOf(SMS_Keyword) >= 0 ) {
      //DEBUG_PRINTLN(F("YAHEEE!!!!!"));
      //DEBUG_PRINTLN(F("reply a sms"));
      if ( smsmsg.indexOf(F("on")) >= 0 ) {
        if ( tinygsm_go_online() ) {
          Blynk.notify(F("Hello. Now I'm online!"));
          online = true;
        }
        else  {
          modem.sendSMS(sender, F("Coldn't connect to Blynk Server"));
        }
        //DEBUG_PRINTLN("F(#delete this sms."));
        modem.deleteSMS(i);
      }
      else if ( smsmsg.indexOf("off") >= 0 ) {
        //modem.sendSMS(sender, "Goodby");
        //Serial.println("delete this sms.");
        Blynk.notify(F("disconnect Blynk"));

        if ( tinygsm_go_offline() ) {
          online = false;
          modem.deleteSMS(i);
          DEBUG_PRINTLN(F("Disconnected"));
        }
      }
    }
    else {
      //Serial.println("delete this sms.");
      modem.deleteSMS(i);
    }
  }
}

boolean tinygsm_go_online() {

  blynk_key.trim();
  char tmp_blynk_key[blynk_key.length() + 1];
  blynk_key.toCharArray(tmp_blynk_key, blynk_key.length() + 1);

  sim_apn.trim();
  char tmp_sim_apn[sim_apn.length() + 1];
  sim_apn.toCharArray(tmp_sim_apn, sim_apn.length() + 1);

  sim_user.trim();
  char tmp_sim_user[sim_pass.length() + 1];
  sim_user.toCharArray(tmp_sim_user, sim_user.length() + 1);

  sim_pass.trim();
  char tmp_sim_pass[sim_pass.length() + 1];
  sim_pass.toCharArray(tmp_sim_pass, sim_pass.length() + 1);

  TRACE_PRINT(F("#tmp_blynk_key: "));
  TRACE_PRINTLN(tmp_blynk_key);
  TRACE_PRINT(F("#tmp_sim_apn: "));
  //TRACE_PRINTLN(tmp_sim_apn);
  TRACE_PRINTLN(sim_apn);
  TRACE_PRINT(F("#tmp_sim_user: "));
  TRACE_PRINTLN(tmp_sim_user);
  TRACE_PRINT(F("#tmp_sim_pass: "));
  TRACE_PRINTLN(tmp_sim_pass);

  Blynk.begin(tmp_blynk_key, modem, tmp_sim_apn, tmp_sim_user, tmp_sim_pass);

  online_intervall_timer = millis() + 10000;
  
  if (Blynk.connect()) {
    blynk_msg(F("Blynk is now online"));
    
    /*sprintf(buf, "%02d:%02d:%02d", rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());
    terminal.print(buf);
    terminal.print(F(" - "));
    terminal.println(F("Blynk is now online"));
    terminal.flush();*/
    
    online_LED.on();
    blynk = true;
    return true;
  }
  else {
    return false;
  }
}

boolean tinygsm_go_offline() {

  blynk_msg(F("Blynk going offline"));
  
  /*sprintf(buf, "%02d:%02d:%02d", rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());
  terminal.print(buf);
  terminal.print(F(" - "));
  terminal.println(F("Blynk going offline"));
  terminal.flush();*/
  online_LED.off();

  delay(1000);

  Blynk.disconnect();
  if (modem.gprsDisconnect()) {
    return true;
  }
  else {
    if (!modem.gprsDisconnect()) {
      if (!modem.gprsDisconnect()) {
        INFO_PRINTLN(F("TinyGSM couldn't go offline"));
        return false;
      }
    }
  }

}


void tinygsm_sleep() {
  INFO_PRINTLN(F("TinyGSM go into sleep mode..."));
  /*if ( modem.standby() ) {
    INFO_PRINTLN(F("OK"));
    } else {
    INFO_PRINTLN(F("failed"));
    }*/

  modem.disableGPS();



  delay(5000);
}

void tinygsm_wakeup() {
  INFO_PRINTLN(F("#wakeup TinyGSM"));


  modem.enableGPS();



  delay(5000);
}

/*void alarmMatch() // Do something when interrupt called
  {

  }*/


void tinygsm_alarm() {
  INFO_PRINTLN(F("#tinygsm_alarm"));
  modem.sendSMS(MYNUMBER, F("ALARM!!!"));
  Blynk.notify(F("ALARM!!!"));

  blynk_msg(F("ALARM!!!"));
  
  /*sprintf(buf, "%02d:%02d:%02d", rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());
  terminal.print(buf);
  terminal.print(F(" - "));
  terminal.println(F("ALARM!!!"));
  terminal.flush();*/

  alarm_led.on();

  if (geo_fence_alarm) {
    //geo_fence_armed_led.on();
    geo_fence_led.on();
    /*sprintf(buf, "%02d:%02d:%02d", rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());
    terminal.print(buf);
    terminal.print(F(" - "));
    terminal.print(F("geo fence:  "));
    terminal.print(get_distance(gps_latitude_blynk, gps_longitude_blynk, gps_latitude_blynk_geo_fence, gps_longitude_blynk_geo_fence));
    terminal.println(F("m"));
    terminal.flush();*/
  }

}

void blynk_msg( String msg ) {
  sprintf(buf, "%02d:%02d:%02d", rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());
  terminal.print(buf);
  terminal.print(F(" - "));
  terminal.println(msg);
  terminal.flush();
}

void blynk_msg_float( String msg, float value ) {
  sprintf(buf, "%02d:%02d:%02d", rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());
  terminal.print(buf);
  terminal.print(F(" - "));
  terminal.print(msg);
  terminal.println(value);
  terminal.flush();
}


/*
   Blynk Functions

*/
BLYNK_CONNECTED() {
  Blynk.syncAll();
}

BLYNK_APP_DISCONNECTED() {
  //Blynk.notify(F("app disconected"));
  blynk_msg(F("app disconected"));
  
  /*sprintf(buf, "%02d:%02d:%02d", rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());
  terminal.print(buf);
  terminal.print(F(" - "));
  terminal.println(F("app disconected"));*/
}

BLYNK_APP_CONNECTED() {
  //Blynk.notify(F("app conected"));
  blynk_msg(F("app conected"));
  
  /*sprintf(buf, "%02d:%02d:%02d", rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());
  terminal.print(buf);
  terminal.print(F(" - "));
  terminal.println(F("app conected"));
  terminal.flush();*/
}

/*BLYNK_WRITE(BLYNK_VIRTUAL_geoalarm_modus)
  {
  int value = param.asInt(); // Get value as integer
  TRACE_PRINTLN(F("#BLYNK_VIRTUAL_geoalarm_modus triggert"));
  TRACE_PRINTLN(value);

  if ( value == 1 ) {
    if (!geo_fence_enabled) {
      geo_fence_enabled = true;
      sprintf(buf, "%02d:%02d:%02d", rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());
      terminal.print(buf);
      terminal.print(F(" - "));
      terminal.println(F("geo_fence alarm on"));

      //geo_fence_armed_led.on();
      geo_fence_led.off();

      gps_latitude_blynk_geo_fence = gps_latitude;
      gps_longitude_blynk_geo_fence = gps_longitude;

      //geo_fence_enableded = false;
      sprintf(buf, "%02d:%02d:%02d", rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());
      terminal.print(buf);
      terminal.print(F(" - "));
      terminal.print(F("geo_fence distance is set to: "));
      terminal.print(geo_fence_distance);
      terminal.println(F("m"));

    }
  }
  else {
    if (geo_fence_enabled) {
      geo_fence_enabled = false;
      geo_fence_armed = false;

      sprintf(buf, "%02d:%02d:%02d", rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());
      terminal.print(buf);
      terminal.print(F(" - "));
      terminal.println(F("geo_fence alarm off"));
      geo_fence_armed_led.off();
      geo_fence_led.off();
      blynk_geo_fence_alarmed = false;
  #ifdef ONLINE_INTERVALL
      online_intervall_timer = millis() + 30000; // 30s to turn off
  #endif

    }
  }
  terminal.flush();
  }*/

BLYNK_WRITE(BLYNK_VIRTUAL_stay_online) {
  int value = param.asInt(); // Get value as integer


  if ( value == 1 ) {
    stay_online = true;
    blynk_msg(F("stay_online turned on"));
    /*sprintf(buf, "%02d:%02d:%02d", rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());
      terminal.print(buf);
      terminal.print(F(" - "));
      terminal.println(F("stay_online turned on"));
      terminal.flush();*/

  }
  else {
    stay_online = false;
    blynk_msg(F("stay_online turned off"));
    online_intervall_timer = millis() + 10000;
    /*sprintf(buf, "%02d:%02d:%02d", rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());
      terminal.print(buf);
      terminal.print(F(" - "));
      terminal.println(F("stay_online turned off"));
      terminal.flush();*/
  }
}

BLYNK_WRITE(BLYNK_VIRTUAL_geo_fence_distance)
{
  int value = param.asInt(); // Get value as integer
  if ( value != geo_fence_distance ) {
    INFO_PRINT(F("#set geo-fance distance to "));
    INFO_PRINTLN(value);
    value += 5; 
    value = ( value / 10 ) * 10;
    geo_fence_distance = value;
    Blynk.virtualWrite(BLYNK_VIRTUAL_geo_fence_distance, value);

    if ( geo_fence_distance != 0 ) {
      blynk_msg_float(F("geo fance distance set to: "), value);
    }    
    else {
      blynk_msg(F("geo fence disabled"));
    }
      

    /*sprintf(buf, "%02d:%02d:%02d", rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());
    terminal.print(buf);
    terminal.print(F(" - "));
    terminal.print(F("geo-fance distance is set to: "));
    terminal.print(geo_fence_distance);
    terminal.println(F("m"));
    terminal.flush();*/
    
  }

}

BLYNK_WRITE(BLYNK_VIRTUAL_alarm) {
  int value = param.asInt(); // Get value as integer


  //sprintf(buf, "%02d:%02d:%02d", rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());
  //terminal.print(buf);
  //terminal.print(F(" - "));


  if ( value == 1 ) {
    //blynk_alarm = true;
    alarm_system_armed = true;
    armed_led.on();

    blynk_msg(F("alarm_system turned on"));

  }
  else {
    //blynk_alarm = false;
    alarm_system_armed = false;
    alarm_system_triggered = false;
    armed_led.off();
    alarm_led.off();
    blynk_msg(F("alarm_system turned off"));

  }
}

#endif // TinyGSM
