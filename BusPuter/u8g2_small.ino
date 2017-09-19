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
    uint16_t m = temp_out - ( temp_out/10 ) * 10;
    sprintf (buf, "%02d.%1d", temp_out/10, m);

    u8g2.print(buf);
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
  u8g2.print(F("Empfang: "));
  u8g2.print(gps_view_satellites, DEC);
  u8g2.print(F(" ( "));
  u8g2.print(gps_used_satellites, DEC);
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
  uint16_t m = gps_distance_trip - ( gps_distance_trip / 1000 ) * 1000;
  sprintf (buf, "%04d.%1d km", gps_distance_trip / 1000, m / 100);

  u8g2.print(buf);
  u8g2.setFont(small_font);
  u8g2.setCursor(LAYOUT4_3_X, LAYOUT4_3_Y);

  //u8g2.print(F("travel time: "));
  u8g2.print(F("in "));
  u8g2.print(engine_running_sec/3600, DEC);
  u8g2.print(F("h "));
//  if ( engine_running_sec > 3600 ) {
  u8g2.print((engine_running_sec - ((engine_running_sec/3600) * 3600)) / 60 , DEC);
  u8g2.print(F("min"));

  switch (button_1) {
    case 1: MainMenuPos++; break;
    case 2:
      gps_distance_trip = 0;
      gps_speed_max_trip = 0;
      engine_running_trip_last = 0 - engine_running_sec;
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
    
  }
  // use a different layout if humidity exists
  if ( hum_out > 0 ) {
    u8g2.setFont(small_font);
    u8g2.setFontPosTop();
    u8g2.setCursor(LAYOUT4_1_X, LAYOUT4_1_Y);
    u8g2.print(F("Klima Aussen:"));

    u8g2.setFont(medium_font);
    u8g2.setCursor(LAYOUT4_2_X + 22, LAYOUT4_2_Y);

    m = temp_out - ( temp_out/10 ) * 10;
    sprintf (buf, "%02d.%1d", temp_out/10, m);
    u8g2.print(buf);
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

    m = temp_out - ( temp_out/10 ) * 10;
    sprintf (buf, "%02d.%1d", temp_out/10, m);
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


/*
 * Water temperatur
 */
void menu_water_temp() {
  if (water_temp_port == 0 ) {
    MainMenuPos++;
  }
  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(LAYOUT3_1_X, LAYOUT3_1_Y);
  u8g2.print(F("Wasser Temperatur:"));

  u8g2.setFont(big_font);
  u8g2.setCursor(LAYOUT3_2_X, LAYOUT3_2_Y);
  sprintf (buf, "%3d", water_temp);
  u8g2.print(buf);

  u8g2.setFont(medium_font);
  u8g2.setCursor(LAYOUT3_3_X, LAYOUT3_3_Y);
  u8g2.print(F("\xb0"));
  u8g2.print(F("C"));

  switch (button_1) {
    case 1: MainMenuPos++; break;
      //case 2: MainMenuPos = SubMenu3Nr; break;
  }
  button_1 = 0;
}

/*
 * Bord Voltage
 */
void menu_power() {

  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(LAYOUT3_1_X, LAYOUT3_1_Y);
  u8g2.print(F("Bordspannung:"));
  u8g2.setFont(big_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(LAYOUT3_2_X, LAYOUT3_2_Y);
  u8g2.print(bord_voltage, 1);
  u8g2.setFont(medium_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(LAYOUT3_3_X, LAYOUT3_3_Y);
  u8g2.print("V");

  switch (button_1) {
    case 1: MainMenuPos++; break;
    case 2: MainMenuPos--; break;
  }
  button_1 = 0;
}

/*
 * Fuel Informations
 */
void menu_fuel() {
  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(LAYOUT3_1_X, LAYOUT3_1_Y);
  u8g2.print(F("Tank:"));
  u8g2.setFont(big_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(LAYOUT3_2_X, LAYOUT3_2_Y);
  u8g2.print(fuel_l, 1);
  u8g2.setFont(medium_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(LAYOUT3_3_X, LAYOUT3_3_Y);
  u8g2.print("l");

  switch (button_1) {
    case 1: MainMenuPos++; break;
    case 2: MainMenuPos--; break;
  }
  button_1 = 0; 
}

/*
 * RPM
 */
void menu_rpm() {
  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(LAYOUT3_1_X, LAYOUT3_1_Y);
  u8g2.print(F("RPM:"));
  u8g2.setFont(big_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(LAYOUT3_2_X, LAYOUT3_2_Y);
  
  u8g2.print(rpm);
  
  switch (button_1) {
    case 1: MainMenuPos++; break;
    case 2: MainMenuPos--; break;
  }
  button_1 = 0; 
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

void menu_opt_temp_out() {

  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos);
  u8g2.print(F("Temperatur Out:"));

  u8g2.setCursor(Xpos + 10, Ypos + 10);
  
  switch (temp_out_port) {
    case 0: u8g2.print(F("kein Sensor")); break;
    case 1: u8g2.print(F("OneWire Sensor")); break;
    case 2: u8g2.print(F("SI7021 Sensor")); break;
    default: temp_out_port = 0;
  }
  

  switch (button_1) {
    case 1: MainMenuPos++; break;
    case 2: temp_out_port++; break;
  }
  button_1 = 0;
}

void menu_opt_rpm() {

  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos);
  u8g2.print(F("RPM Input:"));

  u8g2.setCursor(Xpos + 10, Ypos + 10);
  
  switch (rpm_port) {
    case 0: u8g2.print(F("aus")); break;
    case 1: u8g2.print(F("Eingang 1")); break;
    case 2: u8g2.print(F("Eingang 2")); break;
    case 3: u8g2.print(F("Eingang 3")); break;
    case 4: u8g2.print(F("Eingang 4")); break;
    case 5: u8g2.print(F("Eingang 5")); break;
    case 6: u8g2.print(F("Eingang 6")); break;
    default: rpm_port = 0;
  }
  

  switch (button_1) {
    case 1: MainMenuPos++; break;
    case 2: rpm_port++; break;
  }
  button_1 = 0;
}

void menu_opt_bord_voltage() {

  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos);
  u8g2.print(F("Bordspannung Input:"));

  u8g2.setCursor(Xpos + 10, Ypos + 10);
  
  switch (bord_voltage_port) {
    case 0: u8g2.print(F("aus")); break;
    case 1: u8g2.print(F("Eingang 1")); break;
    case 2: u8g2.print(F("Eingang 2")); break;
    case 3: u8g2.print(F("Eingang 3")); break;
    case 4: u8g2.print(F("Eingang 4")); break;
    case 5: u8g2.print(F("Eingang 5")); break;
    case 6: u8g2.print(F("Eingang 6")); break;
    default: bord_voltage_port = 0;
  }
  

  switch (button_1) {
    case 1: MainMenuPos++; break;
    case 2: bord_voltage_port++; break;
  }
  button_1 = 0;
}

void menu_opt_ki_dimmer() {

  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos);
  u8g2.print(F("Bel. Kombi Instr.:"));

  u8g2.setCursor(Xpos + 10, Ypos + 10);
  
  switch (dimmer_port) {
    case 0: u8g2.print(F("aus")); break;
    case 1: u8g2.print(F("Eingang 1")); break;
    case 2: u8g2.print(F("Eingang 2")); break;
    case 3: u8g2.print(F("Eingang 3")); break;
    case 4: u8g2.print(F("Eingang 4")); break;
    case 5: u8g2.print(F("Eingang 5")); break;
    case 6: u8g2.print(F("Eingang 6")); break;
    default: dimmer_port = 0;
  }
  

  switch (button_1) {
    case 1: MainMenuPos++; break;
    case 2: dimmer_port++; break;
  }
  button_1 = 0;
}

void menu_opt_speed() {

  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos);
  u8g2.print(F("Speed Source:"));

  u8g2.setCursor(Xpos + 10, Ypos + 10);
  
  switch (speed_source) {
    case 0: u8g2.print(F("GPS+Speedpulse")); break;
    case 1: u8g2.print(F("Speedpulse")); break;
    case 2: u8g2.print(F("GPS")); break;
    default: speed_source = 0;
  }
  

  switch (button_1) {
    case 1: MainMenuPos++; break;
    case 2: speed_source++; break;
  }
  button_1 = 0;
}

void menu_opt_dimmer_min() {

  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos);
  u8g2.print(F("Dimmer Minimum:"));

  u8g2.setCursor(Xpos + 10, Ypos + 10);

  u8g2.print(dimmer_min, DEC);
  
  switch (button_1) {
    case 1: MainMenuPos++; break;
    case 2: dimmer_min++; break;
  }
  button_1 = 0;
}

void menu_opt_dimmer_max() {

  u8g2.setFont(small_font);
  u8g2.setFontPosTop();
  u8g2.setCursor(Xpos, Ypos);
  u8g2.print(F("Dimmer Maximum:"));

  u8g2.setCursor(Xpos + 10, Ypos + 10);

  u8g2.print(dimmer_min, DEC);
  
  switch (button_1) {
    case 1: MainMenuPos++; break;
    case 2: dimmer_max++; break;
  }
  button_1 = 0;
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