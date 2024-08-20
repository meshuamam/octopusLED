#include <FastLED.h>
#define VIRTUAL_NUM_LEDS 100
#define LEG_NUM_LEDS 75
#define EYE_LED 23
#define HEAD_NUM_LEDS 25
#define BRIGHTNESS 100
#define HEAD_LEG_CROSSOVER 50
// CALCULATED CONSTANTS
// TOTAL_REAL_LEDS = HEAD_NUM_LEDS + LEG_NUM_LEDS
#define TOTAL_REAL_LEDS 55
CRGB arm1[LEG_NUM_LEDS];
CRGB arm2[LEG_NUM_LEDS];
CRGB head[HEAD_NUM_LEDS];
void setup() {
  FastLED.addLeds<NEOPIXEL, 7>(arm1, LEG_NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 8>(arm2, LEG_NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 9>(head, HEAD_NUM_LEDS);
}

void loop() {
  showRainbow(1000);
  breathe(CHSV(255,0,0), 1000);
}
// START PATTERNS
void showRainbow(int numFrame) {
  for (int frame = 0; frame < numFrame; frame++) {
    for (int led = 0; led < VIRTUAL_NUM_LEDS; led++) {
      setLed(led, CHSV((6 * frame + 4 * led) % 255, 255, BRIGHTNESS));
    }
    FastLED.show();
    delay(40);
  }
}

void breathe(CHSV color, int numFrame) {
  for (int frame = 0; frame < numFrame; frame++) {
    for (int led = 0; led < VIRTUAL_NUM_LEDS; led++) {
     setLed(led, CHSV(color.hue, 255, map(abs(sin(float(frame)/20))*255, 0, 255, 0, BRIGHTNESS)));
    }
    FastLED.show();
    delay(40);
  }
}
// END PATTERNS

// virtual to real mapping
void setLed(int led, CHSV color) {
  if(led < 1) {
    head[23] = color;
    head[24] = color;
  } else if (led < HEAD_LEG_CROSSOVER) {
    head[map(led, 1, HEAD_LEG_CROSSOVER, 22, 0)] = color;
  } else {
    arm1[map(led, HEAD_LEG_CROSSOVER, VIRTUAL_NUM_LEDS, 0, LEG_NUM_LEDS)] = color;
    arm2[map(led, HEAD_LEG_CROSSOVER, VIRTUAL_NUM_LEDS, 0, LEG_NUM_LEDS)] = color;
  }
}