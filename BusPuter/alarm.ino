#ifdef ALARM_OUT

unsigned long alarm_on_timer = 0;
unsigned long alarm_off_timer = 0;

unsigned int alarm_on = 0;
unsigned int alarm_pause = 0;
unsigned int alarm_counter = 0;
boolean blink_bg_led = false;


void alarm_loop() {

  if ( alarm_counter > 0 ) {
    if ( alarm_on_timer > millis() ) {
      digitalWrite(ALARM_PORT, HIGH);
      if ( blink_bg_led ) {
        analogWrite(U8G2_DISPLAY_BG_LED, 255);
      }
    }
    else if ( alarm_off_timer > millis() ) {
      digitalWrite(ALARM_PORT, LOW);
      if ( blink_bg_led ) {
        analogWrite(U8G2_DISPLAY_BG_LED, 0);
      }
    }
    else {
      alarm_counter--;
      alarm_on_timer = millis() + alarm_on;
      alarm_off_timer = alarm_on_timer + alarm_pause;
    }
  }
}

void make_alarm() {

  
  if (alarm = 1 ) {
    //digitalWrite(ALARM_PORT, HIGH);
    //delay(25);
    //digitalWrite(ALARM_PORT, LOW);
    //delay(5000);

    if ( alarm_on_timer > millis() ) {
      digitalWrite(ALARM_PORT, HIGH);
    }
    else if ( alarm_off_timer > millis() ) {
      digitalWrite(ALARM_PORT, LOW);
    }
    else {
      
      alarm_on_timer = millis() + alarm_on;
      alarm_off_timer = alarm_on_timer + alarm_pause;
    }
  }
}

void set_alarm(int on, int pause, int count, boolean blink) {
  alarm_counter = count;
  alarm_on = on;
  alarm_pause = pause;

  blink_bg_led = blink;

  alarm_counter++;
  
}


#endif // ALARM_OUT
