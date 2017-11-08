/***************************************************
 *  This sketch is to check serveral status information and make seperate actions.
 *  https://github.com/vshymanskyy/TinyGSM
 *
 *  Author: Brun
 *  
 ****************************************************/

void status_checker() {
  if ( status_checker_timer < millis() ) {

    check_engine();

    check_alarm_system();
    check_geo_fence();
    check_online();
  
    status_checker_timer = millis() + STATUS_CHECKER_TIMER;
  }
}


void check_engine() {
  if (bord_voltage > 4) {
    if (!engine_running) {
      engine_running = true;

      INFO_PRINTLN(F("#--->Start Engine"));

      

      engine_start = unixTime(rtc.getHours(), rtc.getMinutes(), rtc.getSeconds(), rtc.getDay(), rtc.getMonth(), rtc.getYear());

      if ( alarm_system_armed ) {
        check_alarm_system();
      }
      else {
        #ifdef U8G2_DISPLAY
        MainMenuPos = 2;
        display_update();
        #endif //U8G2_DISPLAY 

        #ifdef TinyGSM
        if ( tinygsm_go_offline() ) {
          online = false;
        }
        #endif
      }
      
      
      

      #ifdef SDCARD
      open_file();
      log_to_sdcard();
      #endif //SDCARD

      

    }
  }
  else {
    if (engine_running) {
      engine_running = false;

      INFO_PRINTLN(F("#--->Stop Engine"));

      #ifdef U8G2_DISPLAY
      MainMenuPos = 1;
      display_update();
      #endif //U8G2_DISPLAY


      engine_running = false;
      engine_running_total += engine_running_sec;

      #ifdef SDCARD
      close_file();
      #endif //SDCARD

      // check if light is still on
      if ( dimmer_V > 2 ) {
        set_alarm(600, 200, 5, true);
      }

      #ifdef TinyGSM
      if ( tinygsm_go_online() ) {
        online = true;
      }
      #endif

      // disable geo fenece
      if ( geo_fence_distance != 0 ) {
        geo_fence_enabled = false;
      }    
      else {
        blynk_msg(F("geo fence is disabled"));
      }
      
    }
  }
  
}

/*
 * Alarm System
 */
void check_alarm_system() {

  if (alarm_system_armed && !alarm_system_triggered) {
    // at started engine
    if (engine_running) {
      INFO_PRINTLN(F("#--->ALARM!!! -Engine-"));
      alarm_system_triggered = true;
      
      if ( tinygsm_go_online() ) {
        online = true;
      }
      
      set_alarm(600, 200, 5, true);
      tinygsm_alarm();

      MainMenuPos = 1;
      display_update();
    } 
  }

  // block display
  if ( alarm_system_armed ) {
    MainMenuPos = 1;
    display_update();  
  }
  
}

/*
 * Geo Fence
 */
void check_geo_fence() {
  if (geo_fence_enabled && (gps_fixstatus = 1) ) {
    if ( !geo_fence_armed ) {
      geo_fence_armed = true;
      gps_latitude_geo_fence = gps_latitude;
      gps_longitude_geo_fence = gps_longitude;
    }
    else {
      int tmp = get_distance(gps_latitude, gps_longitude, gps_latitude_geo_fence, gps_longitude_geo_fence);
      if ( !geo_fence_alarm ) {
        if ( ( tmp >= geo_fence_distance ) && ( tmp < 100000 ) ) {
          geo_fence_alarm = true;
          INFO_PRINT(F("GEO FENCE ALARM"));
          INFO_PRINT(tmp);
          INFO_PRINTLN(F(" m"));
          blynk_msg(String(tmp, DEC));
          tinygsm_alarm();
        }
      }
      else {
        if ( tmp <= geo_fence_distance ) {
          geo_fence_alarm = false;
          INFO_PRINTLN(F("back in geo fence"));
          blynk_msg(F("back in geo fence"));
        }
      }
    }
  }
}

/*void trigger_alarm() {
  alarm_system_triggered = true;
  set_alarm(600, 200, 5, true);
  tinygsm_alarm();
}*/


/*
 * check if it's time to go online or to stay onlie or to go offline
 */
void check_online() {
  
  if ( !stay_online && !alarm_system_triggered && !geo_fence_alarm) {
    if ( online_intervall_timer < millis() ) {
      Serial.println("#");
      if (!online) {
        if (tinygsm_go_online()) {
          online = true;
          online_intervall_timer = millis() + 10000;
        }
      }
      else {
        DEBUG_PRINTLN(F("#going offline "));
        if (tinygsm_go_offline()) {
          online = false;
          online_intervall_timer = millis() + ONLINE_INTERVALL * 60000;
        }
      }
    }
  }

  //check the online state
  if (online) {
    Blynk.run();
    if (!Blynk.connected()) {
      DEBUG_PRINTLN(F("BLYNK is offline"));
      blynk_offline_counter++;
      if ( blynk_offline_counter >= 100 ) {
        tinygsm_go_offline();
        delay(2000);
        tinygsm_go_online();
        blynk_offline_counter = 0;
        blynk_msg(F("BLYNK connection was lost"));
        /*sprintf(buf, "%02d:%02d:%02d", rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());
        terminal.print(buf);
        terminal.print(F(" - "));
        terminal.println(F("BLYNK connection was lost"));
        terminal.flush();*/
      }
    }
  }
}


