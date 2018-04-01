#include <Thread.h>
#include <StaticThreadController.h>
#include <FastLED.h>

#define ACCELX      A0
#define LED_PIN     5
#define NUM_LEDS    30

#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

const int      serialBaudRate           = 9600;
const int      cylonUpdateDelay         = 10;
const int      accelerometerUpdateDelay = 50;
const int      movementThreshold        = 100;

// State
int            cylonLedNum              = 0;
bool           cylonUpState             = true;
bool           brakingState             = false;
static uint8_t hue                      = 0;
int            prevAccelx;
int            lastMod                  = 0;

Thread* cylonUp = new Thread();
Thread* cylonDown = new Thread();
Thread* accelerometer = new Thread();
StaticThreadController<3> control (cylonUp, cylonDown, accelerometer);

void cylonUpCallback() {
  if (cylonUpState && !brakingState) {
    leds[cylonLedNum] = CHSV(hue++, 255, 255);
    FastLED.show();
    
    cylonLedNum++;
    if (cylonLedNum == NUM_LEDS) {
      cylonUpState = false;
      cylonLedNum--;
    }
  }
}

void cylonDownCallback() {
  if (!cylonUpState && !brakingState) {
    leds[cylonLedNum] = CHSV(hue++, 255, 255);
    FastLED.show();
    
    cylonLedNum--;
    if (cylonLedNum < 0) {
      cylonUpState = true;
      cylonLedNum++;
    }
  }
}

/*
 * Run every accelerometerUpdateDelay seconds
 * 
 * More and higher values = more acceleration or continued acceleration
 * Consistently decreasing values = less acceleration or braking
 */
void accelerometerCallback() {
  int acceleration = analogRead(ACCELX);
  int mod = 0;
  if (acceleration < prevAccelx) {
    mod = prevAccelx % acceleration;
  }

  if ((mod < lastMod) && (mod > movementThreshold)) {
    brakingState = true;
    fill_solid(leds, NUM_LEDS, CRGB::Red);
    FastLED.show();
  } else {
    brakingState = false;
  }
  
  prevAccelx = acceleration;
  lastMod = mod;
  
  Serial.print(mod);
  Serial.print(' braking? ');
  Serial.print(brakingState);
  Serial.println();
}

void setup() {
  Serial.begin(serialBaudRate);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  fill_solid( leds, NUM_LEDS, CRGB::Blue);
  FastLED.show();

  cylonUp->onRun(cylonUpCallback);
  cylonUp->setInterval(cylonUpdateDelay);
  cylonDown->onRun(cylonDownCallback);
  cylonDown->setInterval(cylonUpdateDelay);

  accelerometer->onRun(accelerometerCallback);
  accelerometer->setInterval(accelerometerUpdateDelay);
  
  prevAccelx = analogRead(ACCELX);
}

void loop() {
  control.run();
}
