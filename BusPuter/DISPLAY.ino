#ifdef DISPLAY


#ifdef DOGS102
U8G2_UC1701_EA_DOGS102_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ DISPLAY_CS, /* dc=*/ DISPLAY_DC);
#endif

#ifdef OLED
U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ DISPLAY_CS, /* dc=*/ DISPLAY_DC, /* reset=*/ DISPLAY_RST);
#endif


void display_loop() {
  //TRACE_PRINTLN(F("#->display_loop"));
  if ( !SPI_lock ) {
    SPI_lock = true;
    if ( !display_update_timer_lock ) {
      display_update_timer_lock = true;
      display_update();
      display_update_timer_lock = false;
    }
    else {
      TRACE_PRINTLN(F("#display update locked..."));
    }
    SPI_lock = false;
  }  
  else {
    DEBUG_PRINTLN(F("#SPI Bus locked"));
  }
}

void display_update(void) {
  //TRACE_PRINTLN(F("#->display_update"));
  u8g2.firstPage();
  do {
    display_draw();
  } while ( u8g2.nextPage() );
}


void display_bootmsg(String msg) {
  TRACE_PRINTLN(F("#->display_bootmsg"));
  bootmsg1 = bootmsg2;
  bootmsg2 = bootmsg3;
  bootmsg3 = msg;

  //bootmsg = "Waiting for Serial";
  display_update();

  INFO_PRINT(F("#"));
  INFO_PRINTLN(msg);

  //delay(2000);
}

#else // DISPLAY

void display_bootmsg(String msg) {
  TRACE_PRINTLN(F("#->display_bootmsg"));
  INFO_PRINT(F("#"));
  INFO_PRINTLN(msg);
}

#endif // DISPLAY
