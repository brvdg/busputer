#ifdef ONEWIRE




//http://www.hobbytronics.co.uk/ds18b20-arduino
#include <DallasTemperature.h>
// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 10
 
// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
 
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);


void onewire_init() {
  
}

void onewire_loop() {
  if ( onewire_timer < millis() ) {
    
    onewire_timer = millis() + ONEWIRE_TIMER;

    // call sensors.requestTemperatures() to issue a global temperature
    // request to all devices on the bus
    //Serial.print(" Requesting temperatures...");
    sensors.requestTemperatures(); // Send the command to get temperatures
    //Serial.println("DONE");

    //Serial.print("Temperature for Device 1 is: ");
    //Serial.println(sensors.getTempCByIndex(0)); // Why "byIndex"? 
    temp_out = (sensors.getTempCByIndex(0) * 10);
    //Serial.print("#Aussentemperatur: ");
    //Serial.println(temp_out, DEC);
    // You can have more than one IC on the same bus. 
    // 0 refers to the first IC on the wire
  }
}


#endif

