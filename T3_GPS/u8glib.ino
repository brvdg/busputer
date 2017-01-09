#ifndef ARDUINO_ARCH_SAMD
#ifdef LCD

/*
 * https://github.com/olikraus/u8glib/wiki/fontsize
 * ACHTUNG: Fonts brauchen sehr viel Platz!!!
 */

#include "U8glib.h"


//#define small_font u8g_font_courB08r //1145
#define small_font u8g_font_5x8r //805
//#define small_font u8g_font_5x7 //1500
//#define medium_font u8g_font_courB14r //2167
//#define medium_font u8g_font_courB12r //1857

#define medium_font u8g_font_6x13r //1041

//#define big_font u8g_font_courB24n //707

//#define BG_LED 9


int y_pos = 0;  // global variable
int x_pos = 0;  // global variable

//Software SPI
//U8GLIB_DOGS102 u8g(13, 12, 11, 10);    // U8GLIB_DOGS102(sck, mosi, cs, a0 [, reset])
U8GLIB_DOGM132 u8g(13, 12, 11, 10);   // SPI Com: SCK = 13, MOSI = 12, CS = 11, A0 = 10

//Hardware SPI
//U8GLIB_DOGS102 u8g(10, 8);    // U8GLIB_DOGS102(cs, a0 [, reset])




void init_display() {
  // flip screen, if required
  // u8g.setRot180();
  
  // set SPI backup if required
  //u8g.setHardwareBackup(u8g_backup_avr_spi);

  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
}



void draw(void) {
  char buf[9];
  
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(small_font);
  //u8g.setFont(u8g_font_freedoomr25n);
  //u8g.setFont(u8g_font_osb21);
  //u8g.drawStr( 0, y_pos, "18:29");
  

  u8g.setFontPosTop();
  u8g.setPrintPos(0, 0);
  if (gps_success) {
    //u8g.print(F("lat:   "));
    //u8g.print(gps_latitude, DEC);
    //u8g.setPrintPos(0, 8);
    //u8g.print(F("long:  "));
    //u8g.print(F(":"));
    //u8g.print(gps_longitude, DEC);
    u8g.setFont(medium_font);
    u8g.setFontPosTop();
    u8g.setPrintPos(0, 0);
    //u8g.print(gps_speed, DEC);
    
    //u8g.setFont(small_font);
    //u8g.setPrintPos(0, 8);
    //u8g.print(F("alti:  "));
    //u8g.print(gps_altitude, DEC);
    //u8g.setPrintPos(64, 8);
    u8g.print(F("speed: "));
    sprintf (buf, "%3d km/h", gps_speed);
    u8g.print(buf);
    //u8g.print(gps_speed, DEC);
    //u8g.setPrintPos(0,16);
    //u8g.print(F("Time:"));
    //u8g.print(fona_time);
    //u8g.setPrintPos(0,24);
    //u8g.print(F("distance:"));
    //u8g.print(gps_distance, DEC);

    u8g.setFont(small_font);
    u8g.setFontPosTop();
    u8g.setPrintPos(0, 16);
    u8g.print(F("dist:"));
    //u8g.print(km_all, DEC);
    //u8g.print(F(" ( "));
    //u8g.print(km_1, DEC);
    //u8g.print(F(" )"));
    u8g.setPrintPos(0, 24);
    u8g.print(F("Batt:"));
    u8g.print(vbat, DEC);
    u8g.print(F("%"));

    
    sprintf (buf, "%3d%%", vbat);
    u8g.drawStr(112, 0, buf);
    
  }
  else {
    //u8g.print(F("waiting for GPS..."));
    u8g.print(bootmsg);
  }
  //u8g.print("Hello World");
  //u8g.drawStr( 0, 22, "Hello World!");

}

void display_update(void) {
  // picture loop
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
}


#endif
#endif
