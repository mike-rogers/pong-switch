#define SLEEPABLE 1

#include "avr/sleep.h"

int switch_on = 2;
int switch_off = 3;
int sleep_on = 7;
int debounce = 100;
int canSleep = 0;
volatile long lastBounce = 0;

void setup() {
  pinMode(switch_on, INPUT_PULLUP);
  pinMode(switch_off, INPUT_PULLUP);
  pinMode(sleep_on, OUTPUT);
  attachInterrupt(0, switch_triggered_cb, FALLING);
  attachInterrupt(1, switch_triggered_cb, FALLING);
  Serial1.begin(9600);
#ifdef SLEEPABLE
  digitalWrite(sleep_on, LOW);
  delay(5000); // let the xbee module attach
  digitalWrite(sleep_on, HIGH);
#else
  digitalWrite(sleep_on, LOW);
#endif

  canSleep = 1;
}

void loop() {
  if (lastBounce != 0) {
    if (millis() - lastBounce > debounce) {
      if (digitalRead(switch_on) == LOW) {
        Serial1.print("Switch on\n");
      }

      if (digitalRead(switch_off) == LOW) {
        Serial1.print("Switch off\n");
      }

      lastBounce = 0;
#ifdef SLEEPABLE
      Serial1.flush();
      digitalWrite(sleep_on, HIGH);
      delay(500); // give the xbee time to transmit and sleep
#endif
      canSleep = 1;
    }
  }

  if (canSleep == 1) {
    sleepNow();
  }
}

void sleepNow() {
#ifdef SLEEPABLE
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); 
  sleep_enable();
  sleep_mode();
  sleep_disable();
#endif
}

void switch_triggered_cb() {
#ifdef SLEEPABLE
  digitalWrite(sleep_on, LOW);
#endif
  canSleep = 0;
  lastBounce = millis();
}
