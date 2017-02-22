/***************************************************
 *  This sketch is for I2C Bus functions.
 *
 *  Author: Brun
 *  
 ****************************************************/

#ifdef I2C
#include <Wire.h>

#define I2C_TIMER 1000 // 200ms
unsigned long i2c_timer = 0;


#ifdef SI7021
double si7021_temperature;
double si7021_humidity;
#endif // SI7021

#ifdef SI7021_AS_IN
#define TEMPERATURE_IN
#define HUMIDITY_IN
#endif // SI7021_AS_IN
#ifdef SI7021_AS_OUT
#define TEMPERATURE_OUT
#define HUMIDITY_OUT
#endif // SI7021_AS_OUT


//const int ADDR =0x40;
int X0,X1,Y0,Y1,Y2,Y3;
double X,Y,X_out,Y_out1,Y_out2;


void i2c_init() {
  TRACE_PRINTLN(F("#i2c_init"));
  display_bootmsg(F("Init I2C"));


  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  Wire.begin();
  delay(100);

  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error==4) 
    {
      Serial.print("Unknow error at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
  
  //Wire.begin();
  delay(100);

  #ifdef SI7021
  Wire.beginTransmission(0x40);
  delay(200);
  #endif
  
  Wire.endTransmission();
}

void i2c_loop() {
  if ( i2c_timer < millis() ) {
    // put your code here

    i2c_get_si7021();

    i2c_timer = millis() + I2C_TIMER;
  }
}


#ifdef SI7021
void i2c_get_si7021() {
  TRACE_PRINT(F("#si7021_loop"));
  

  //Serial.println("START #1");
  //Send command of initiating temperature measurement
  Wire.beginTransmission(0x40);
  Wire.write(0xE3);
  Wire.endTransmission();

  //Serial.println("END #1");

  //read temperature
  Wire.requestFrom(0x40,2);
  if(Wire.available()<=2);
  {
    int t0 = Wire.read();
    int t1 = Wire.read();
    t0 = t0<<8;
    si7021_temperature = ((175.72*(t0+t1))/65536)-46.85;
  }

  DEBUG_PRINT(F("#si7021_temperature: "));
  DEBUG_PRINT(si7021_temperature);

  // read humidity
  Wire.beginTransmission(0x40);                     
  Wire.write(0xE5);
  Wire.endTransmission(); 

  //Serial.println("END #2");

  Wire.requestFrom(0x40,2);
  if(Wire.available()<=2);
  {
    int h0 = Wire.read();
    int h2=h0/100; 
    h0=h0%100;
    int h1 = Wire.read();
    double hd1 = h2*25600;
    double hd2 = h0*256+h1;
    hd1 = (125*hd1)/65536;                     
    hd2 = (125*hd2)/65536;
    //Y = hd1+hd2;
    si7021_humidity = hd1+hd2-6;
  }

  DEBUG_PRINT(F(", si7021_humidity: "));
  DEBUG_PRINTLN(si7021_humidity);

  #ifdef SI7021_AS_IN
  temp_in = si7021_temperature*10;
  hum_in = si7021_humidity;
  #endif // SI7021_AS_IN

  #ifdef SI7021_AS_OUT
  temp_out = si7021_temperature*10;
  hum_out = si7021_humidity;
  #endif // SI7021_AS_IN
  
}
#endif // SI7021
#endif // I2C

