/**************************************************************
 *
 * For this example, you need to install Blynk library:
 *   https://github.com/blynkkk/blynk-library/releases/latest
 *
 * TinyGSM Getting Started guide:
 *   http://tiny.cc/tiny-gsm-readme
 *
 **************************************************************
 *
 * Blynk is a platform with iOS and Android apps to control
 * Arduino, Raspberry Pi and the likes over the Internet.
 * You can easily build graphic interfaces for all your
 * projects by simply dragging and dropping widgets.
 *
 * Blynk supports many development boards with WiFi, Ethernet,
 * GSM, Bluetooth, BLE, USB/Serial connection methods.
 * See more in Blynk library examples and community forum.
 *
 *                http://www.blynk.io/
 *
 * Change GPRS apm, user, pass, and Blynk auth token to run :)
 **************************************************************/

//#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space

// Default heartbeat interval for GSM is 60
// If you want override this value, uncomment and set this option:
//#define BLYNK_HEARTBEAT 30

#define BLYNK_PRINT Serial // Defines the object that is used for printing
//#define BLYNK_DEBUG        // Optional, this enables more detailed prints

// Select your modem:
//#define TINY_GSM_MODEM_SIM800
#define TINY_GSM_MODEM_SIM808
//#define TINY_GSM_MODEM_SIM900
//#define TINY_GSM_MODEM_A6
//#define TINY_GSM_MODEM_M590

// included in the main part
//#include <TinyGsmClient.h>
//#include <BlynkSimpleSIM800.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
//const char auth[] = "2f4fcc4b571343c182cb46318f073427";
//#define AUTH_KEY "2f4fcc4b571343c182cb46318f073427"

// Your GPRS credentials
// Leave empty, if missing user or pass
//const char apn[]  = "internet.t-mobile";
//const char user[] = "t-mobile";
//const char pass[] = "tm";

// Hardware Serial on Mega, Leonardo, Micro
#define SerialAT Serial1

TinyGsm modem(SerialAT);

WidgetTerminal terminal(BLYNK_VIRTUAL_terminal);
WidgetMap myMap(BLYNK_VIRTUAL_map);
WidgetLED alarm_led(BLYNK_VIRTUAL_alarm_led);
WidgetLED geofancy_led(BLYNK_VIRTUAL_geofancy_led);

boolean blynk = false;
byte i = 0;

unsigned long timer = 0;
unsigned long timer2 = 0;

unsigned long sim808_gps_timer = 0;
unsigned long sim808_sms_timer = 0;
unsigned long sim808_blynk_timer = 0;
//unsigned long fona_batt_timer = 0;
//unsigned long fona_gps_status_timer = 0;




void sim808_init()
{

  //pinMode(8, OUTPUT);
  //digitalWrite(8, HIGH);

  //pinMode(5, OUTPUT);
  //digitalWrite(5, HIGH);
  
  // Set console baud rate
//  Serial.begin(115200);
//  delay(10);

  // Set GSM module baud rate
  SerialAT.begin(115200);
  //delay(3000);

  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  //Serial.println("Initializing modem...");
  display_bootmsg(F("Initializing SIM808"));
  modem.restart();

  // Unlock your SIM card with a PIN
  //modem.simUnlock("7782");
  //delay(3000);

  //Blynk.begin(auth, modem, apn, user, pass);
  modem.begin();

  display_bootmsg(F("enable GPS"));
  if ( !modem.enableGPS() ) {
    INFO_PRINTLN(F("can't enable GPS"));
  }
  else {
    DEBUG_PRINTLN(F("GPS enabled"));
  }
  modem.getSimStatus();

  //digitalWrite(8, LOW);

  //rtc.begin();

  // go online if we defined it
  #ifdef ONLINE_ON_STANDBY
  if ( sim808_go_online() ) {
    //Blynk.notify(F("Blynk is initialized"));
    terminal.println(F("Blynk is initialized"));
    terminal.flush();
    online=true;
  }
  #endif
  
}



/*
 * Blynk Functions
 * 
 */
BLYNK_CONNECTED() {
    Blynk.syncAll();
}
BLYNK_APP_DISCONNECTED() {
  //Blynk.notify(F("app disconected"));
  terminal.println(F("app disconected"));
}
BLYNK_APP_CONNECTED() {
  //Blynk.notify(F("app conected"));
  terminal.println(F("app conected"));
  terminal.flush();
}

BLYNK_WRITE(BLYNK_VIRTUAL_alarm_modus) 
{   
  int value = param.asInt(); // Get value as integer
  INFO_PRINTLN(F("BLYNK_VIRTUAL_alarm_modus triggert"));
  INFO_PRINTLN(value);

  if ( value == 1 ) {
    if (!blynk_alarm) {
      blynk_alarm = true;
      terminal.println(F("alarm modus on"));
    
      alarm_led.on();
      geofancy_led.off();

      gps_latitude_blynk_geofancy = gps_latitude;
      gps_longitude_blynk_geofancy = gps_longitude;

      //blynk_geofancy_alarmed = false;

      terminal.print(F("geofancy distance is set to: "));
      terminal.print(blynk_geofancy_distance);
      terminal.println(F("m"));
    }
  }
  else {
    if (blynk_alarm) {
      blynk_alarm = false;
      terminal.println(F("alarm modus off"));
      alarm_led.off();
      geofancy_led.off();
      blynk_geofancy_alarmed = false;
    }
  }
  //terminal.println(F("V10 triggert"));
  terminal.flush();
}

BLYNK_WRITE(BLYNK_VIRTUAL_geofancy_distance) 
{
  int value = param.asInt(); // Get value as integer
  if ( value != blynk_geofancy_distance ) {
    INFO_PRINTLN(F("set geofancy distance"));
    INFO_PRINTLN(value);
    blynk_geofancy_distance = value;

    terminal.print(F("geofancy distance is set to: "));
    terminal.print(blynk_geofancy_distance);
    terminal.println(F("m"));
    terminal.flush();
  }
  
}

void sim808_loop()
{
  //TRACE_PRINTLN(F("#->sim808_loop"));

  #ifdef ONLINE_INTERVALL
  if ( online_intervall_timer < millis() ) {
    if (!online) {
      DEBUG_PRINTLN(F("#going online "));
      if (sim808_go_online()) { 
        online = true; 
        online_intervall_timer = millis() + 10000;
      }
    }
    else {
      if (!blynk_geofancy_alarmed) {
        DEBUG_PRINTLN(F("#going offline "));
        if (sim808_go_offline()) { 
          online = false; 
          online_intervall_timer = millis() + ONLINE_INTERVALL * 6000;
        }
      }
    }
  }
  #endif

  if ( sim808_sms_timer < millis() ) {
    sim808_sms_timer = millis() + SIM808_SMS_TIMER;
    DEBUG_PRINTLN(F("#checking sms"));
    sim808_sms();
  }
  
  //modem.getGPS(&latitude, &longitude, &speed_kph, &altitude, &fix, &viewed_sat, &used_sat);

  if ( sim808_gps_timer < millis() ) {
    sim808_gps_timer = millis() + SIM808_GPS_TIMER;
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
      
      //DEBUG_PRINT(F("#Distance is: "));
      //DEBUG_PRINTLN(distance);

      if (gps_speed > 10) {
        gps_distance += (int)(distance + .5);
        gps_distance_trip += (int)(distance + .5);
        //gps_distance_today += (int)(distance + .5);
        //gps_distance_start += (int)(distance + .5);
        
        gps_latitude_old = gps_latitude;
        gps_longitude_old = gps_longitude;
      }

      /*
       * Geo Fancy
       */
      if ( blynk_alarm ) {
        if ( get_distance(gps_latitude_blynk, gps_longitude_blynk, gps_latitude_blynk_geofancy, gps_longitude_blynk_geofancy) >= blynk_geofancy_distance ) {
          if (!blynk_geofancy_alarmed) {
            blynk_geofancy_alarmed = true;
            if (!online) {
              if (sim808_go_online()) {
                online = true;
              }
              else {
                //Send an SMS
              }
            }
            Blynk.notify(F("ALARM!!! Geofancy exceeded!"));
            geofancy_led.on();
            terminal.print(F("Geofancy exceeded!!! "));
            terminal.print(get_distance(gps_latitude_blynk, gps_longitude_blynk, gps_latitude_blynk_geofancy, gps_longitude_blynk_geofancy));
            terminal.println(F("m"));
            terminal.flush();
          }
        }
      }

          

      if (online) {
        //Blynk.run();
        if ( sim808_blynk_timer < millis() ) {
          DEBUG_PRINTLN(F("#set location"));

          // Update position if it's more then 10m
          if ( get_distance(gps_latitude, gps_longitude, gps_latitude_blynk, gps_longitude_blynk) >= 10 ) {
            myMap.location(1, gps_latitude, gps_longitude, BLYNK_DEVICE_NAME);
            DEBUG_PRINTLN(F("#location is set"));
            Blynk.virtualWrite(BLYNK_VIRTUAL_gps_used_satellites, gps_used_satellites);
            Blynk.virtualWrite(BLYNK_VIRTUAL_gps_view_satellites, gps_view_satellites);
            Blynk.virtualWrite(BLYNK_VIRTUAL_gps_latitude, gps_latitude);
            Blynk.virtualWrite(BLYNK_VIRTUAL_gps_longitude, gps_longitude);
            Blynk.virtualWrite(BLYNK_VIRTUAL_gps_altitude, gps_altitude);
            gps_latitude_blynk = gps_latitude;
            gps_longitude_blynk = gps_longitude;
            terminal.print(F("distance was: "));
            terminal.println(get_distance(gps_latitude, gps_longitude, gps_latitude_old, gps_longitude_old));
            terminal.flush();
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
      
          sim808_blynk_timer = millis() + SIM808_BLYNK_TIMER;
        }
          
        
      }

      
    }
    else {
      DEBUG_PRINTLN(F("#gps not fix"));
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

  if (online) {
    Blynk.run();
    if (Blynk.connected()){

      if (gps_used_satellites != gps_used_satellites_blynk) {
        Blynk.virtualWrite(BLYNK_VIRTUAL_gps_used_satellites, gps_used_satellites);
        gps_used_satellites_blynk = gps_used_satellites;
      }
      if (gps_view_satellites != gps_view_satellites_blynk) {
        Blynk.virtualWrite(BLYNK_VIRTUAL_gps_view_satellites, gps_view_satellites);
        gps_view_satellites_blynk = gps_view_satellites;
      }

      /*if (bord_voltage != bord_voltage_blynk {
        Blynk.virtualWrite(BLYNK_VIRTUAL_bord_voltage, bord_voltage);
        bord_voltage_blynk = bord_voltage;
      }*/

    }
    else {
      DEBUG_PRINTLN(F("BLYNK is offline"));
      blynk_offline_counter++;
      if ( blynk_offline_counter >= 100 ) {
        sim808_go_offline();
        delay(2000);
        sim808_go_online();
        blynk_offline_counter = 0;
        terminal.println(F("BLYNK connection was lost"));
        terminal.flush();
      }
    }
  }
}


void sim808_sms() {
  
  int numofsms = modem.getNumSMS();
  //Serial.println(numofsms, DEC);

  char sender[18];
  
  DEBUG_PRINTLN(F("#Read SMS"));
  for (int i = 1; i < numofsms+1; i++) {
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
        if ( sim808_go_online() ) {
          Blynk.notify(F("Hello. Now I'm online!"));
          online=true;
        }
        else  {
          modem.sendSMS(sender, F("Coldn't connect to Blynk Server"));
        }
        
        
        //Serial.println("delete this sms.");
        modem.deleteSMS(i);

        /*float latitude, longitude, speed_kph, heading, speed_mph, altitude;
        String utctime;
        int fix, viewed_sat, used_sat;
        modem.getGPS(&latitude, &longitude, &speed_kph, &altitude, &fix, &viewed_sat, &used_sat);

        String status = String("http://maps.apple.com/?q=" + String(latitude, 6) + String(longitude, 6));
        status = String(status + "\nSpeed " + String(speed_kph, 0));*/
        //
        //Carng(%s,%s&t=m\n\nLatitude:%s\nLongitude:%s\nSpeed:%s\nCourse:%s"

        //modem.sendSMS(sender, status);
      }
      else if ( smsmsg.indexOf("off") >= 0 ) {
        //modem.sendSMS(sender, "Goodby");
        //Serial.println("delete this sms.");
        Blynk.notify(F("disconnect Blynk"));
        
        if ( sim808_go_offline() ) {
          online = false;
          modem.deleteSMS(i);
          DEBUG_PRINTLN(F("Disconnected"));
        }
        //Blynk.notify(F("disconnect Blynk"));
        //Blynk.disconnect();
        //if (modem.gprsDisconnect()) Serial.println(F("Disconnect ok"));
        
        

        //RESET SIM
        //digitalWrite(5, LOW);
        //delay(100);
        //digitalWrite(5, HIGH);
      }
      
    }
    else {
      //Serial.println("delete this sms.");
      modem.deleteSMS(i);
    }

  }

  //delay(1000);

}

boolean sim808_go_online() {
  
  blynk_key.trim();
  char tmp_blynk_key[blynk_key.length()+1];
  blynk_key.toCharArray(tmp_blynk_key, blynk_key.length()+1);
        
  sim_apn.trim();
  char tmp_sim_apn[sim_apn.length()+1];
  sim_apn.toCharArray(tmp_sim_apn, sim_apn.length()+1);
        
  sim_user.trim();
  char tmp_sim_user[sim_pass.length()+1];
  sim_user.toCharArray(tmp_sim_user, sim_user.length()+1);
        
  sim_pass.trim();
  char tmp_sim_pass[sim_pass.length()+1];
  sim_pass.toCharArray(tmp_sim_pass, sim_pass.length()+1);

  TRACE_PRINT(F("#tmp_blynk_key: "));
  TRACE_PRINTLN(tmp_blynk_key);
  TRACE_PRINT(F("#tmp_sim_apn: "));
  TRACE_PRINTLN(tmp_sim_apn);
  //TRACE_PRINTLN(sim_apn);
  TRACE_PRINT(F("#tmp_sim_user: "));
  TRACE_PRINTLN(tmp_sim_user);        
  TRACE_PRINT(F("#tmp_sim_pass: "));
  TRACE_PRINTLN(tmp_sim_pass);
        
  Blynk.begin(tmp_blynk_key, modem, tmp_sim_apn, tmp_sim_user, tmp_sim_pass);
  
  if (Blynk.connect()) {
    terminal.println(F("Blynk is now online"));
    terminal.flush();
    blynk = true;
    return true;
  } 
  else {
    return false;
  }
}

boolean sim808_go_offline() {

  terminal.println(F("Blynk going offline"));
  terminal.flush();

  delay(1000);
  
  Blynk.disconnect();
  if (modem.gprsDisconnect()) {
    return true;
  }
  else {
    if (!modem.gprsDisconnect()) {
      if (!modem.gprsDisconnect()) {
        INFO_PRINTLN(F("SIM808 couldn't go offline"));
        return false;
      }
    }
  }
        
}


void sim808_sleep() {
  INFO_PRINTLN(F("SIM808 go into sleep mode..."));
  /*if ( modem.standby() ) {
    INFO_PRINTLN(F("OK"));
  } else {
    INFO_PRINTLN(F("failed"));
  }*/

  modem.disableGPS();

  
  
  delay(5000);
}

void sim808_wakeup() {
  INFO_PRINTLN(F("#wakeup SIM808"));


  modem.enableGPS();

  
  
  delay(5000);
}

/*void alarmMatch() // Do something when interrupt called
{
  
}*/





