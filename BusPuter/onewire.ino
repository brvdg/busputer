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


unsigned long onewire_timer = 0;



void onewire_init() {
  display_bootmsg(F("Init OneWire"));
  sensors.begin();

  sensors.requestTemperatures(); // Send the command to get temperatures

    ds18b20_temp = sensors.getTempCByIndex(0)*10;

    if (sensors.getTempCByIndex(0) != -127 ) {
      DEBUG_PRINTLN(F("#OneWire sensors found"));
      onewire_available = true;
      //delay(2000);
      if ( temp_out_port == 0 ) {
        temp_out_port = 1;
      }
    }
    else {
      if ( temp_out_port == 1 ) {
        temp_out_port = 2;
      }
    }

  
}

void onewire_loop() {
  if ( ( onewire_timer < millis() ) && onewire_available ) {
    
    onewire_timer = millis() + ONEWIRE_TIMER;

    sensors.requestTemperatures(); // Send the command to get temperatures

    ds18b20_temp = sensors.getTempCByIndex(0)*10;

    //if (ds18b20_temp = -127 ) {
    //  DEBUG_PRINTLN(F("#NO sensor found"));
    //}

  }
}


#endif

