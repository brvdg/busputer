#ifdef U8G2_DISPLAY


//Menu defination
#define MENU_clock 1
#define MENU_speed 2
#define MENU_trip 3
#define MENU_values 4
//#define MENU_water_temp 40
//#define MENU_fuel 50
#define MENU_clima 5
//#define MENU_power 7
#define MENU_gpsinfo 6
//#define MENU_rpm 9
#define MENU_optionen 7
#define MENU_info 8

//#define MENU_altitude 

#define MENU_custom 13
#define MENU_gpsinfo2 14
#define MENU_

#define MENU_opt_config 101

//#define MENU_opt_temp_out 1010
//#define MENU_opt_rpm 102
//#define MENU_opt_bord_voltage 103
//#define MENU_opt_ki_dimmer 104
//#define MENU_opt_speed 105
//#define MENU_opt_dimmer_min 106
//#define MENU_opt_dimmer_max 107
#define MENU_save_config 102

#define MENU_debug_ports 200
#define MENU_debug_ports_2 201
#define MENU_sdcard 202




#ifdef DOGS102
U8G2_UC1701_EA_DOGS102_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ U8G2_DISPLAY_CS, /* dc=*/ U8G2_DISPLAY_DC);
//U8G2_PCD8544_84X48_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ U8G2_DISPLAY_CS, /* dc=*/ U8G2_DISPLAY_DC, /*rst=*/ U8G2_DISPLAY_RST );

//U8G2_ST7565_EA_DOGM128_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ U8G2_DISPLAY_CS, /* dc=*/ U8G2_DISPLAY_DC);
#endif

#ifdef NOKIA
U8G2_PCD8544_84X48_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 12, /* dc=*/ 10, /* reset=*/ 11);     // Nokia 5110 Display
#endif

#ifdef OLED
U8G2_SH1106_128X64_NONAME_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ U8G2_DISPLAY_CS, /* dc=*/ U8G2_DISPLAY_DC, /* reset=*/ U8G2_DISPLAY_RST);
#endif


void display_init(void) {
    u8g2.begin();    
}

void display_loop() {
  //TRACE_PRINTLN(F("#->display_loop"));
  //if ( !SPI_lock ) {
    //SPI_lock = true;
    if ( !display_update_timer_lock ) {
      minute = rtc.getMinutes();
      hour = rtc.getHours();
      day = rtc.getDay();
      month = rtc.getMonth();
      year = rtc.getYear() + 2000;

      display_update_timer_lock = true;
      display_update();
      display_update_timer_lock = false;

      //display_set_led();
    }
    else {
      TRACE_PRINTLN(F("#display update locked..."));
    }
    //SPI_lock = false;
  //}  
  //else {
    //DEBUG_PRINTLN(F("#SPI Bus locked for Display update"));
  //}
}

void display_draw(void) {
  //TRACE_PRINTLN(F("#->draw"));
   //u8g2.setFont(small_font);

  u8g2.setFontPosTop();
  //u8g2.setCursor(Xpos, Ypos);

  //DEBUG_PRINTLN(MainMenuPos);

  switch (MainMenuPos) {
    case 0: bootscreen(); break;
    case MENU_clock: menu_clock(); break;
    case MENU_speed: menu_speed(); break;
    case MENU_trip: menu_trip(); break;
    case MENU_values: menu_values(); break;
    case MENU_gpsinfo: menu_gpsinfo(); break;
    case MENU_gpsinfo2: menu_gpsinfo2(); break;
//    case MENU_power: menu_power(); break;
    case MENU_sdcard: menu_sdcard(); break;
    //case MENU_water_temp: menu_water_temp(); break;
    case MENU_clima: menu_clima(); break; 
    case MENU_info: menu_info(); break; 
    //case MENU_fuel: menu_fuel(); break;
    //case MENU_rpm: menu_rpm(); break;
    case MENU_optionen: menu_optionen(); break;

    case MENU_opt_config: menu_opt_config(); break;
    //case MENU_opt_temp_out: menu_opt_temp_out(); break;
    //case MENU_opt_rpm: menu_opt_rpm(); break;
    //case MENU_opt_bord_voltage: menu_opt_bord_voltage(); break;
    //case MENU_opt_ki_dimmer: menu_opt_ki_dimmer(); break;
    //case MENU_opt_speed: menu_opt_speed(); break;
    //case MENU_opt_dimmer_min: menu_opt_dimmer_min(); break;
    //case MENU_opt_dimmer_max: menu_opt_dimmer_max(); break;
    case MENU_save_config: menu_save_config(); break;

    case MENU_debug_ports: menu_debug_ports(); break;
    case MENU_debug_ports_2: menu_debug_ports_2(); break;
    
    

    #ifdef CUSTOM
    case MENU_custom: custom_menu(); break; // display the custom menu
    #endif // CUSTOM

    default: {
      menu_clock();
      MainMenuPos = 1;
      break;
    }
  }
}

void display_update(void) {
  if ( !SPI_lock ) {
    SPI_lock = true;
    //TRACE_PRINTLN(F("#->display_update"));
    u8g2.firstPage();
    do {
      display_draw();
    } while ( u8g2.nextPage() );
    SPI_lock = false;
  }  
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

#else // U8G2_DISPLAY

void display_bootmsg(String msg) {
  TRACE_PRINTLN(F("#->display_bootmsg"));
  INFO_PRINT(F("#"));
  INFO_PRINTLN(msg);
}

#endif // U8G2_DISPLAY
