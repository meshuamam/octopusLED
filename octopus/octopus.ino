#include <FastLED.h>
#define VIRTUAL_NUM_LEDS 100
#define LEG_NUM_LEDS 75
#define HEAD_NUM_LEDS 25
#define BRIGHTNESS 100
#define HEAD_LEG_CROSSOVER 25
CRGB arm1[LEG_NUM_LEDS];
CRGB arm2[LEG_NUM_LEDS];
CRGB arm3[LEG_NUM_LEDS];
CRGB arm4[LEG_NUM_LEDS];
CRGB arm5[LEG_NUM_LEDS];
CRGB arm6[LEG_NUM_LEDS];
CRGB arm7[LEG_NUM_LEDS];
CRGB arm8[LEG_NUM_LEDS];
CRGB head[HEAD_NUM_LEDS];
void setup() {
  FastLED.addLeds<NEOPIXEL, 1>(arm1, LEG_NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 2>(arm2, LEG_NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 3>(arm3, LEG_NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 4>(arm4, LEG_NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 5>(arm5, LEG_NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 6>(arm6, LEG_NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 7>(arm7, LEG_NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 8>(arm8, LEG_NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 0>(head, HEAD_NUM_LEDS);
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
    arm3[map(led, HEAD_LEG_CROSSOVER, VIRTUAL_NUM_LEDS, 0, LEG_NUM_LEDS)] = color;
    arm4[map(led, HEAD_LEG_CROSSOVER, VIRTUAL_NUM_LEDS, 0, LEG_NUM_LEDS)] = color;
    arm5[map(led, HEAD_LEG_CROSSOVER, VIRTUAL_NUM_LEDS, 0, LEG_NUM_LEDS)] = color;
    arm6[map(led, HEAD_LEG_CROSSOVER, VIRTUAL_NUM_LEDS, 0, LEG_NUM_LEDS)] = color;
    arm7[map(led, HEAD_LEG_CROSSOVER, VIRTUAL_NUM_LEDS, 0, LEG_NUM_LEDS)] = color;
    arm8[map(led, HEAD_LEG_CROSSOVER, VIRTUAL_NUM_LEDS, 0, LEG_NUM_LEDS)] = color;
  }
}

// Function to print colors from an LED array
void printLedArray(const CRGB* leds, int numLeds, const String& arrayName) {
  Serial.println("Current colors in " + arrayName + ":");
  for (int i = 0; i < numLeds; i++) {
    CRGB led = leds[i];
    Serial.print("Index ");
    Serial.print(i);
    Serial.print(": (");
    Serial.print(led.r);
    Serial.print(", ");
    Serial.print(led.g);
    Serial.print(", ");
    Serial.print(led.b);
    Serial.println(")");
  }
}