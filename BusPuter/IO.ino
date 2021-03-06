/***************************************************
 *  This sketch is to read the local analog ports.
 *
 *  Author: Brun
 *  
 ****************************************************/

int vw_temp [][2] PROGMEM = {  {1000, 20},
                        {800, 26},
                        {700, 30},
                        {500, 40},
                        {425, 44},
                        {350, 50},
                        {300, 55},
                        {250, 60},
                        {212, 65},
                        {180, 69},
                        {150, 75},
                        {125, 80},
                        {107, 84},
                        {89, 90},
                        {75, 95},
                        {60, 101},
                        {53, 105},
                        {45, 110},
                        {38, 114},
                        {32, 119},
                        {26, 125}};

unsigned long IO_timer = 0;

void IO_init() {

  #ifdef ALARM_OUT
  
  pinMode(ALARM_PORT, OUTPUT); 
  
  digitalWrite(ALARM_PORT, HIGH);
  delay(100);
  digitalWrite(ALARM_PORT, LOW);
  delay(100);
  
  #endif //ALARM_OUT


  //pinMode(A0,INPUT_PULLUP)
  /*digitalWrite(A0, HIGH);
  digitalWrite(A1, HIGH);
  digitalWrite(A2, HIGH);
  digitalWrite(A3, HIGH);*/

  
  pinMode(A4, INPUT);           // set pin to input
  pinMode(A5, INPUT);           // set pin to input
  digitalWrite(A4, HIGH);       // turn on pullup resistors
  digitalWrite(A5, HIGH);       // turn on pullup resistors
  attachInterrupt(A4, interrupt_A4, FALLING); // attach interrupt
  attachInterrupt(A5, interrupt_A5, FALLING);

  #ifdef U8G2_DISPLAY_BG_LED
  analogWrite(U8G2_DISPLAY_BG_LED, DIMMER_MIN);
  #endif

  read_ports();
  
}

void IO_loop() {

  if ( IO_timer < millis() ) {
    TRACE_PRINTLN(F("#analog_loop()"));
    IO_timer = millis() + IO_TIMER;

    // read the voltage of each port
    read_ports();

    // get values from OneWire
    #ifdef ONEWIRE
    onewire_loop();
    #endif // ONEWIRE

    // get values from I2C
    #ifdef I2C
    i2c_loop();
    #endif // SI7021

    vw_water_temp();
    
    get_bord_voltage();

    #ifdef U8G2_DISPLAY_BG_LED
    dimmer();
    #endif
    
    get_rpm();

    get_clima_out();

    get_speed();

    get_fuel();
 

  }

}

void vw_water_temp() {
  TRACE_PRINTLN(F("#vw_water_temp()"));
  float val=0;
  float temp = 0;

  water_temp = 0;

  switch (water_temp_port) {
    case 1: val = a0_V; break;
    case 2: val = a1_V; break;
    case 3: val = a2_V; break;
    case 4: val = a3_V; break;
  }

  float ohm = SERIESRESISTOR * (val / (10 - val));
  water_temp_ohm = ohm;
  
  for (int i=0; i<sizeof(vw_temp)/sizeof(vw_temp[0]);i++) {
    if (ohm > vw_temp[i][0] ) {
      water_temp = pgm_read_word_near(&vw_temp[i][1]);
      i=sizeof(vw_temp)/sizeof(vw_temp[0]);  
    }
  }
}

void get_bord_voltage() {
  TRACE_PRINTLN(F("#bord_voltage()"));
  switch (bord_voltage_port) {
    case 1: bord_voltage = a0_V; break;
    case 2: bord_voltage = a1_V; break;
    case 3: bord_voltage = a2_V; break;
    case 4: bord_voltage = a3_V; break;
  }
  
  TRACE_PRINT(F("#BORD VOLTAGE: "));
  TRACE_PRINTLN(bord_voltage);
  
  /*if (bord_voltage > 4) {
    if (!running) {
      running = true;
      start_engine();
    }
  }
  else {
    if (running) {
      running = false;
      stop_engine();
    }
  }*/
}

#ifdef U8G2_DISPLAY_BG_LED
void dimmer() {
  switch (dimmer_port) {
    case 1: dimmer_V = a0_V; break;
    case 2: dimmer_V = a1_V; break;
    case 3: dimmer_V = a2_V; break;
    case 4: dimmer_V = a3_V; break;
  }

  int dimmer_val = 0;
  
  int dimmer_pct = ((100000 / (DIMMER_MAX_mV - DIMMER_MIN_mV)) * (dimmer_V * 1000 - DIMMER_MIN_mV)) / 1000;
  
  if ( dimmer_pct < 5 ) {
    if (running) {
      dimmer_pct = 5;
    } else {
      dimmer_pct = 0;
    }
  }

  dimmer_val = ( dimmer_pct * DIMMER_MAX ) / 100;
  analogWrite(U8G2_DISPLAY_BG_LED, dimmer_val);

  #ifdef OFFLINE
  analogWrite(U8G2_DISPLAY_BG_LED, 180);
  #endif
  
}
#endif


/*
 * RPM
 */
void get_rpm() {
  int val = 0;
  switch (rpm_port) {
    case 5: val = a4_hz; a4_hz = 0; break;
    case 6: val = a5_hz; a5_hz = 0; break;
  }

  rpm = (float(val) / rpm_multipl) * 60;

  TRACE_PRINT(F("#RPM: "));
  TRACE_PRINTLN(rpm);
  
}

void get_clima_out() {
  switch (temp_out_port) {
    case 1: 
      temp_out = lm75_1_temp; 
      hum_out = -1000;
      break;
    case 2: 
      temp_out = ds18b20_temp; 
      hum_out = -1000;
      break;
    case 3:
      temp_out = si7021_temp;
      hum_out = si7021_hum;
      break;
  }
}


/*
 * Speed
 */
void get_speed() {
  int val = 0;
  uint32_t speed_sum = 0;
  switch (speedpulse_port) {
    case 5: val = a4_hz; a4_hz = 0; break;
    case 6: val = a5_hz; a5_hz = 0; break;
  }

  speedpulse_tmp[SPEEDPULSEARRAY-1] = ( val * 3600 ) / 4000;
  
  for (int i = 0; i < SPEEDPULSEARRAY-1; i++) {
    speedpulse_tmp[i] = speedpulse_tmp[i+1];
    speed_sum += speedpulse_tmp[i];
  }
  speed_sum += speedpulse_tmp[SPEEDPULSEARRAY-1];
  speedpulse = speed_sum / SPEEDPULSEARRAY;

  TRACE_PRINT(F("#SPEEDPULSE: "));
  TRACE_PRINTLN(speedpulse);

  switch (speed_source) {
    case 0:
      speed = 0;
      #ifdef GPS
      if (gps_fixstatus) {
        if (gps_speed >= 5) {
          speed = int(gps_speed);
        }
        else {
          speed = 0;
        }
      }
      else {
        speed = 0;
      }
      #endif

      if (speed == 0) {
        speed = speedpulse;
      }
      break;
    case 1:
      speed = speedpulse;
      break;
    case 2:
      speed = int(gps_speed);
      break;
  }
  
}

void get_fuel() {
  TRACE_PRINTLN(F("#get_fuel()"));
  switch (fuel_port) {
    case 1: fuel_V = a0_V; break;
    case 2: fuel_V = a1_V; break;
    case 3: fuel_V = a2_V; break;
    case 4: fuel_V = a3_V; break;
  }

  TRACE_PRINT(F("#FUEL VOLTAGE: "));
  TRACE_PRINTLN(fuel_V);

  fuel_ohm = FUEL_GAUGERESISTOR * (fuel_V  / (10 - fuel_V ));
  
  float fuel_pct =(fuel_ohm - FUEL_FULL) * 100 / (FUEL_EMPTY - FUEL_FULL);
  fuel_pct = 100 - fuel_pct;
  fuel_l = fuel_pct * FUEL_L / 100;

  
}


/*
 * Read the voltage of each Port
 * The Voltage is used in other functions to calculate diffrent values
 */
void read_ports() {
  TRACE_PRINTLN(F("#read_ports("));

  float a0_sum = 0; 
  float a1_sum = 0; 
  float a2_sum = 0; 
  float a3_sum = 0; 

  
  //a0_V = analogRead(A0) * A0_MULTIPLICATOR;
  //a1_V = analogRead(A1) * A1_MULTIPLICATOR;
  //a2_V = analogRead(A2) * A2_MULTIPLICATOR;
  //a3_V = analogRead(A3) * A3_MULTIPLICATOR;


  // for port A0
  a0_tmp[IO_ARRAY-1] = analogRead(A0) * A0_MULTIPLICATOR;
  for (int i = 0; i < IO_ARRAY-1; i++) {
    
    a0_tmp[i] = a0_tmp[i+1];
    a0_sum += a0_tmp[i];
  }
  a0_sum += a0_tmp[IO_ARRAY-1];
  a0_V = a0_sum / IO_ARRAY;

  // for port A1
  a1_tmp[IO_ARRAY-1] = analogRead(A1) * A1_MULTIPLICATOR;
  for (int i = 0; i < IO_ARRAY-1; i++) {
    
    a1_tmp[i] = a1_tmp[i+1];
    a1_sum += a1_tmp[i];
  }
  a1_sum += a1_tmp[IO_ARRAY-1];
  a1_V = a1_sum / IO_ARRAY;
    
  // for port A2
  a2_tmp[IO_ARRAY-1] = analogRead(A2) * A2_MULTIPLICATOR;
  for (int i = 0; i < IO_ARRAY-1; i++) {
    
    a2_tmp[i] = a2_tmp[i+1];
    a2_sum += a2_tmp[i];
  }
  a2_sum += a2_tmp[IO_ARRAY-1];
  a2_V = a2_sum / IO_ARRAY;

  // for port A3
  a3_tmp[IO_ARRAY-1] = analogRead(A3) * A3_MULTIPLICATOR;
  for (int i = 0; i < IO_ARRAY-1; i++) {
    
    a3_tmp[i] = a3_tmp[i+1];
    a3_sum += a3_tmp[i];
  }
  a3_sum += a3_tmp[IO_ARRAY-1];
  a3_V = a3_sum / IO_ARRAY;



  TRACE_PRINT(F("#Port Voltage: ")); 
  TRACE_PRINT(a0_V);
  TRACE_PRINT(F(" V, "));
  TRACE_PRINT(a1_V);
  TRACE_PRINT(F(" V, "));
  TRACE_PRINT(a2_V);
  TRACE_PRINT(F(" V, "));
  TRACE_PRINT(a3_V);
  TRACE_PRINTLN(F(" V"));
}

void interrupt_A4()
{
  
  unsigned long diff = micros() - a4_time;
  a4_time = micros();
  a4_hz = 1000000 / diff;

}

void interrupt_A5()
{
  
  unsigned long diff = micros() - a5_time;
  a5_time = micros();
  a5_hz = 1000000 / diff;
  
}


