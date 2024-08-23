#include <FastLED.h>
#define VIRTUAL_NUM_LEDS 100
#define LEG_NUM_LEDS 75
#define HEAD_NUM_LEDS 25
#define BRIGHTNESS 100
#define HEAD_LEG_CROSSOVER 25

CRGB arm[LEG_NUM_LEDS];
CRGB head[HEAD_NUM_LEDS];

// Parameters for the wave pattern
float waveSpeed = 0.1;   // Speed of the wave
float waveFrequency = 0.2; // Frequency of the wave
float time = 0;  // Time variable for animation

void setup() {
  FastLED.addLeds<NEOPIXEL, 1>(arm, LEG_NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 2>(arm, LEG_NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 3>(arm, LEG_NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 4>(arm, LEG_NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 5>(arm, LEG_NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 6>(arm, LEG_NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 7>(arm, LEG_NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 8>(arm, LEG_NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 9>(head, HEAD_NUM_LEDS);
}

void loop() {
  showRainbow(1000);
  breathe(CHSV(random(0, 255),0,0), 1000);
  wave(1000);
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

void wave(int numFrame) {
  for (int frame = 0; frame < numFrame; frame++) {
    time += 0.1;  // Increment time to create the wave effect
  
    // Wrap the time value to prevent overflow
    if (time > 1000) {
      time -= 1000;
    }

    for (int i = 0; i < VIRTUAL_NUM_LEDS; i++) {
      // Calculate the position of the wave for each LED
      float wave = (sin(i * 0.2 + time) + 1.0) / 2.0; // Sine wave calculation

      // Generate a random color for each LED
      CHSV randomColor = CHSV(random(0, 255), 255, 255);

      // Map the wave value to the brightness of the LED
      setLed(i, randomColor);
      fadeToBlackBy(i, 255 * (1.0 - wave));
    }
  }
  FastLED.show();
  delay(40);
}

// virtual to real mapping
void setLed(int led, CHSV color) {  
  if(led < 1) {
    head[23] = color;
    head[24] = color;
  } else if (led < HEAD_LEG_CROSSOVER) {
    head[map(led, 1, HEAD_LEG_CROSSOVER, 22, 0)] = color;
  } else {
    arm[map(led, HEAD_LEG_CROSSOVER, VIRTUAL_NUM_LEDS, 0, LEG_NUM_LEDS)] = color;
  }
}

void fadeToBlackBy(int led, float amount) {
  if(led < 1) {
    
  } else if (led < HEAD_LEG_CROSSOVER) {
    head[map(led, 1, HEAD_LEG_CROSSOVER, 22, 0)].fadeToBlackBy(amount);
  } else {
    arm[map(led, HEAD_LEG_CROSSOVER, VIRTUAL_NUM_LEDS, 0, LEG_NUM_LEDS)].fadeToBlackBy(amount);
  }
}