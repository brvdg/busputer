/***************************************************
 *  This sketch is for OneWire functions.
 *  http://www.hobbytronics.co.uk/ds18b20-arduino
 *
 *  Author: Brun
 *  
 ****************************************************/

#ifdef ONEWIRE

#include <DallasTemperature.h>
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

#define ONEWIRE_TIMER 500 // 5s
unsigned long onewire_timer = 0;



void onewire_init() {
  display_bootmsg(F("Init OneWire"));
  
}

void onewire_loop() {
  if ( onewire_timer < millis() ) {
    
    onewire_timer = millis() + ONEWIRE_TIMER;

    sensors.requestTemperatures(); // Send the command to get temperatures

    #ifdef DS18B20_AS_OUT
    temp_out = (sensors.getTempCByIndex(0) * 10);
    #endif

    #ifdef DS18B20_AS_IN
    temp_in = (sensors.getTempCByIndex(0) * 10);
    #endif
  }
}


#endif

