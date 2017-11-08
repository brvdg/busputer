/***************************************************
 *  This code is based on the SdFat library from Bill Greiman.
 *  GIT: https://github.com/greiman/SdFat
 *
 *  Author: Brun
 *  
 ****************************************************/

#ifdef SDCARD

#include <SPI.h>
#include "SdFat.h"

SdFat SD;
SdFile logfile;



//File configfile;

void enable_sdcard() {
  //#ifdef SDCARD
  TRACE_PRINTLN(F("#->enable_sdcard"));
  if ( !SPI_lock ) {
    SPI_lock = true;
    for (int i = 0; i<25; i++) {

/*      SPI_lock = false;
      display_bootmsg(F("format SD Card?"));
      display_bootmsg(F("hold button for format"));
      display_bootmsg(F("int the next 3sec"));
      SPI_lock = true;
      delay(3000);
      
      if (digitalRead(BUTTON_PIN_1) == LOW) {
        display_bootmsg(F("okay. formating SD"));
        display_bootmsg(F("release button to abort"));
        delay(1000);
        if (digitalRead(BUTTON_PIN_1) == LOW) {
          display_bootmsg(F("in 3"));
          delay(1000);
          if (digitalRead(BUTTON_PIN_1) == LOW) {
            display_bootmsg(F("in 2"));
            delay(1000);
            if (digitalRead(BUTTON_PIN_1) == LOW) {
              display_bootmsg(F("in 1"));
              delay(1000);
              if (digitalRead(BUTTON_PIN_1) == LOW) {
                display_bootmsg(F("formating ..."));
                delay(3000);
                display_bootmsg(F("SHIT!!!"));
                display_bootmsg(F("This function isn't"));
                display_bootmsg(F("implemented. Yet..."));
                delay(3000);
              } else {
                display_bootmsg(F("aborting ..."));
                delay(3000);
              }
            } else {
              display_bootmsg(F("aborting ..."));
              delay(3000);
            }
          } else {
            display_bootmsg(F("aborting ..."));
            delay(3000);
          }
        } else {
          display_bootmsg(F("aborting ..."));
          delay(3000);
        }
      }
*/

      SPI_lock = false;
      display_bootmsg(F("Init SD Card"));
      SPI_lock = true;
      // initialize the SD card at SPI_HALF_SPEED to avoid bus errors with
      // breadbords.  use SPI_FULL_SPEED for better performance.
      if (!SD.cardBegin(SD_CS, SPI_FULL_SPEED)) {
        SPI_lock = false;
        display_bootmsg(F("cardBegin failed"));
        SPI_lock = true;
        delay(500);
        //return;
      }
      else {
        i = 100;
        SDmount = true;
        SPI_lock = false;
        display_bootmsg(F("SD Card ok"));
        SPI_lock = true;
      }
    }
    if (!SD.fsBegin()) {
      SPI_lock = false;
      display_bootmsg(F("FS init failed."));
      SPI_lock = true;
      delay(1000);
      //return;
      SDmount = false;
      NVIC_SystemReset();      // processor software reset
    }
    SPI_lock = false;
  }

  //open_config();
  
  //#endif
}

void sdcard_open_config() {
  TRACE_PRINTLN(F("#->open_config"));
  String tmp;
  int n;

  display_bootmsg(F("Open Config"));

  SdFile rdfile("config.txt", O_READ);

  // check for open error
  if (!rdfile.isOpen()) {
    display_bootmsg(F("can't read config"));
  }

  while ((n = rdfile.fgets(replybuffer, sizeof(replybuffer))) > 0) {
    tmp = String(replybuffer);
    //DEBUG_PRINTLN(tmp);
    if ( tmp.startsWith("#") ) {
      //DEBUG_PRINTLN(F("found comment"));
    }
    // Speed
    //else if ( tmp.startsWith("speed_offset=") ) {
      //DEBUG_PRINTLN(F("found config for speed_offset"));
      //DEBUG_PRINTLN(getValue( tmp, '=', 1 ));
    //}
    //else if ( tmp.startsWith("speed_source=") ) {
      //DEBUG_PRINTLN(F("found config for speed_source"));
      //speed_source = getValue( tmp, '=', 1 ).toInt();
      //DEBUG_PRINTLN(getValue( tmp, '=', 1 ));
    //}
    else if ( tmp.startsWith("rpm_multipl=") ) {
      //DEBUG_PRINTLN(F("found config for rpm_multipl"));
      rpm_multipl = getValue( tmp, '=', 1 ).toFloat();
      //DEBUG_PRINTLN(getValue( tmp, '=', 1 ));
    }
    //else if ( tmp.startsWith("temp_out=") ) {
      //DEBUG_PRINTLN(F("found config temp_out"));
      //DEBUG_PRINTLN(getValue( tmp, '=', 1 ));
    //}
    //else if ( tmp.startsWith("rpm_multip=") ) {
      //DEBUG_PRINTLN(F("found config"));
      //DEBUG_PRINTLN(getValue( tmp, '=', 1 ));
    //}
    //else if ( tmp.startsWith("rpm_multip=") ) {
      //DEBUG_PRINTLN(F("found config"));
      //DEBUG_PRINTLN(getValue( tmp, '=', 1 ));
    //}
    // TinyGSM
    else if ( tmp.startsWith("sim_apn=") ) {
      //DEBUG_PRINTLN(F("found sim_apn"));
      sim_apn=getValue( tmp, '=', 1 );
      //DEBUG_PRINTLN(getValue( tmp, '=', 1 ));
    }
    else if ( tmp.startsWith("sim_user=") ) {
      //DEBUG_PRINTLN(F("found sim_user"));
      sim_user=getValue( tmp, '=', 1 );
      //DEBUG_PRINTLN(getValue( tmp, '=', 1 ));
    }
    else if ( tmp.startsWith("sim_pass=") ) {
      //DEBUG_PRINTLN(F("found sim_pass"));
      sim_pass=getValue( tmp, '=', 1 );
      //DEBUG_PRINTLN(getValue( tmp, '=', 1 ));
    }
    else if ( tmp.startsWith("blynk_key=") ) {
      //DEBUG_PRINTLN(F("found blynk_key"));
      blynk_key=getValue( tmp, '=', 1 );
      //DEBUG_PRINTLN(getValue( tmp, '=', 1 ));
    }
    else {
      //INFO_PRINT(F("unknown config: "));
      //INFO_PRINTLN(tmp);
      for (int i = 0; i <= (sizeof(port_config) / sizeof(port_config[0])) - 1; i++){
        //Serial.println(port_config[i].name);
        //Serial.println(port_config[i].desc);
        //Serial.println(*port_config[i].port);
        if ( tmp.startsWith(port_config[i].name) ) {
          DEBUG_PRINT(F("found "));
          DEBUG_PRINTLN(port_config[i].desc);
          tmp = getValue( tmp, '=', 1 );
          *port_config[i].port = tmp.toInt();
          //delay(1500);
        }
        
      }
    }


    lastfile = lastfile_config * 10;
    
    
  }
  //delay(5000);


  //TRACE_PRINTLN(F("#print pointer..."));
  /*Serial.println(sizeof(port_config));
  Serial.println(sizeof(port_config[0]));
  Serial.println(sizeof(port_config) / sizeof(port_config[0]));
  for (int i = 0; i <= (sizeof(port_config) / sizeof(port_config[0])) - 1; i++){
    Serial.println(port_config[i].name);
    Serial.println(port_config[i].desc);
    Serial.println(*port_config[i].port);
  }*/

  //delay( 5000 );
  
}

void log_to_sdcard() {
  if ( log_timer < millis() ) {
    log_timer = millis() + LOG_TIMER;
    TRACE_PRINTLN(F("#->log_to_sdcard"));
    if ( !SDmount ) return;
    if ( !SPI_lock ) {
      SPI_lock = true;
      digitalWrite(8, HIGH);
      

      if (filename[0] == '-') {
        Serial.println(F("#no file opened"));
      }
      else {
        if ( gps_fixstatus ) {
          logfile.print(gps_longitude, DEC);
          logfile.print(F(","));
          logfile.print(gps_latitude, DEC);
          logfile.print(F(","));
          logfile.println(gps_altitude, DEC);
          logfile.flush();
        }


        minute = rtc.getMinutes();
        hour = rtc.getHours();
        day = rtc.getDay();
        month = rtc.getMonth();
        year = 2000 + rtc.getYear();

        long running = engine_running_total + engine_running_sec;
  
        logfile.print(F("<!--"));
        logfile.print(year);
        logfile.print(F("/"));
        logfile.print(month);
        logfile.print(F("/"));
        logfile.print(day);
        logfile.print(F(" "));
        logfile.print(hour);
        logfile.print(F(":"));
        logfile.print(minute);
        logfile.print(F(":"));
        logfile.print(second);
        logfile.print(F(","));
        logfile.print(gps_fixstatus);
        logfile.print(F(","));
        logfile.print(gps_latitude, DEC);
        logfile.print(F(","));
        logfile.print(gps_longitude, DEC);
        logfile.print(F(","));
        logfile.print(gps_altitude, DEC);
        logfile.print(F(","));
        logfile.print(gps_speed, DEC);
        logfile.print(F(","));
        logfile.print(gps_course, DEC);
        logfile.print(F(","));
        logfile.print(gps_view_satellites, DEC);
        logfile.print(F(","));
        logfile.print(gps_used_satellites, DEC);
        logfile.print(F(","));
        logfile.print(gps_distance, DEC);
        logfile.print(F(","));
        //logfile.print(bord_voltage, 1);
        //logfile.print(F(","));
        logfile.print(engine_running_total, DEC);
        logfile.print(F(","));
        
        logfile.print(running, DEC);
        logfile.print(F(","));
        logfile.println(F("-->"));
            
        logfile.flush();
        
        if (logfile.getWriteError()) {
          Serial.println(F("#write error"));
          SDerror = true;
        }
        if (!logfile.sync()) {
          Serial.println(F("#sync error"));
          SDerror = true;
        }
      }
      SPI_lock = false;
    }
    else {
      DEBUG_PRINTLN(F("#SPI Bus locked for log_to_sdcard"));
    }
    
    digitalWrite(8, LOW);
    
  }
}

void dump_sd_card() {
  //#ifdef SDCARD
  TRACE_PRINTLN(F("#->dump_sd_card"));
  
  display_bootmsg(F("Init SD Card"));
  if (!SD.begin(SD_CS)) {
    display_bootmsg(F("Card init. failed!"));
  }

  display_bootmsg(F("dumping ..."));

  File dir = SD.open("/");
  
  // Open next file in root.  The volume working directory, vwd, is root.
  // Warning, openNext starts at the current position of sd.vwd() so a
  // rewind may be neccessary in your application.
  SD.vwd()->rewind();
  
  while (logfile.openNext(SD.vwd(), O_READ)) {


    if (logfile.isDir()) {
      // Indicate a directory.
      Serial.write('/');
    }
    char chfilename[14];
    logfile.getName(chfilename, 13);
    Serial.println(chfilename);
    String filename = String(chfilename);
    int filetest = filename.indexOf(".KML");
    if ( filetest == 8 ) {
      display_bootmsg(F("dumping "));
      display_bootmsg(chfilename);
      while (logfile.available()) {
        Serial.write(logfile.read());
      }
    }


    logfile.close();
  }

  Serial.println(F("#Dump Complete"));

  //#endif
}





void get_last_log(void) {
  //#ifdef SDCARD
  TRACE_PRINTLN(F("#->get_last_log"));
  display_bootmsg(F("Check Files"));

  if ( !SDmount ) return;
  
  if ( !SPI_lock ) {
    SPI_lock = true;
    
    char filename_tmp[16];
    char filename[16]; 
    char ch;
    int field = 0;
    int i=0;
    boolean comment = false;
    boolean fileclosed = false;
    byte nofound_cnt = 0;
  
  

    //strcpy(filename_tmp, "TRACK000.LOG");
    strcpy(filename_tmp, "TRACK000.KML");
    for (uint16_t i = (lastfile_config * 10); i < 1000; i++) {
      filename_tmp[5] = '0' + i / 100;
      filename_tmp[6] = '0' + i / 10 - (i / 100 * 10);
      filename_tmp[7] = '0' + i % 10;
      TRACE_PRINT(F("#Check file: "));
      TRACE_PRINT(filename_tmp);
      if (! SD.exists(filename_tmp)) {
        TRACE_PRINTLN(F(" doesn't exist"));
        //TRACE_PRINT(filename_tmp);
        //break;
        nofound_cnt++;
        if ( nofound_cnt >= 50 ) break;
      }
      else {
        TRACE_PRINTLN(F(" exist"));
        strcpy(filename, filename_tmp);
        lastfile = i;
        lastfile_config = i / 10;
      }
    }

    INFO_PRINT(F("#Last Log:"));
    INFO_PRINTLN(filename);
    File dataFile = SD.open(filename);
    unsigned long filesize = dataFile.size();

    DEBUG_PRINT(F("#FileSize: "));
    DEBUG_PRINTLN(filesize);

    filesize -= 500;
    dataFile.seek(filesize);
    if (dataFile) {
      while (dataFile.available()) {
        ch = dataFile.read();
        //Serial.print(ch);
        if (ch == '\n') {
          field = 0;
          i = 0;
          //clear the buffer
          for (i = 0; i < 200; i++) {
            replybuffer[i] = '\0';
          }
          comment = true;
        } else {
          // searching for field delimiter
          if (ch == ',') {
            if (comment) {

              //Serial.print("###");
              //Serial.print(field,DEC);
              //Serial.println(replybuffer);
              
              switch (field) {
                
                case 2: 
                  gps_latitude = atof(replybuffer); 
                  DEBUG_PRINT(F("#gps_latitude: "));
                  DEBUG_PRINTLN(gps_latitude);
                  break;
                case 3: 
                  gps_longitude = atof(replybuffer); 
                  DEBUG_PRINT(F("#gps_longitude: "));
                  DEBUG_PRINTLN(gps_longitude);
                  break;
                case 9: 
                  gps_distance = atoi(replybuffer); 
                  DEBUG_PRINT(F("#gps_distance: "));
                  DEBUG_PRINTLN(gps_distance);
                  break;
                case 11:
                  engine_running_total = atoi(replybuffer);
                  DEBUG_PRINT(F("#engine_running_total: "));
                  DEBUG_PRINTLN(engine_running_total);
              }

              //clear the buffer
              for (i = 0; i < 200; i++) {
                replybuffer[i] = '\0';
              }
              
              field++;
              i = 0;
            }

            
          } 
          else if ( ch == '!' ) {
            //Serial.print("comment found");
            comment = true;
          }
          else if ( ch == '%' ) {
            fileclosed = true;
            INFO_PRINTLN("#last file is closed");
          }
          else {
            replybuffer[i] = ch;
            i++;
            /*Serial.println(replybuffer);
            Serial.println(strcmp(replybuffer, "</kml>"));
            if(strcmp(replybuffer, "</kml>") > 1) {
              INFO_PRINTLN("#last file is closed");
              fileclosed = true;
            }*/
            
          }
        }
      
      }
      dataFile.close();

      if ( !fileclosed ) {
        INFO_PRINTLN(F("#last file is not closed..."));
        if (!dataFile.open(filename, O_RDWR | O_CREAT | O_AT_END)) {
          SD.errorHalt(F("opening test.txt for write failed"));
          //delay(2000);
        }
        else {
          dataFile.println(F("</coordinates>"));
          dataFile.println(F("</LineString>"));
          dataFile.println(F("</Placemark>"));
          dataFile.println(F("</kml>"));
          dataFile.print(F("<!--%-->"));
          Serial.println(F("#last file now closed"));
        }
        dataFile.close();
      }
  
      //Serial.println(gps_distance);
    }
    SPI_lock = false;  
  }
  else {
    TRACE_PRINTLN(F("#SPI locked"));
  }
  //#endif
}

void open_file() {
  //#ifdef SDCARD
  TRACE_PRINTLN(F("#->open_file"));

  if ( !SDmount ) return;
  
  if ( !SPI_lock ) {
    SPI_lock = true;

    // make sure that no other file is opened
    //logfile.close();

    // set date time callback function
    SdFile::dateTimeCallback(dateTime); 

    /*#ifdef KMLLOG
    strcpy(filename, "TRACK000.KML");
    #else
    strcpy(filename, "TRACK000.LOG");
    #endif*/

    strcpy(filename, "TRACK000.KML");
    
    for (uint16_t i = lastfile; i < 1000; i++) {
      filename[5] = '0' + i / 100;
      filename[6] = '0' + i / 10 - (i / 100 * 10);
      filename[7] = '0' + i % 10;
      TRACE_PRINT(F("#Check file: "));
      TRACE_PRINTLN(filename);
      if (! SD.exists(filename)) {
        lastfile = i;
        lastfile_config = i / 10;
        break;
      }
    }

    //INFO_PRINT(F("#Last Log:"));
    //INFO_PRINTLN(filename);
    
    //logfile = SD.open(filename, O_RDWR | O_CREAT | O_AT_END);
    //logfile.open(filename, O_WRITE);
    if ( !logfile.open(filename, O_RDWR | O_CREAT | O_AT_END) ) {
      INFO_PRINT(F("#Couldnt create "));
      INFO_PRINTLN(filename);
      //error(3);
    }
    INFO_PRINT("#Writing to ");
    INFO_PRINTLN(filename);
    #ifdef KMLLOG
    logfile.println(F("<?xml version=\"1.0\" encoding=\"UTF-8\"?>"));
    logfile.println(F("<kml xmlns=\"http://earth.google.com/kml/2.2\">"));
    logfile.println(F("<Placemark>"));
    logfile.println(F("<LineString>"));
    logfile.println(F("<coordinates>"));
    #else //KMLLOG
    logfile.println(F("#start engine"));
    #endif //KMLLOG

    SPI_lock = false;
    TRACE_PRINTLN(F("#SPI unlock"));  

    log_to_sdcard();

  }
  //#endif //LCD
}

void close_file() {
  //#ifdef SDCARD
  TRACE_PRINTLN(F("#->close_file"));

  if ( !SDmount ) return;
  
  if ( !SPI_lock ) {
    SPI_lock = true;
    #ifdef KMLLOG
    logfile.println(F("</coordinates>"));
    logfile.println(F("</LineString>"));
    logfile.println(F("</Placemark>"));
    logfile.println(F("</kml>"));
    logfile.print(F("<!--%-->"));
    #else
    logfile.println(F("#stop engine"));
    #endif
    logfile.close();
    DEBUG_PRINT(F("#close file "));
    DEBUG_PRINTLN(filename);
    strcpy(filename, "-");
    SPI_lock = false;  
  }
  //#endif
}

void sdcard_save_config() {
  TRACE_PRINTLN(F("#->save_config"));

  if ( !SDmount ) return;
  
  if ( !SPI_lock ) {
    SPI_lock = true;

    if ( !SD.remove("config.txt")) {
      INFO_PRINTLN(F("#can't remove config.txt"));
    }

    SdFile::dateTimeCallback(dateTime); 

    if ( !logfile.open("config.txt", O_RDWR | O_CREAT | O_AT_END) ) {
      INFO_PRINTLN(F("#Couldnt create config.txt"));
    }
    else {
    
      INFO_PRINTLN("#Writing config.txt");

      logfile.println(F("#This is the configuration file"));

      // TinyGSM
      //String sim_apn = SIM_APN;
      logfile.println(F("#Provider APN"));
      logfile.print(F("sim_apn="));
      logfile.println(sim_apn);
      //String sim_user = SIM_USER;
      logfile.println(F("#username for the APN"));
      logfile.print(F("sim_user="));
      logfile.println(sim_user);
      //String sim_pass = SIM_PASS;
      logfile.println(F("#password for the APN"));
      logfile.print(F("sim_pass="));
      logfile.println(sim_pass);
      //String blynk_key = BLYNK_KEY;
      logfile.println(F("#BLYNK authentication key"));
      logfile.print(F("blynk_key="));
      logfile.println(blynk_key);

      /*logfile.println(F("#define the speed source"));
      logfile.print(F("speed_source="));
      logfile.println(speed_source, DEC);
      logfile.println(F("#define the speedpulse port"));
      logfile.print(F("speedpulse_port="));
      logfile.println(speedpulse_port, DEC);
      logfile.println(F("#how many offset should get the speed"));
      logfile.print(F("speed_offset="));
      //logfile.println(speed_offset, DEC);

      //byte bord_voltage_port = BORD_VOLTAGE_PORT;
      logfile.println(F("#port for the bord voltage"));
      logfile.print(F("bord_voltage_port="));
      logfile.println(bord_voltage_port, DEC);

      //byte dimmer_port = DIMMER;
      logfile.println(F("#port for the bord voltage"));
      logfile.print(F("dimmer_port="));
      logfile.println(dimmer_port, DEC);

      //byte water_temp_port = WATER_TEMP;
      logfile.println(F("#port for the bord voltage"));
      logfile.print(F("water_temp_port="));
      logfile.println(water_temp_port, DEC);

      //byte speedpulse_port = SPEEDPULSE;
      logfile.println(F("#port for the bord voltage"));
      logfile.print(F("speedpulse_port="));
      logfile.println(speedpulse_port, DEC);

      //byte speed_source = SPEEDSOURCE;
      logfile.println(F("#which source should be used for speedsource"));
      logfile.println(F("#0=GPS+Speedpulse"));
      logfile.println(F("#1=Speedpulse"));
      logfile.println(F("#2=GPS"));
      logfile.print(F("speed_source="));
      logfile.println(speed_source, DEC);*/

      for (int i = 0; i <= (sizeof(port_config) / sizeof(port_config[0])) - 1; i++){
        //Serial.println(port_config[i].name);
        //Serial.println(port_config[i].desc);
        //Serial.println(*port_config[i].port);

        logfile.print(F("# "));
        logfile.println(port_config[i].desc);
        logfile.print(port_config[i].name);
        logfile.print(F("="));
        logfile.println(*port_config[i].port, DEC);
      }
      
      
      logfile.close();
    }
      
    SPI_lock = false;
    INFO_PRINTLN("#OK");
  }

  //delay(3000);
}


/* 
 * call back for file timestamps 
 */
void dateTime(uint16_t* date, uint16_t* time) {
  #ifdef SDCARD
  TRACE_PRINTLN(F("#->dateTime"));

  /* using arduino RTC
  while(I2C_lock);
  I2C_lock = true;
  DateTime now = rtc.now();
  I2C_lock = false;

  minute = now.minute();
  hour = now.hour();
  day = now.day();
  month = now.month();
  year = now.year();
  */
  
  minute = rtc.getMinutes();
  hour = rtc.getHours();
  day = rtc.getDay();
  month = rtc.getMonth();
  year = 2000 + rtc.getYear();

  
  // return date using FAT_DATE macro to format fields
  *date = FAT_DATE(year, month, day);

  // return time using FAT_TIME macro to format fields
  *time = FAT_TIME(hour, minute, second);
  #endif
}


#endif //SDCARD

