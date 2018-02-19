/***************************************************
    This sketch is for TinyGSM and BLYNK communication.
    https://github.com/vshymanskyy/TinyGSM

    Author: Brun

 ****************************************************/


#ifdef TinyGSM

// Default heartbeat interval for GSM is 60
// If you want override this value, uncomment and set this option:
//#define BLYNK_HEARTBEAT 30

#define BLYNK_PRINT Serial // Defines the object that is used for printing
#define BLYNK_DEBUG        // Optional, this enables more detailed prints

// Select your modem:
//#define TINY_GSM_MODEM_SIM800
#define TINY_GSM_MODEM_SIM808
//#define TINY_GSM_MODEM_SIM900
//#define TINY_GSM_MODEM_A6
//#define TINY_GSM_MODEM_M590

#if !defined(TINY_GSM_RX_BUFFER)
#define TINY_GSM_RX_BUFFER 2048
#endif

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


unsigned long tinygsm_gps_timer = 0;
unsigned long tinygsm_sms_timer = 0;
unsigned long tinygsm_blynk_timer = 0;





void tinygsm_init()
{
  display_bootmsg(F("Initializing TinyGSM"));
  TINYGSM_DEBUG_PRINTLN(F("#tinygsm_init"));
  // Set GSM module baud rate
  SerialAT.begin(115200);

  // Restart takes quite some time
  // To skip it, call init() instead of restart()
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
    TINYGSM_DEBUG_PRINTLN(F("GPS enabled"));
  }
  modem.getSimStatus();

  // go online if we defined it
#ifdef ONLINE_ON_STANDBY
  stay_online = true;
  display_bootmsg(F("going online"));
#endif

  if ( stay_online ) {
    if ( tinygsm_go_online() ) {
      blynk_msg(F("Blynk is initialized"));
      online = true;
      display_bootmsg(F("online"));
      Blynk.notify(F("online"));

    }
  }

  online_intervall_timer = millis() + 10000;

  tinygsminit = true;

}



void tinygsm_loop()
{

  // Check SMS
  if ( tinygsm_sms_timer < millis() ) {
    tinygsm_sms_timer = millis() + TinyGSM_SMS_TIMER;
    TINYGSM_DEBUG_PRINTLN(F("#checking sms"));
    tinygsm_sms();
  }

  // Check GPS position
  if ( tinygsm_gps_timer < millis() ) {
    tinygsm_gps_timer = millis() + TinyGSM_GPS_TIMER;
    TRACE_PRINTLN(F("#get location"));

    gps_view_satellites = 0;
    gps_used_satellites = 0;
    gps_fixstatus = modem.getGPS(&gps_latitude, &gps_longitude, &gps_speed, &gps_altitude, &gps_view_satellites, &gps_used_satellites);
    if ( gps_fixstatus ) {

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
      //gps_distance_trip += (int)(distance + .5);
#endif

      if (gps_speed > 10) {
        gps_distance += (int)(distance + .5);

        gps_latitude_old = gps_latitude;
        gps_longitude_old = gps_longitude;
      }

      if (online) {
        if ( tinygsm_blynk_timer < millis() ) {
          TINYGSM_DEBUG_PRINTLN(F("#update Blynk location"));

          // Update position if it's more then 10m
          if ( get_distance(gps_latitude, gps_longitude, gps_latitude_blynk, gps_longitude_blynk) >= 10 ) {
            //myMap.location(1, 52.4161, 9.66569, BLYNK_DEVICE_NAME);
            myMap.location(1, gps_latitude, gps_longitude, BLYNK_DEVICE_NAME);
            TINYGSM_DEBUG_PRINTLN(F("#location is set"));
            Blynk.virtualWrite(BLYNK_VIRTUAL_gps_used_satellites, gps_used_satellites);
            Blynk.virtualWrite(BLYNK_VIRTUAL_gps_view_satellites, gps_view_satellites);

            //Blynk.virtualWrite(BLYNK_VIRTUAL_gps_latitude, 52.4161);
            //Blynk.virtualWrite(BLYNK_VIRTUAL_gps_longitude, 9.6656);
            Blynk.virtualWrite(BLYNK_VIRTUAL_gps_latitude, gps_latitude);
            Blynk.virtualWrite(BLYNK_VIRTUAL_gps_longitude, gps_longitude);
            Blynk.virtualWrite(BLYNK_VIRTUAL_gps_altitude, gps_altitude);

            gps_latitude_blynk = gps_latitude;
            gps_longitude_blynk = gps_longitude;
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


      gps_fixerrcnt = 0;
    }
    else {
      TINYGSM_DEBUG_PRINT(F("#gps not fix (cnt: "));
      TINYGSM_DEBUG_PRINT(gps_fixerrcnt);
      TINYGSM_DEBUG_PRINTLN(F(" )"));

      if ( gps_view_satellites == 0 ) {
        gps_fixerrcnt++;
      }

      if (gps_fixerrcnt >= 100) {
        TINYGSM_DEBUG_PRINTLN(F("#try to reset GPS"));
        if ( !modem.disableGPS() ) {
          INFO_PRINTLN(F("can't disable GPS"));
        }
        else {
          TINYGSM_DEBUG_PRINTLN(F("GPS disabled"));
        }
        delay(3000);
        if ( !modem.enableGPS() ) {
          INFO_PRINTLN(F("can't enable GPS"));
        }
        else {
          TINYGSM_DEBUG_PRINTLN(F("GPS enabled"));
        }
        gps_fixerrcnt = 0;
      }

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
}


void tinygsm_sms() {

  int numofsms = modem.getNumSMS();

  char sender[18];

  TRACE_PRINTLN(F("#Read SMS"));
  for (int i = 1; i < numofsms + 1; i++) {
    TINYGSM_DEBUG_PRINTLN(F("Read SMS# "));
    TINYGSM_DEBUG_PRINTLN(i);
    String smsmsg = modem.readSMS(i, sender);
    smsmsg.toLowerCase();
    //    sender.replace('"', 0);
    TINYGSM_DEBUG_PRINT(F("#Sender: "));
    TINYGSM_DEBUG_PRINTLN(sender);
    TINYGSM_DEBUG_PRINTLN(F("Message: "));
    TINYGSM_DEBUG_PRINTLN(F("##############################"));
    TINYGSM_DEBUG_PRINTLN(smsmsg);
    TINYGSM_DEBUG_PRINTLN(F("##############################"));

    // Check if keyword present
    if ( smsmsg.indexOf(SMS_Keyword) >= 0 ) {
      if ( smsmsg.indexOf(F("on")) >= 0 ) {
        if ( tinygsm_go_online() ) {
          Blynk.notify(F("Hello. Now I'm online!"));
          online = true;
        }
        else  {
          modem.sendSMS(sender, F("Coldn't connect to Blynk Server"));
        }
        modem.deleteSMS(i);
      }
      else if ( smsmsg.indexOf("off") >= 0 ) {
        Blynk.notify(F("disconnect Blynk"));

        if ( tinygsm_go_offline() ) {
          online = false;
          modem.deleteSMS(i);
          TINYGSM_DEBUG_PRINTLN(F("Disconnected"));
        }
      }
    }
    else {
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

    online_LED.on();

    if (flash_watchdog_reset.read()) {
      Blynk.notify(F("Watchdog reset..."));
      flash_watchdog_reset.write(false);
    }

    if (startmsg) {
      Blynk.notify(F("booted..."));
      startmsg = false;
    }

    blynk = true;
    return true;
  }
  else {
    return false;
  }
}

boolean tinygsm_go_offline() {

  blynk_msg(F("Blynk going offline"));

  online_LED.off();

  delay(2000);

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

  modem.disableGPS();

  delay(5000);
}

void tinygsm_wakeup() {
  INFO_PRINTLN(F("#wakeup TinyGSM"));


  modem.enableGPS();



  delay(5000);
}


void tinygsm_alarm() {
  INFO_PRINTLN(F("#tinygsm_alarm"));
  modem.sendSMS(MYNUMBER, F("ALARM!!!"));
  Blynk.notify(F("ALARM!!!"));

  blynk_msg(F("ALARM!!!"));

  alarm_led.on();

  if (geo_fence_alarm) {
    geo_fence_led.on();
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
 *   Blynk Functions
 */
BLYNK_CONNECTED() {
  Blynk.syncAll();
}

BLYNK_APP_DISCONNECTED() {
  blynk_msg(F("app disconected"));
}

BLYNK_APP_CONNECTED() {
  blynk_msg(F("app conected"));
}


BLYNK_WRITE(BLYNK_VIRTUAL_stay_online) {
  int value = param.asInt(); // Get value as integer


  if ( value == 1 ) {
    stay_online = true;
    blynk_msg(F("stay_online turned on"));
  }
  else {
    stay_online = false;
    blynk_msg(F("stay_online turned off"));
    online_intervall_timer = millis() + 10000;
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
  }
}

BLYNK_WRITE(BLYNK_VIRTUAL_alarm) {
  int value = param.asInt(); // Get value as integer
  if ( value == 1 ) {
    //blynk_alarm = true;
    alarm_system_armed = true;
    armed_led.on();

    blynk_msg(F("alarm_system turned on"));

  }
  else {
    alarm_system_armed = false;
    alarm_system_triggered = false;
    armed_led.off();
    alarm_led.off();
    blynk_msg(F("alarm_system turned off"));

  }
}

#endif // TinyGSM
