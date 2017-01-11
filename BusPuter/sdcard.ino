/***************************************************
 *  This code is based on the SdFat library from Bill Greiman.
 *  GIT: https://github.com/greiman/SdFat
 *
 *  Author: Brun
 *  
 ****************************************************/

#ifdef SDCARD

void enable_sdcard() {
  #ifdef SDCARD
  TRACE_PRINTLN(F("#->enable_sdcard"));
  if ( !SPI_lock ) {
    SPI_lock = true;
    for (int i = 0; i<25; i++) {

      display_bootmsg(F("Init SD Card"));
      // initialize the SD card at SPI_HALF_SPEED to avoid bus errors with
      // breadboards.  use SPI_FULL_SPEED for better performance.
      if (!SD.cardBegin(cardSelect, SPI_FULL_SPEED)) {
        display_bootmsg(F("cardBegin failed"));
        delay(500);
        //return;
      }
      else {
        i = 100;
        SDmount = true;
        display_bootmsg(F("SD Card ok"));
      }
    }
    if (!SD.fsBegin()) {
      display_bootmsg(F("File System initialization failed."));
      delay(3000);
      //return;
      SDmount = false;
    }
    SPI_lock = false;
  }
  
  #endif
}

void log_to_sdcard() {
  /*
   * Logging format (not for KML)
   * Default values
   * utctime(yyyymmddHHMMSS),fixstatus,latitude,longitude,altitude,speed,course,view_satellites,used_satellites,distance,board_voltage
   * Debugging informations starting with a "#"
   * optional logging which are not standart starting with two "//" ore some other shit. I don't know yet.
   */
  #ifdef SDCARD
  TRACE_PRINTLN(F("#->log_to_sdcard"));
  if ( !SPI_lock ) {
    SPI_lock = true;
    digitalWrite(8, HIGH);
    #ifdef KMLLOG

    if (filename[0] == '-') {
      Serial.println(F("#no file opened"));
    }
    else {
    
      //Serial.println(F("#log to SD Card"));
      logfile.print(gps_longitude, DEC);
      logfile.print(F(","));
      logfile.print(gps_latitude, DEC);
      logfile.print(F(","));
      logfile.println(gps_altitude, DEC);
      logfile.flush();
      //#else

      logfile.print(F("<!--"));
      logfile.print(utc_time);
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
      logfile.print(board_voltage, 1);
      logfile.print(F(","));
      logfile.print(engine_running_total, DEC);
      logfile.print(F(","));
      logfile.println(F("-->"));
          
      logfile.flush();
      if (!logfile.sync() || logfile.getWriteError()) {
        Serial.println(F("#write error"));
        SDerror = true;
      }
      else {
        //Serial.println(F("#write ok"));
      }
    }
    SPI_lock = false;
  }
  else {
    DEBUG_PRINTLN(F("#SPI Bus locked"));
    //Serial.println(F("#SPI Bus locked"));
  }
  digitalWrite(8, LOW);
  #endif
  #endif
}

void dump_sd_card() {
#ifdef SDCARD
  TRACE_PRINTLN(F("#->dump_sd_card"));
  
  display_bootmsg(F("Init SD Card"));
  if (!SD.begin(cardSelect)) {
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

#endif
}


/* 
 * call back for file timestamps 
 */
void dateTime(uint16_t* date, uint16_t* time) {
  #ifdef SDCARD
  TRACE_PRINTLN(F("#->dateTime"));
  // return date using FAT_DATE macro to format fields
  *date = FAT_DATE(gps_year, gps_month, gps_day);

  // return time using FAT_TIME macro to format fields
  *time = FAT_TIME(gps_hour, gps_minute, gps_second);
  #endif
}

void get_last_log(void) {
  #ifdef SDCARD
  TRACE_PRINTLN(F("#->get_last_log"));
  display_bootmsg(F("Check Files"));
  if ( !SPI_lock ) {
    SPI_lock = true;
    
    char filename_tmp[16];
    char filename[16];
    char ch;
    int field = 0;
    int i=0;
    boolean comment = false;
    boolean fileclosed = false;
  
  

    //strcpy(filename_tmp, "TRACK000.LOG");
    strcpy(filename_tmp, "TRACK000.KML");
    for (uint16_t i = 0; i < 1000; i++) {
      filename_tmp[5] = '0' + i / 100;
      filename_tmp[6] = '0' + i / 10 - (i / 100 * 10);
      filename_tmp[7] = '0' + i % 10;
      TRACE_PRINT(F("#Check file: "));
      TRACE_PRINT(filename_tmp);
      if (! SD.exists(filename_tmp)) {
        TRACE_PRINTLN(F(" doesn't exist"));
        //TRACE_PRINT(filename_tmp);
        break;
      }
      else {
        TRACE_PRINTLN(F(" exist"));
        strcpy(filename, filename_tmp);
        lastfile = i;
      }
    }

    INFO_PRINT(F("#Last Log:"));
    INFO_PRINTLN(filename);
    File dataFile = SD.open(filename);
    unsigned long filesize = dataFile.size();
  
    Serial.println(filesize);

    filesize -= 500;
    dataFile.seek(filesize);
    if (dataFile) {
      while (dataFile.available()) {
        ch = dataFile.read();
        if (ch == '\n') {
          field = 0;
          i = 0;
          comment = true;
        } else {
          if (ch == ',') {
            
            if (comment) {
              switch (field) {
                case 2: 
                  gps_latitude = atof(replybuffer); 
                  break;
                case 3: 
                  gps_longitude = atof(replybuffer); 
                  break;
                case 9: 
                  gps_distance = atoi(replybuffer); 
                  break;
                case 11:
                  engine_running_total_last = atoi(replybuffer);
              }
            } 
            else if ( ch == '!' ) {
              comment = true;
            }
            else if ( ch == 'l' ) {
              fileclosed = true;
              INFO_PRINTLN("#last file is closed");
            }

          
            for (i = 0; i < 200; i++) {
              replybuffer[i] = '\0';
            }
            field++;
            i = 0;
            
          }
          else {
            replybuffer[i] = ch;
            i++;
          }
        }
      
      }
      dataFile.close();

      if ( !fileclosed ) {
        INFO_PRINTLN(F("#last file is not closed..."));
        if (!dataFile.open(filename, O_RDWR | O_CREAT | O_AT_END)) {
          SD.errorHalt("opening test.txt for write failed");
        }
        else {
          dataFile.println(F("</coordinates>"));
          dataFile.println(F("</LineString>"));
          dataFile.println(F("</Placemark>"));
          dataFile.println(F("</kml>"));
          Serial.println(F("#last file now closed"));
        }
      }
  
      Serial.println(gps_distance);
    }
    SPI_lock = false;  
  }
  else {
    TRACE_PRINTLN(F("#SPI locked"));
  }
  #endif
}

void open_file() {
#ifdef SDCARD
  TRACE_PRINTLN(F("#->open_file"));
  if ( !SPI_lock ) {
    SPI_lock = true;

    // make sure that no other file is opened
    logfile.close();

    // set date time callback function
    SdFile::dateTimeCallback(dateTime); 

    #ifdef KMLLOG
    strcpy(filename, "TRACK000.KML");
    #else
    strcpy(filename, "TRACK000.LOG");
    #endif

    for (uint16_t i = lastfile; i < 1000; i++) {
      filename[5] = '0' + i / 100;
      filename[6] = '0' + i / 10 - (i / 100 * 10);
      filename[7] = '0' + i % 10;
      TRACE_PRINT(F("#Check file: "));
      TRACE_PRINT(filename);
      if (! SD.exists(filename)) {
        break;
      }
    } 

    //logfile = SD.open(filename, O_RDWR | O_CREAT | O_AT_END);
    if ( !logfile.open(filename, O_RDWR | O_CREAT | O_AT_END) ) {
      Serial.print("#Couldnt create ");
      Serial.println(filename);
      //error(3);
    }
    Serial.print("#Writing to ");
    Serial.println(filename);
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
  #endif //LCD
}

void close_file() {
  #ifdef SDCARD
  TRACE_PRINTLN(F("#->close_file"));
  if ( !SPI_lock ) {
    SPI_lock = true;
    #ifdef KMLLOG
    logfile.println(F("</coordinates>"));
    logfile.println(F("</LineString>"));
    logfile.println(F("</Placemark>"));
    logfile.println(F("</kml>"));
    #else
    logfile.println(F("#stop engine"));
    #endif
    logfile.close();
    Serial.print("#close file ");
    Serial.println(filename);
    strcpy(filename, "-");
    SPI_lock = false;  
  }
#endif
}

#endif //SDCARD

