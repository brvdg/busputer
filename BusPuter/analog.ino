/***************************************************
 *  This sketch is to read the local analog ports.
 *
 *  Author: Brun
 *  
 ****************************************************/


// VW Temperature sensor


// which analog pin to connect
//#define THERMISTORPIN A1         
// resistance at 25 degrees C
#define THERMISTORNOMINAL 300      
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 55  
// how many samples to take and average, more takes longer
// but is more 'smooth'
//#define NUMSAMPLES 1
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 4000
// the value of the 'other' resistor
#define SERIESRESISTOR 50    


#define VW_WATER_TEMP_MULTIPLICATOR 1.56 // the multiplicator for Analog Port. 

#define VW_WATER_TEMP_TIMER 1000 // 200ms
unsigned long water_temp_timer = 0;


void analog_init() {

  digitalWrite(X_Kontakt, HIGH);

  
  #ifdef VW_WATER_TEMP
  digitalWrite(VW_WATER_TEMP, HIGH);
  #endif // VW_WATER_TEMP
  
}

#ifdef VW_WATER_TEMP
void vw_water_temp() {
  float val=0;
  float temp = 0;


  // to save cpu power run this function only every (above defined) time
  if ( water_temp_timer < millis() ) {
    
    val = analogRead(VW_WATER_TEMP);
    //float mV = val * 0.01525;
    temp = val * VW_WATER_TEMP_MULTIPLICATOR;
    temp = (1023 / temp) - 1;
    water_temp_ohm = temp;
    temp = SERIESRESISTOR / temp;
    
    val = val * 0.0152;
    val *= 1000;
    water_temp_V = val;
  
    float steinhart;
    steinhart = temp / THERMISTORNOMINAL;     // (R/Ro)
    steinhart = log(steinhart);                  // ln(R/Ro)
    steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
    steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
    steinhart = 1.0 / steinhart;                 // Invert
    steinhart -= 273.15;                         // convert to C

    water_temp = steinhart;
    DEBUG_PRINT(F("#VW temperature sensor: ")); 
    DEBUG_PRINT(water_temp);
    DEBUG_PRINT(F(" *C; "));
    DEBUG_PRINT(water_temp_V);
    DEBUG_PRINT(F(" mV; "));
    DEBUG_PRINT(water_temp_ohm);
    DEBUG_PRINTLN(F(" Ohm"));

    water_temp_timer = millis() + VW_WATER_TEMP_TIMER;

  }
}
#endif // VW_WATER_TEMP

void x_kontakt() {
  int val = analogRead(X_Kontakt);
  bord_voltage = val * 0.0152;
  //Serial.print("BOARD VOLTAGE : ");
  //Serial.println(bord_voltage);
  if (bord_voltage > 3) {
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
    #ifdef FONA
    //alarmtatus_send = false;
    #endif // FONA
  }
}

