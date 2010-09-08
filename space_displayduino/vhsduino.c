#define rPin1 13
#define gPin1 14
#define bPin1 15
#define rPin2 3
#define gPin2 4
#define bPin2 12
#define rPot 2
#define gPot 1
#define bPot 0
#define toilet 2

int flush_idle_state;
void setup()   {       
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
  int flushing = digitalRead(toilet);
  if (flush_idle_state != flushing) {
    soothing_flushing_motion();
  }
  else {
    write_both_leds(analogRead(rPot) / 4,
              analogRead(bPot) / 4,
              analogRead(gPot) / 4);
  }
}

