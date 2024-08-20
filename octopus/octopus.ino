#include <FastLED.h>
#define VIRTUAL_NUM_LEDS 100
#define LEG_NUM_LEDS 75
#define HEAD_NUM_LEDS 25
#define BRIGHTNESS 100
#define HEAD_LEG_CROSSOVER 25

// Fire defs
#define FIRE_COUNT 7

#define FLARE_CHANCE 5

#define FALLING_STATE 1
#define FLARING_STATE 2
#define RESTING_STATE 3
#define FALLING_FRAMES 10
#define FLARING_FRAMES 2

// stores the current intensity of the fire (1-100)
short fireIntensity[FIRE_COUNT];
// stores the current state of the fire (RESTING|FLARING|FALLING)
short fireState[FIRE_COUNT];
// stores the current time left in a given fire state
short fireFrame[FIRE_COUNT];
// stores the current target of the fire
short fireTarget[FIRE_COUNT];

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
  for (short i = 0; i < FIRE_COUNT; i++) {
    fireIntensity[i] = 20 + random(-10, 10);
    fireState[i] = RESTING_STATE;
  }
  
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
  breathe(CHSV(random(256),0,0), 1000);
  displayFires(1000);
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

void displayFires(int numFrame) {
  for (int frame = 0; frame < numFrame; frame++) {
    updateFires();
    for (short i = 0; i < VIRTUAL_NUM_LEDS; i += 2) {
      setLed(i, getFireColor(fireIntensity[i / 2]));
    }
    for (short i = 1; i < VIRTUAL_NUM_LEDS; i += 2) {
      short avg = (fireIntensity[i / 2] + fireIntensity[(i / 2 + 1) % FIRE_COUNT]) / 2;
      setLed(i, getFireColor(avg));
    }
    FastLED.show();
    delay(40);
  }
}

// END PATTERNS

CHSV getFireColor(short intensity) {
  return CHSV(map(intensity, 0, 100, 35, 0), 255, map(intensity, 0, 100, 0, 100));
}

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

// START FIRE STATE MACHINE CODE

void updateFires() {
  for (short i = 0; i < FIRE_COUNT; i++) {
    updateIntensity(i);
  }
}

void flare(short i) {
  fireState[i] = FLARING_STATE;
  short flareTarget = fireIntensity[i] + random(45, 70);
  fireTarget[i] = min(100, flareTarget);
  fireFrame[i] = FLARING_FRAMES;
}

void fall(short i) {
  fireState[i] = FALLING_STATE;
  fireTarget[i] = 20 + random(-10, 10);
  fireFrame[i] = FALLING_FRAMES;
}

void rest(short i) {
  fireState[i] = RESTING_STATE;
}

void updateIntensity(short i) {
  if (fireState[i] == FLARING_STATE) {
    fireIntensity[i] = fireIntensity[i] + ((float(FLARING_FRAMES - fireFrame[i]) / FLARING_FRAMES) * (fireTarget[i] - fireIntensity[i]));
    fireFrame[i] = fireFrame[i] - 1;
    if (fireFrame[i] < 0) {
      fall(i);
    }
  } else if (fireState[i] == RESTING_STATE) {
    short newIntensity = fireIntensity[i] + random(-3, 3);
    fireIntensity[i] = constrain(newIntensity, 0, 100);
    if (random(0, 100) < FLARE_CHANCE) {
      flare(i);
    }
  } else if (fireState[i] == FALLING_STATE) {
    fireIntensity[i] = fireIntensity[i] - ((float(FALLING_FRAMES - fireFrame[i]) / FALLING_FRAMES) * (fireIntensity[i] - fireTarget[i]));
    fireFrame[i] = fireFrame[i] - 1;
    if (fireFrame[i] < 0) {
      rest(i);
    }
    if (random(0, 100) < FLARE_CHANCE) {
      flare(i);
    }
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