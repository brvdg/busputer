/***************************************************
  This code is based on the U8glib library from olikraus.
  GIT: 
  Wiki: 

  * Author: Brun von der Gönne

  This module will only used for 8Bit Arduino processors.
  It's not usable right now.
  
 ****************************************************/
 
//#ifndef ARDUINO_ARCH_SAMD
#ifndef ARDUINO_ARCH_SAMD

#ifdef LCD

//Menu defination
#define MENU_clock 1



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

  
  switch (MainMenuPos) {
    case 0: bootscreen(); break;
    case MENU_clock: menu_clock(); break; // 1
    
    default: {
      menu_clock();
      MainMenuPos = 1;
      break;
    }
  }

  
  char buf[9];
  
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(small_font);
  //u8g.setFont(u8g_font_freedoomr25n);
  //u8g.setFont(u8g_font_osb21);
  //u8g.drawStr( 0, y_pos, "18:29");

}

void display_update(void) {
  // picture loop
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
}

void display_loop() {
  //TRACE_PRINTLN(F("#->display_loop"));
  if ( !lcd_update_timer_lock ) {
    if ( !SPI_lock ) {
      SPI_lock = true;
      lcd_update_timer_lock = true;
      display_update();
      lcd_update_timer_lock = false;
      SPI_lock = false;
    } else {
      TRACE_PRINTLN(F("#SPI Bus locked"));
    }
  }
  else {
    Serial.println(F("#display update locked..."));
  }
}

/*
 * simple clock with date
 */
void menu_clock() {
  char buf[24];

  u8g.setFont(big_font);
  u8g.setFontPosTop();
  u8g.setPrintPos(Xpos + 10, Ypos);
  sprintf(buf, "%02d:%02d", hour, minute);
  u8g.print(buf);

  u8g.setFont(medium_font);
  u8g.setFontPosTop();
  u8g.setPrintPos(Xpos + 22, Ypos + 24);
  sprintf(buf, "%02d.%02d.%04d", day, month, year);
  u8g.print(buf);

  switch (button_1) {
    case 1: MainMenuPos++; break;
    case 2: MainMenuPos--; break;
  }
  button_1 = 0;
}

void bootscreen() {
  //u8g.drawXBMP( 48, 0, vw_width, vw_height, vw_bits);
  u8g.setFontPosTop();
  //u8g2.setCursor(24, 0);
  u8g.setPrintPos(Xpos, Ypos);
  u8g.print(bootmsg1);
  u8g.setPrintPos(Xpos, Ypos + 8);
  u8g.print(bootmsg2);
  u8g.setPrintPos(Xpos, Ypos + 16);
  u8g.print(bootmsg3);

}
void display_bootmsg(String msg) {
  bootmsg1 = bootmsg2;
  bootmsg2 = bootmsg3;
  bootmsg3 = msg;

  //bootmsg = "Waiting for Serial";
  display_update();

  INFO_PRINT(F("#"));
  INFO_PRINTLN(msg);
}

#else //LCD
/*void init_display(void) {
  //empty funktion to disable the LCD insted to use "#ifdef LCD" in the code
  }*/

/*void display_update(void) {
  //empty funktion to disable the LCD insted to use "#ifdef LCD" in the code
  }*/

void display_bootmsg(String msg) {
  INFO_PRINT(F("#"));
  INFO_PRINTLN(msg);
}
#endif //
#endif
