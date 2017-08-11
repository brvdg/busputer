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
#define BLYNK_DEBUG        // Optional, this enables more detailed prints

// Select your modem:
//#define TINY_GSM_MODEM_SIM800
#define TINY_GSM_MODEM_SIM808v2
//#define TINY_GSM_MODEM_SIM900
//#define TINY_GSM_MODEM_A6
//#define TINY_GSM_MODEM_M590

#include <TinyGsmClient.h>
#include <BlynkSimpleSIM800.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
//const char auth[] = "2f4fcc4b571343c182cb46318f073427";
//#define AUTH_KEY "2f4fcc4b571343c182cb46318f073427"

// Your GPRS credentials
// Leave empty, if missing user or pass
const char apn[]  = "internet.t-mobile";
const char user[] = "t-mobile";
const char pass[] = "tm";

// Hardware Serial on Mega, Leonardo, Micro
#define SerialAT Serial1

TinyGsm modem(SerialAT);

WidgetMap myMap(V1);


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

  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);

  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);
  
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

  digitalWrite(8, LOW);

  //rtc.begin();
  
}

void sim808_loop()
{
  //TRACE_PRINTLN(F("#->sim808_loop"));

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
      
      DEBUG_PRINT(F("#GPS Location: LAT: "));
      DEBUG_PRINT(gps_latitude);
      DEBUG_PRINT(F(" LONG: "));
      DEBUG_PRINT(gps_longitude);
      DEBUG_PRINT(F(" SPEED: "));
      DEBUG_PRINT(gps_speed);
      DEBUG_PRINT(F(" ALTITUDE: "));
      DEBUG_PRINT(gps_altitude);
      DEBUG_PRINT(F(" VIEWED SATELITES: "));
      DEBUG_PRINT(gps_view_satellites);
      DEBUG_PRINT(F(" USED STELITES: "));
      DEBUG_PRINTLN(gps_used_satellites);

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
      
      DEBUG_PRINT(F("#Distance is: "));
      DEBUG_PRINTLN(distance);

      if (gps_speed > 10) {
        gps_distance += (int)(distance + .5);
        gps_distance_trip += (int)(distance + .5);
        //gps_distance_today += (int)(distance + .5);
        //gps_distance_start += (int)(distance + .5);
        
        gps_latitude_old = gps_latitude;
        gps_longitude_old = gps_longitude;
      }

      
    }
    else {
      DEBUG_PRINTLN(F("#gps not fix"));
      //delay(1000);
    }
  }

  if (blynk) {
    //delay(500);
    Blynk.run();
    if ( sim808_blynk_timer < millis() ) {
      DEBUG_PRINTLN(F("#set location"));
       
      myMap.location(1, gps_latitude, gps_longitude, "Bus");
      i++;
      DEBUG_PRINTLN(F("#location is set"));
      Blynk.virtualWrite(V2, gps_used_satellites);
      
      sim808_blynk_timer = millis() + SIM808_BLYNK_TIMER;
    }
    Blynk.run();  
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
        
        //delay(10000);
        //Blynk.begin(AUTH_KEY, modem, apn, user, pass);
        
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
        //Blynk.begin(BLYNK_KEY, modem, SIM_APN, SIM_USER, SIM_PASS);
        if (Blynk.connect()) {
          //Serial.println(F("BLYNK is connected."));
          blynk = true;

          Blynk.notify(F("Hello. Now I'm online!"));
        }
        else  {
          //modem.sendSMS(sender, F("Coldn't connect to Blynk Server"));
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
        Blynk.disconnect();
        if (modem.gprsDisconnect()) Serial.println(F("Disconnect ok"));
        //if (modem.gprsDisconnect()) Serial.println(F("Disconnect failed"));
        //if (modem.gprsDisconnect()) Serial.println(F("Disconnect failed"));
        
        modem.deleteSMS(i);
        Serial.println(F("Disconnected"));

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

