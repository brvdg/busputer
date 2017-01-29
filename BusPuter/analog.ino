/***************************************************
 *  This sketch is to read the local analog ports.
 *
 *  Author: Brun
 *  
 ****************************************************/


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

