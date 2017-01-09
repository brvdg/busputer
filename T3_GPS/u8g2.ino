#ifdef LCD

#ifdef ARDUINO_ARCH_SAMD

#define Xpos 0
#define Ypos 14

//Menü Definationen
#define MaxMENU 7
#define MENU1 menu_uhr
#define MENU2 menu_speed
#define MENU3 menu_gpsinfos // jump to SubMenu3
#define MENU4 menu_power
#define MENU5 menu_speedinfos
#define MENU6 menu_tripinfos
#define MENU7 menu_sdcard

#define MaxEndMENU 1
//#define EndMENU1 menu_dimmer
#define EndMENU1 menu_info // jump to SubMenu1

#define SubMenu1Nr 101
#define SubMenu1 menu_sub_1
#define SubMenu2Nr 102
#define SubMenu2 menu_sub_2
#define SubMenu3Nr 104
#define SubMenu3 menu_gpsinfos_sub1


/*
   https://github.com/olikraus/u8glib/wiki/fontsize
   ACHTUNG: Fonts brauchen sehr viel Platz!!!
*/

#define small_font u8g2_font_profont10_tf //2311
#define medium_font u8g2_font_profont12_tf //2311
#define big_font u8g2_font_profont29_mn //2719

int last_menu = 0;

//#ifdef LCD
//Software SPI
//U8G2_UC1701_EA_DOGS102_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//Hardware SPI
U8G2_UC1701_EA_DOGS102_1_4W_HW_SPI u8g2(U8G2_R0, 12, 13);

//U8G2_ST7565_EA_DOGM128_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
//U8G2_ST7565_EA_DOGM128_1_4W_HW_SPI u8g2(U8G2_R0, 12, 13);
#endif

void init_display(void) {
#ifdef LCD
  if ( !SPI_lock ) {
    SPI_lock = true;
    u8g2.begin();
    //u8g2.setContrast(220);
    pinMode(LCD_LED, OUTPUT);
    analogWrite(LCD_LED, 128);
    SPI_lock = false;
  }
#endif
}


void display_loop() {
  //TRACE_PRINTLN(F("#->display_loop"));
  //if ( lcd_update_timer < millis() ) {
    //Serial.println(F("#it's time for display update"));
    //lcd_update_timer = millis() + LCD_UPDATE_TIMER;
    if ( !lcd_update_timer_lock ) {
      if ( !SPI_lock ) {
        SPI_lock = true;
        lcd_update_timer_lock = true;
        display_update();
        lcd_update_timer_lock = false;
        SPI_lock = false;
      } else {
        //DEBUG_PRINTLN(F("#SPI Bus locked"));
        //Serial.println(F("#SPI Bus locked"));
      }
    }
    else {
      Serial.println(F("#display update locked..."));
    }
  //}
}



void draw(void) {
  //TRACE_PRINTLN(F("#->draw"));
  char buf[24];
  

  // graphic commands to redraw the complete screen should be placed here
  u8g2.setFont(small_font);

  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos);

  /*if (button_1 == 1 ) {
    //  MainMenuPos++;
    if ((MainMenuPos >= 90+MaxEndMENU) && (MainMenuPos < 100)) {
      MainMenuPos = 1;
    }
    else if ((MainMenuPos >= MaxMENU+1) && (MainMenuPos < 90)) {
      MainMenuPos = 90;
    }

    //button_1 = 0;
    }*/

  switch (MainMenuPos) {
    case 0: bootscreen(); break;
    case 1: MENU1(); break;
    case 2: MENU2(); break;
#ifdef MENU3
    case 3: MENU3(); break;
#endif
#ifdef MENU4
    case 4: MENU4(); break;
#endif
#ifdef MENU5
    case 5: MENU5(); break;
#endif
#ifdef MENU6
    case 6: MENU6(); break;
#endif
#ifdef MENU7
    case 7: MENU7(); break;
#endif
#ifdef MENU8
    case 8: MENU8(); break;
#endif

#ifdef EndMENU1
    case 90: EndMENU1(); break;
#endif

#ifdef SubMenu1
    case SubMenu1Nr: SubMenu1(); break;
#endif
#ifdef SubMenu2
    case SubMenu2Nr: SubMenu2(); break;
#endif
#ifdef SubMenu3
    case SubMenu3Nr: SubMenu3(); break;
#endif
    //default: MainMenuPos = last_menu;
    default: {
        MENU1();
        MainMenuPos = 1;
        break;
      }
  }
}

void display_update(void) {
#ifdef LCD
  // picture loop
  u8g2.firstPage();
  do {
    draw();
  } while ( u8g2.nextPage() );
#endif
}



/*
   Display MENUs

*/
void bootscreen() {
  //u8g.drawXBMP( 48, 0, vw_width, vw_height, vw_bits);
  u8g2.setFontPosTop();
  //u8g2.setCursor(24, 0);
  u8g2.setCursor(Xpos, Ypos);
  u8g2.print(bootmsg1);
  u8g2.setCursor(Xpos, Ypos + 8);
  u8g2.print(bootmsg2);
  u8g2.setCursor(Xpos, Ypos + 16);
  u8g2.print(bootmsg3);

}

void menu_speed() {
  char buf[24];

  // Speed
  u8g2.setFont(big_font);
  u8g2.setFontPosTop();
  //u8g2.setCursor(36, -2);
  u8g2.setCursor(Xpos + 18, Ypos);
  if (gps_success) {
    if (gps_speed >= 5) {
      sprintf (buf, "%3d", gps_speed + SPEED_OFFSET);
      u8g2.print(buf);
    }
    else {
      u8g2.print("  0");
    }
    //u8g2.print(buf);
  }
  else {
    u8g2.print(F("---"));
  }

  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos + 74, Ypos);
  u8g2.print(F("km/h"));

  //Distance
  /*u8g2.setFont(medium_font);
    u8g2.setFontPosTop();
    u8g2.setCursor(Xpos, Ypos + 24);
    //u8g.print(F("dist:"));
    uint16_t m = gps_distance - ( gps_distance/1000 ) * 1000;
    sprintf (buf, "%04d.%1d km", gps_distance/1000, m/100);
    u8g2.print(buf);*/

  //Temperature
  u8g2.setFont(medium_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos + 24);
  //u8g.print(F("dist:"));
  //uint16_t m = temp_out * 10;
  uint16_t m = temp_out - ( temp_out/10 ) * 10;
  sprintf (buf, "%02d.%1d", temp_out/10, m);
  //u8g2.print(F("\xb0:"));

  u8g2.print(buf);
  u8g2.print(F("\xb0"));
  u8g2.print(F("C"));

  //time
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos + 68, Ypos + 24);
  sprintf(buf, "%02d:%02d", hour, minute);
  u8g2.print(buf);

  switch (button_1) {
    case 1: MainMenuPos++; break;
    case 2: MainMenuPos--; break;
  }
  button_1 = 0;
}

void menu_uhr() {
  char buf[24];

  u8g2.setFont(big_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos + 10, Ypos);
  sprintf(buf, "%02d:%02d", hour, minute);
  u8g2.print(buf);

  u8g2.setFont(medium_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos + 22, Ypos + 24);
  sprintf(buf, "%02d.%02d.%04d", day, month, year);
  u8g2.print(buf);

  switch (button_1) {
    case 1: MainMenuPos++; break;
    case 2: MainMenuPos--; break;
  }
  button_1 = 0;
}

void menu_gpsinfos() {
  char buf[24];

  u8g2.setFont(medium_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos);
  u8g2.print(F("Lat : "));
  u8g2.print(gps_latitude, 5);
  u8g2.setCursor(Xpos, Ypos + 10);
  u8g2.print(F("Long: "));
  u8g2.print(gps_longitude, 5);
  u8g2.setCursor(Xpos, Ypos + 20);
  u8g2.print(F("Alt : "));
  u8g2.print(gps_altitude, DEC);

  switch (button_1) {
    case 1: MainMenuPos++; break;
    case 2: MainMenuPos = SubMenu3Nr; break;
  }
  button_1 = 0;

}

void menu_speedinfos() {
  char buf[24];

  u8g2.setFont(medium_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos);
  u8g2.print(F("Avg. Speed: "));
  u8g2.print(gps_speed_avg, DEC);
  u8g2.setCursor(Xpos, Ypos + 10);
  u8g2.print(F("Avg. Today: "));
  //u8g2.print(gps_speed_avg_today, DEC);
  u8g2.setCursor(Xpos, Ypos + 20);
  u8g2.print(F("Max. Speed: "));
  u8g2.print(gps_speed_max, DEC);

  switch (button_1) {
    case 1: MainMenuPos++; break;
      //case 2: MainMenuPos = SubMenu3Nr; break;
  }
  button_1 = 0;

}

void menu_tripinfos() {
  char buf[24];

  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos);
  u8g2.print(F("Trip Infos: "));
  u8g2.setCursor(Xpos, Ypos + 8);
  //u8g2.print(gps_trip_distance/1000, DEC);
  u8g2.print(F("\x97\x3e "));
  uint16_t m = gps_distance_trip - ( gps_distance_trip / 1000 ) * 1000;
  sprintf (buf, "%04d.%1d km", gps_distance_trip / 1000, m / 100);
  //u8g2.print(F(" km "));

  u8g2.print(buf);
  u8g2.setCursor(Xpos, Ypos + 16);
  u8g2.print(F("\xf8 "));
  u8g2.print(gps_speed_avg_trip, DEC);
  u8g2.print(F("km/h"));

  u8g2.setCursor(Xpos + 48, Ypos + 16);
  u8g2.print(F("\x86:"));
  u8g2.print(gps_speed_max_trip, DEC);
  u8g2.print(F("km/h"));

  u8g2.setCursor(Xpos, Ypos + 24);
  u8g2.print(engine_running_trip_last + engine_running_sec, DEC);

  switch (button_1) {
    case 1: MainMenuPos++; break;
    case 2:
      //gps_distance = 0;
      gps_distance_trip = 0;
      gps_speed_max_trip = 0;
      //gps_speed_trip_avg = 0;
      engine_running_trip_last = 0 - engine_running_sec;
      break;
  }
  button_1 = 0;

}

void menu_sdcard() {
#ifdef SDCARD
  char buf[24];

  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos);
  u8g2.print(F("SD Card Infos: "));

  u8g2.setCursor(Xpos, Ypos + 8);
  u8g2.print(F("Card: "));
  if ( SDmount && !SDerror) {
    u8g2.print(F("ready"));
  }
  else {
    u8g2.print(F("not ready"));
  }

  u8g2.setCursor(Xpos, Ypos + 16);
  u8g2.print(F("File: "));
  u8g2.print(filename);

  switch (button_1) {
    case 1: MainMenuPos++; break;
    case 2:
      button_1 = 0;
      if (SDmount) {
        close_file();
        SDmount = false;
      } else {
        MainMenuPos = 0;
        Serial.println(F("#remount_sdcard"));
        SDmount = false;
        delay(1000);
        enable_sdcard();
        MainMenuPos = 7;
      }
      break;
  }
  button_1 = 0;
#endif
}

void menu_gpsinfos_sub1() {
  char buf[24];

  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos);
  u8g2.print(F("Satellites"));
  u8g2.setCursor(Xpos + 6, Ypos + 8);
  u8g2.print(F("view  : "));
  u8g2.print(gps_view_satellites, DEC);
  u8g2.setCursor(Xpos + 6, Ypos + 16);
  u8g2.print(F("used  : "));
  u8g2.print(gps_used_satellites, DEC);
  u8g2.setCursor(Xpos + 6, Ypos + 24);
  u8g2.print(F("status: "));
  //u8g2.print(gps_fixstatus, DEC);
  if (gps_fixstatus == 1) {
    u8g2.print(F("fixed"));
  } else {
    u8g2.print(F("not fixed"));
  }
  //u8g2.print(F(")"));

  switch (button_1) {
    case 1: MainMenuPos = 3; break;
      //case 2: MainMenuPos = 3; break;
  }
  button_1 = 0;

}
void menu_power() {
  char buf[24];

  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos);
  u8g2.print(F("Boardspannung:"));
  u8g2.setFont(big_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos + 10, Ypos + 10);
  u8g2.print(board_voltage, 1);
  u8g2.setFont(medium_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos + 80, Ypos + 20);
  u8g2.print("V");

  switch (button_1) {
    case 1: MainMenuPos++; break;
    case 2: MainMenuPos--; break;
  }
  button_1 = 0;
}
void menu_dimmer() {
  char buf[24];

  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos);
  u8g2.print(F("Dimmer Pref"));
  sprintf(buf, "%3d%%", vbat);
  u8g2.print(buf);
  u8g2.setCursor(Xpos, Ypos + 10);
  u8g2.print(F("Hold Button to change"));
  //sprintf(buf, "%4d%V", X_pin);
  u8g2.print(board_voltage, 2);
  u8g2.setCursor(Xpos, Ypos + 16);

  switch (button_1) {
    case 1: MainMenuPos++; break;
    case 2: MainMenuPos--; break;
  }
  button_1 = 0;
}
void menu_info() {
  char buf[24];

  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos);
  u8g2.print(F("T3 Board Computer"));
  u8g2.setCursor(Xpos, Ypos + 8);
  u8g2.print(F("Designed by Brun"));
  u8g2.setCursor(Xpos, Ypos + 16);
  u8g2.print(F("Version: "));
  u8g2.print(F(VERSION));
  u8g2.setCursor(Xpos, Ypos + 24);
  u8g2.print(F("Bckp Batt:"));
  sprintf(buf, "%3d%%", vbat);
  u8g2.print(buf);

  /*if (button_1 == 2 ) {
    MainMenuPos = SubMenu1Nr;
    button_1 = 0;
    last_menu = 90;
    }*/

  switch (button_1) {
    case 1: MainMenuPos = 1; break;
    case 2: MainMenuPos--; break;
  }
  button_1 = 0;
}

void menu_sub_1() {
  char buf[24];

  u8g2.setFont(medium_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos);
  u8g2.print(F("Sub Menue 1"));

  switch (button_1) {
    case 1: MainMenuPos++; break;
    case 2: MainMenuPos--; break;
  }
  button_1 = 0;
}
void menu_sub_2() {
  char buf[24];

  u8g2.setFont(medium_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos);
  u8g2.print(F("Sub Menue 2"));

  switch (button_1) {
    case 1: MainMenuPos++; break;
    case 2: MainMenuPos--; break;
  }
  button_1 = 0;
}


void display_bootmsg(String msg) {
  bootmsg1 = bootmsg2;
  bootmsg2 = bootmsg3;
  bootmsg3 = msg;

  //bootmsg = "Waiting for Serial";
  display_update();

  Serial.print(F("#"));
  Serial.println(msg);
}

#else //LCD
/*void init_display(void) {
  //empty funktion to disable the LCD insted to use "#ifdef LCD" in the code
  }*/

/*void display_update(void) {
  //empty funktion to disable the LCD insted to use "#ifdef LCD" in the code
  }*/

void display_bootmsg(String msg) {
  Serial.print(F("#"));
  Serial.println(msg);
}

#endif //LCD
