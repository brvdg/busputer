/***************************************************
 *  This sketch is for I2C Bus functions.
 *
 *  Author: Brun
 *  
 ****************************************************/

#ifdef I2C



//const int ADDR =0x40;
int X0,X1,Y0,Y1,Y2,Y3;
double X,Y,X_out,Y_out1,Y_out2;


void i2c_init() {
  //INFO_PRINTLN(F("#si7021_init"));
  display_bootmsg(F("Init I2C"));
  Wire.begin();
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

