/***************************************************
 *  This code is based on the U8G2 library from olikraus.
 *  GIT: https://github.com/olikraus/u8g2
 *       https://github.com/olikraus/U8g2_Arduino
 *  Wiki: https://github.com/olikraus/u8g2/wiki
 *
 *  Author: Brun von der Gönne
 *
 *  This module will only used by 32Bit processors.
 *
 ****************************************************/


#ifdef U8G2_DISPLAY
#ifdef ARDUINO_ARCH_SAMD
#ifdef DOGS102


/*
 *  https://github.com/olikraus/u8glib/wiki/fontsize
 *  ATTENTION!!! Fonts need space
 */

#define small_font u8g2_font_profont10_tf
#define small_font_high 8
#define medium_font u8g2_font_profont12_tf
#define medium_font_high 10
#define big_font u8g2_font_profont29_mn
#define big_font_high 24

// this defines the starting possition.
#define Xpos 0
#define Ypos 16
#define Xsize 102
#define Ysize 48

#define BIG_VAL_POS_X Xsize/2 + Xpos - 36
#define BIG_VAL_POS_Y Ysize/2 + Ypos - big_font_high/2
#define UNIT_POS_X BIG_VAL_POS_X + 36 + 30
#define UNIT_POS_Y Ysize/2 + Ypos - big_font_high/2


// Layout Nr. 1
#define LAYOUT1_1_X Xsize/2 + Xpos - 40
#define LAYOUT1_1_Y Ysize/2 + Ypos - big_font_high/2 - 12
#define LAYOUT1_2_X LAYOUT1_1_X
#define LAYOUT1_2_Y LAYOUT1_1_Y + big_font_high

// Layout Nr. 2
//BIG_VAL_POS_X, BIG_VAL_POS_Y-12
#define LAYOUT2_1_X Xsize/2 + Xpos - 36
#define LAYOUT2_1_Y Ysize/2 + Ypos - big_font_high/2 - 12
#define LAYOUT2_2_X LAYOUT2_1_X + 36 + 30
#define LAYOUT2_2_Y LAYOUT2_1_Y

#define LAYOUT2_3_X 4
#define LAYOUT2_3_Y LAYOUT2_2_Y + big_font_high
#define LAYOUT2_4_X 68
#define LAYOUT2_4_Y LAYOUT2_3_Y

// Layout Nr. 3
#define LAYOUT3_1_X Xpos
#define LAYOUT3_1_Y Ypos
#define LAYOUT3_2_X Xsize/2 + Xpos - 36
#define LAYOUT3_2_Y Ysize/2 + Ypos - big_font_high/2
#define LAYOUT3_3_X LAYOUT3_2_X + 36 + 30
#define LAYOUT3_3_Y Ysize/2 + Ypos - big_font_high/2

// Layout Nr. 4
#define LAYOUT4_1_X Xpos
#define LAYOUT4_1_Y Ypos
#define LAYOUT4_2_X Xpos + 12
#define LAYOUT4_2_Y LAYOUT4_1_Y + small_font_high + 2
#define LAYOUT4_3_X Xpos + 12
#define LAYOUT4_3_Y LAYOUT4_2_Y + medium_font_high + 2

// Layout Nr. 5
#define LAYOUT5_1_X Xpos
#define LAYOUT5_1_Y Ypos
#define LAYOUT5_2_X Xpos + 2
#define LAYOUT5_2_Y LAYOUT5_1_Y + small_font_high
#define LAYOUT5_3_X Xpos + 2
#define LAYOUT5_3_Y LAYOUT5_2_Y + small_font_high
#define LAYOUT5_4_X Xpos + 2
#define LAYOUT5_4_Y LAYOUT5_3_Y + small_font_high

#endif
#ifdef OLED

#define small_font u8g2_font_profont10_tf
#define small_font_high 8
#define medium_font u8g2_font_profont12_tf
#define medium_font_high 10
#define big_font u8g2_font_profont29_mn
#define big_font_high 24

// this defines the starting possition.
#define Xpos 0
#define Ypos 16
#define Xsize 128
#define Ysize 48

#define BIG_VAL_POS_X Xsize/2 + Xpos - 36
#define BIG_VAL_POS_Y Ysize/2 + Ypos - big_font_high/2
#define UNIT_POS_X BIG_VAL_POS_X + 36 + 30
#define UNIT_POS_Y Ysize/2 + Ypos - big_font_high/2


// Layout Nr. 1
#define LAYOUT1_1_X Xsize/2 + Xpos - 40
#define LAYOUT1_1_Y Ysize/2 + Ypos - big_font_high/2 - 12
#define LAYOUT1_2_X LAYOUT1_1_X
#define LAYOUT1_2_Y LAYOUT1_1_Y + big_font_high

// Layout Nr. 2
//BIG_VAL_POS_X, BIG_VAL_POS_Y-12
#define LAYOUT2_1_X Xsize/2 + Xpos - 36
#define LAYOUT2_1_Y Ysize/2 + Ypos - big_font_high/2 - 12
#define LAYOUT2_2_X LAYOUT2_1_X + 36 + 30
#define LAYOUT2_2_Y LAYOUT2_1_Y

#define LAYOUT2_3_X 4
#define LAYOUT2_3_Y LAYOUT2_2_Y + big_font_high
#define LAYOUT2_4_X 68
#define LAYOUT2_4_Y LAYOUT2_3_Y

// Layout Nr. 3
#define LAYOUT3_1_X Xpos
#define LAYOUT3_1_Y Ypos
#define LAYOUT3_2_X Xsize/2 + Xpos - 36
#define LAYOUT3_2_Y Ysize/2 + Ypos - big_font_high/2
#define LAYOUT3_3_X LAYOUT3_2_X + 36 + 30
#define LAYOUT3_3_Y Ysize/2 + Ypos - big_font_high/2

// Layout Nr. 4
#define LAYOUT4_1_X Xpos
#define LAYOUT4_1_Y Ypos
#define LAYOUT4_2_X Xpos + 12
#define LAYOUT4_2_Y LAYOUT4_1_Y + small_font_high + 2
#define LAYOUT4_3_X Xpos + 12
#define LAYOUT4_3_Y LAYOUT4_2_Y + medium_font_high + 2

// Layout Nr. 5
#define LAYOUT5_1_X Xpos
#define LAYOUT5_1_Y Ypos
#define LAYOUT5_2_X Xpos + 2
#define LAYOUT5_2_Y LAYOUT5_1_Y + small_font_high
#define LAYOUT5_3_X Xpos + 2
#define LAYOUT5_3_Y LAYOUT5_2_Y + small_font_high
#define LAYOUT5_4_X Xpos + 2
#define LAYOUT5_4_Y LAYOUT5_3_Y + small_font_high

#endif



/*
 * Display MENUs
 */
void bootscreen() {

  u8g2.setFontPosTop();
  u8g2.setFont(small_font);
  u8g2.setCursor(Xpos, Ypos);
  u8g2.setCursor(Xpos, Ypos);
  u8g2.print(bootmsg1);
  u8g2.setCursor(Xpos, Ypos + 8);
  u8g2.print(bootmsg2);
  u8g2.setCursor(Xpos, Ypos + 16);
  u8g2.print(bootmsg3);

}

/*
 * simple clock with date
 */
void menu_clock() {

  u8g2.setFont(big_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(LAYOUT1_1_X, LAYOUT1_1_Y);
  sprintf(buf, "%02d:%02d", hour, minute);
  u8g2.print(buf);

  u8g2.setFont(medium_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(LAYOUT1_2_X + 10, LAYOUT1_2_Y);
  sprintf(buf, "%02d.%02d.%04d", day, month, year);
  u8g2.print(buf);

  switch (button_1) {
    case 1: MainMenuPos++; break;
//    case 2: MainMenuPos--; break;
    case 2: MainMenuPos = 200; break;
//    case 2: MainMenuPos = 101; break;
  }
  button_1 = 0;

}



/*
 * SPEED
 */
void menu_speed() {
  if ( !engine_running) {
    MainMenuPos++;
  }
  u8g2.setFont(big_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(LAYOUT2_1_X, LAYOUT2_1_Y);
  //sprintf (buf, "%3d", speed + SPEED_OFFSET);
  sprintf (buf, "%3d", speed);
  u8g2.print(buf);

  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(LAYOUT2_2_X, LAYOUT2_2_Y);
  u8g2.print(F("km/h"));

  //Temperature
  if ( temp_out_port != 0 ) {
    u8g2.setFont(medium_font);
    u8g2.setFontPosTop();
    u8g2.setCursor(LAYOUT2_3_X, LAYOUT2_3_Y);
    int m = (temp_out - ( (int) temp_out ))*10;
    sprintf (buf, "%02d.%1d", (int)temp_out, m);
    u8g2.print(buf);
    //u8g2.print(temp_out, 1);
    u8g2.print(F("\xb0"));
    u8g2.print(F("C"));
  }

  //Time
  u8g2.setFontPosTop();
  u8g2.setCursor(LAYOUT2_4_X, LAYOUT2_4_Y);
  sprintf(buf, "%02d:%02d", hour, minute);
  u8g2.print(buf);

  switch (button_1) {
    case 1: MainMenuPos++; break;
    case 2: MainMenuPos--; break;
  }
  button_1 = 0;
}


/*
 * GPS informations
 */
void menu_gpsinfo() {

  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(LAYOUT5_1_X, LAYOUT5_1_Y);
  u8g2.print(F("GPS Infos: "));
  u8g2.setCursor(LAYOUT5_2_X, LAYOUT5_2_Y);
  u8g2.print(F("Pos : "));
  u8g2.print(gps_latitude, 3);
  u8g2.print(F(", "));
  u8g2.print(gps_longitude, 3);
  u8g2.setCursor(LAYOUT5_3_X, LAYOUT5_3_Y);
  u8g2.print(F("Alt : "));
  u8g2.print(gps_altitude, DEC);
  u8g2.print(F("m"));
  u8g2.setCursor(LAYOUT5_4_X, LAYOUT5_4_Y);
  u8g2.print(F("Empfang: u: "));
  u8g2.print(gps_used_satellites, DEC);
  u8g2.print(F(" ( v: "));
  u8g2.print(gps_view_satellites, DEC);
  u8g2.print(F(" )"));


  switch (button_1) {
    case 1: MainMenuPos++; break;
    case 2: MainMenuPos = MENU_gpsinfo2; break;
  }
  button_1 = 0;

}

void menu_gpsinfo2() {

  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos);
  u8g2.print(F("GPS Infos (Trip): "));
  u8g2.setCursor(Xpos, Ypos + 8);
  u8g2.print(F("V max: "));
  u8g2.print(gps_speed_max, 5);
  u8g2.setCursor(Xpos, Ypos + 16);
  u8g2.print(F("Dist.: "));
  u8g2.print(gps_distance/1000, DEC);
  u8g2.setCursor(Xpos, Ypos + 24);

  switch (button_1) {
    case 1: MainMenuPos++; break;
  }
  button_1 = 0;

}

/*
 * Trip Infos
 * distance, speed (avg, max), triptime
 */
void menu_trip() {

  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(LAYOUT4_1_X, LAYOUT4_1_Y);
  u8g2.print(F("Trip Infos: "));
  u8g2.setFont(medium_font);
  u8g2.setCursor(LAYOUT4_2_X, LAYOUT4_2_Y);
  //u8g2.print(F("Dist: "));
  uint16_t m = trip_distance - ( gps_distance / 1000 ) * 1000;
  sprintf (buf, "%04d.%1d km", gps_distance / 1000, m / 100);

  u8g2.print(buf);
  u8g2.setFont(small_font);
  u8g2.setCursor(LAYOUT4_3_X, LAYOUT4_3_Y);

  long tmp = engine_running_total + engine_running_sec;
  //u8g2.print(F("travel time: "));
  u8g2.print(F("in "));
  u8g2.print(tmp/3600, DEC);
  u8g2.print(F("h "));
//  if ( engine_running_sec > 3600 ) {
  u8g2.print((tmp - ((tmp/3600) * 3600)) / 60 , DEC);
  u8g2.print(F("min"));

  switch (button_1) {
    case 1: MainMenuPos++; break;
    case 2:
      //gps_distance_trip = 0;
      gps_speed_max_trip = 0;
      engine_running_total = 0;
      break;
  }
  button_1 = 0;
}

/*
 * SD Card
 */
void menu_sdcard() {
  #ifdef SDCARD

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
  #else //SDCard
  MainMenuPos++;
  #endif //SDCARD
}


/*
 * Clima
 */
void menu_clima() {
  int m = 0;

  if ( temp_out_port == 0 ) {
    MainMenuPos++;

  } else {
    // use a different layout if humidity exists
    if ( hum_out > 0 ) {
      u8g2.setFont(small_font);
      u8g2.setFontPosTop();
      u8g2.setCursor(LAYOUT4_1_X, LAYOUT4_1_Y);
      u8g2.print(F("Klima Aussen:"));

      u8g2.setFont(medium_font);
      u8g2.setCursor(LAYOUT4_2_X + 22, LAYOUT4_2_Y);

      m = (temp_out - ( (int) temp_out ))*10;
      sprintf (buf, "%02d.%1d", (int)temp_out, m);
      u8g2.print(buf);

      //u8g2.print(temp_out, 1);
      u8g2.print(F("\xb0"));
      u8g2.print(F("C"));

      u8g2.setFont(small_font);
      u8g2.setCursor(LAYOUT4_3_X + 28, LAYOUT4_3_Y);

      sprintf (buf, "%02d", hum_out);
      u8g2.print(buf);
      u8g2.print(F("%"));
    }
    else {
      u8g2.setFont(small_font);
      u8g2.setFontPosTop();
      u8g2.setCursor(LAYOUT3_1_X, LAYOUT3_1_Y);
      u8g2.print(F("Klima Aussen:"));

      u8g2.setFont(big_font);
      u8g2.setCursor(LAYOUT3_2_X, LAYOUT3_2_Y);

      m = (temp_out - ( (int) temp_out ))*10;
      sprintf (buf, "%02d.%1d", (int)temp_out, m);
      u8g2.print(buf);

      u8g2.setFont(small_font);
      u8g2.setCursor(LAYOUT3_3_X, LAYOUT3_3_Y);
      u8g2.print(F("\xb0"));
      u8g2.print(F("C"));
    }
    switch (button_1) {
      case 1: MainMenuPos++; break;
      //case 2: MainMenuPos = SubMenu3Nr; break;
    }
    button_1 = 0;
  }
}



/*
 * Dynamic Menu for all define values
 */
void menu_values() {
  if ( *values[MenuValuesPos].port == 0 ) {
    if (MenuValuesPos+1 >= (sizeof(values) / sizeof(values[0])) ) {
      MainMenuPos++;
      MenuValuesPos = 0;
    } else {
      MenuValuesPos++;
    }
  }
  else if ( !values[MenuValuesPos].show_off && !engine_running) {
    if (MenuValuesPos+1 >= (sizeof(values) / sizeof(values[0])) ) {
      MainMenuPos++;
      MenuValuesPos = 0;
    } else {
      MenuValuesPos++;
    }
  }
  else {
    u8g2.setFont(small_font);
    u8g2.setFontPosTop();
    u8g2.setCursor(LAYOUT3_1_X, LAYOUT3_1_Y);
    u8g2.print(values[MenuValuesPos].desc);
    u8g2.setFont(big_font);
    u8g2.setFontPosTop();
    u8g2.setCursor(LAYOUT3_2_X, LAYOUT3_2_Y);

    int digits = 1;
    if ( *values[MenuValuesPos].value > 999 ) {
      digits = 4;
    }
    else if ( *values[MenuValuesPos].value > 99 ) {
      digits = 3;
    }
    else if ( *values[MenuValuesPos].value > 9 ) {
      digits = 2;
    }

    if ( values[MenuValuesPos].digits != 0 ) {
      digits += 1 + values[MenuValuesPos].digits;
    }

    digits = 4 - digits;
    for (digits; digits>0; digits--) {
      u8g2.print(F(" "));
    }

    u8g2.print(*values[MenuValuesPos].value, values[MenuValuesPos].digits);

    u8g2.setFont(medium_font);
    u8g2.setCursor(LAYOUT3_3_X, LAYOUT3_3_Y);
    u8g2.print(values[MenuValuesPos].suffix);

    switch (button_1) {
      case 1:
        if (MenuValuesPos+1 >= (sizeof(values) / sizeof(values[0])) ) {
          MainMenuPos++;
          MenuValuesPos = 0;
        } else {
          MenuValuesPos++;
        }
        break;
      case 2: MainMenuPos--; break;
    }
    button_1 = 0;
  }
}

/*
 * Info Menu
 */
void menu_info() {

  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(LAYOUT5_1_X, LAYOUT5_1_Y);
  u8g2.print(F("BusPuter"));
  u8g2.setCursor(LAYOUT5_2_X, LAYOUT5_2_Y);
  u8g2.print(F("Designed by Brun"));
  u8g2.setCursor(LAYOUT5_3_X, LAYOUT5_3_Y);
  u8g2.print(F("Version: "));
  u8g2.print(F(VERSION));
  u8g2.setCursor(LAYOUT5_4_X, LAYOUT5_4_Y);
  u8g2.print(F("Build: "));
  u8g2.print(F(BUILD));

  switch (button_1) {
    case 1: MainMenuPos++; break;
    //case 2: MainMenuPos--; break;
    case 2: MainMenuPos = 200; break;

  }
  button_1 = 0;
}

/*
 * Options Menu
 */
void menu_optionen() {

  if ( engine_running) {
    MainMenuPos++;
  }

  u8g2.setFont(medium_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos + 10, Ypos + 10);
  u8g2.print(F("Optionen"));


  switch (button_1) {
    case 1: MainMenuPos++; break;
    case 2: MainMenuPos = 101; break;
  }
  button_1 = 0;
}

/*
 * Dynamic Menu for config parameters
 */
void menu_opt_config() {

  if ( config[MenuConfigPos].steps == 0 ) {
    if (MenuConfigPos+1 >= (sizeof(config) / sizeof(config[0])) ) {
      MainMenuPos++;
      MenuConfigPos = 0;
    } else {
      MenuConfigPos++;
    }
  }
  else {
    u8g2.setFont(small_font);
    u8g2.setFontPosTop();
    u8g2.setCursor(LAYOUT4_1_X, LAYOUT4_1_Y);
    u8g2.print(config[MenuConfigPos].desc);
    u8g2.setFont(medium_font);
    u8g2.setFontPosTop();
    u8g2.setCursor(LAYOUT4_2_X, LAYOUT4_2_Y);

    u8g2.print(*config[MenuConfigPos].config);

    u8g2.setFont(small_font);
    u8g2.setCursor(LAYOUT4_3_X, LAYOUT4_3_Y);

    if ( MenuEdit ) {
      u8g2.print(F("long -> accept"));
    }
    else {
      u8g2.print(F("long -> edit"));
    }


    switch (button_1) {
      case 1:
        if ( MenuEdit ) {
          *config[MenuConfigPos].config = *config[MenuConfigPos].config + config[MenuConfigPos].steps;
          if ( *config[MenuConfigPos].config > config[MenuConfigPos].max ) {
            *config[MenuConfigPos].config = 0;
          }
        }
        else {
          if (MenuConfigPos+1 >= (sizeof(config) / sizeof(config[0])) ) {
            MainMenuPos++;
            MenuConfigPos = 0;
          } else {
            MenuConfigPos++;
          }
        }
        break;
      case 2:
        if ( MenuEdit ) {
          MenuEdit = false;
        }
        else {
          MenuEdit = true;
        }
        break;
    }
    button_1 = 0;
  }
}


void menu_save_config() {

  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos);
  u8g2.print(F("Save config:"));

  u8g2.setCursor(Xpos + 10, Ypos + 10);
  //u8g2.print(F("press and hold to save"));

  switch (saved_config) {
    case 0: u8g2.print(F("hold to save")); break;
    case 1: u8g2.print(F("OK config saved")); break;
    case 2: u8g2.print(F("ERR: stop Engine")); break;

  }


  switch (button_1) {
    case 1:
      MainMenuPos++;
      saved_config = 0;
      break;
    case 2:
      if (!engine_running) {
        if (saved_config != 1 ) {
          saved_config = 1;
          SPI_lock = false;
          save_config();
          SPI_lock = true;
        }
      }
      else {
        saved_config = 2;
      }

      break;
  }
  button_1 = 0;

  //MainMenuPos++;
}

void menu_debug_ports() {

  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos);
  u8g2.print(F("Port1: "));
  u8g2.print(a0_V, 1);

  u8g2.setCursor(Xpos, Ypos+small_font_high);
  u8g2.print(F("Port2: "));
  u8g2.print(a1_V, 1);

  u8g2.setCursor(Xpos, Ypos+2*small_font_high);
  u8g2.print(F("Port3: "));
  u8g2.print(a2_V, 1);

  u8g2.setCursor(Xpos, Ypos+3*small_font_high);
  u8g2.print(F("Port4: "));
  u8g2.print(a3_V, 1);


  switch (button_1) {
    case 1: MainMenuPos++; break;
    //case 2: speed_source++; break;
  }
  button_1 = 0;
}

void menu_debug_ports_2() {

  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos);
  u8g2.print(F("Port4: "));
  u8g2.print(a4_hz);

  u8g2.setCursor(Xpos, Ypos+small_font_high);
  u8g2.print(F("Port5: "));
  u8g2.print(a5_hz);


  u8g2.setCursor(Xpos, Ypos+2*small_font_high);
  u8g2.print(F("Fuel Ohm: "));
  u8g2.print(fuel_ohm);

  u8g2.setCursor(Xpos, Ypos+3*small_font_high);
  u8g2.print(F("Temp Ohm: "));
  u8g2.print(water_temp_ohm);

  switch (button_1) {
    case 1: MainMenuPos++; break;
    //case 2: speed_source++; break;
  }
  button_1 = 0;
}


//#endif // DOGS102
#endif // ARDUINO_ARCH_SAMD
#endif // U8G2_DISPLAY
