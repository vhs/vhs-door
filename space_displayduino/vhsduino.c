#define rPin1 13
#define gPin1 14
#define bPin1 15
#define rPin2 3
#define gPin2 4
#define bPin2 12
#define speedPot 3
#define rPot 2
#define gPot 1
#define bPot 0
#define toilet 2

// State for the colour change system
int desired_r = 0;
int desired_g = 0;
int desired_b = 0;
int previous_r = 0;
int previous_g = 0;
int previous_b = 0;
int actual_r = 0;
int actual_g = 0;
int actual_b = 0;
unsigned long last_change = 0;

int flush_idle_state;
void setup()   {       
  last_change = millis();
  pinMode(rPin1, OUTPUT); analogWrite(rPin1, 0);
  pinMode(gPin1, OUTPUT); analogWrite(gPin1, 0);
  pinMode(bPin1, OUTPUT); analogWrite(bPin1, 0);
  pinMode(rPin2, OUTPUT); analogWrite(rPin2, 0);
  pinMode(gPin2, OUTPUT); analogWrite(gPin2, 0);
  pinMode(bPin2, OUTPUT); analogWrite(bPin2, 0);
  pinMode(toilet, INPUT);
  flush_idle_state = digitalRead(toilet);
}

void write_west_leds (int r, int g, int b) {
    analogWrite(rPin2, r); analogWrite(gPin2, g); analogWrite(bPin2, b);
}
void write_east_leds (int r, int g, int b) {
    analogWrite(rPin1, r); analogWrite(gPin1, g); analogWrite(bPin1, b);
}

void write_both_leds (int r, int g, int b) {
    // For now, just set it but in the future a smoothing function would be
    // best.
    write_west_leds(r, g, b);
    write_east_leds(r, g, b);
    actual_r = r;
    actual_g = g;
    actual_b = b;
}

#define white_max 100
void soothing_flushing_motion (void) {
    int b = 255;
    write_both_leds(0, 0, b);
    int east = random(white_max);
    int west = white_max - east;

    write_west_leds(west, west, b);
    write_east_leds(east, east, b);
}

void loop()                     {
  int is_flushing = digitalRead(toilet);
  int speed    = analogRead(speedPot); // 0 to 1023
  if (flush_idle_state != is_flushing) {
    soothing_flushing_motion();
  }
  else if (speed > 10) {
      unsigned long time_interval = speed * 10;
      unsigned long now = millis();
      unsigned long next_change = last_change + time_interval;
      
      if (now > next_change) {
          // time to change the desired
          desired_r = random(255);
          desired_g = random(255);
          desired_b = random(255);
          previous_r = actual_r;
          previous_g = actual_g;
          previous_b = actual_b;
          last_change = now;
      }

      float percent = ((float) (now - last_change)) / time_interval;
      int r_change = (desired_r - previous_r) * percent;
      int g_change = (desired_g - previous_g) * percent;
      int b_change = (desired_b - previous_b) * percent;
      write_both_leds(previous_r + r_change, previous_g + g_change, previous_b + b_change);
  }
  else {
    write_both_leds(analogRead(rPot) / 4,
              analogRead(bPot) / 4,
              analogRead(gPot) / 4);
  }
}
