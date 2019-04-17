
#include <FastLED.h>
#define NUM_LEDS 8
#define LED_STRING_1 10
#define LED_STRING_2 6
#define LED_STRING_3 7

#define DELAY 10
#define FADED_INTENSITY 2
#define FADED_DELAY 25
#define DELAY_START 10

CRGB led_string1[NUM_LEDS];
//CRGB led_string2[NUM_LEDS];
//CRGB led_string3[NUM_LEDS];
boolean dimOrLight[NUM_LEDS];
int nextBoom[NUM_LEDS];
int startingColors[3];
int waveDelay = 50;
int startDelay = 100;

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<NEOPIXEL, LED_STRING_1>(led_string1, NUM_LEDS);
  //  FastLED.addLeds<NEOPIXEL, LED_STRING_2>(led_string2, NUM_LEDS);
  //  FastLED.addLeds<NEOPIXEL, LED_STRING_3>(led_string3, NUM_LEDS);

  for (int i = 0; i < NUM_LEDS; i++) {
    nextBoom[i] = FADED_DELAY + (i * 30);
  }

  startingColors[0] = random8(255);
  startingColors[1] = random8(255);
  startingColors[2] = random8(255);


  for (int i = 0; i < NUM_LEDS; i++) {
    led_string1[i].setRGB( startingColors[0] - (10 * i), startingColors[1] - (10 * i), startingColors[2] - (10 * i));
    //    led_string2[i].setRGB( 185, 185, 185);
    //    led_string3[i].setRGB( 185, 185, 185);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  //  led_string1, led_string2, led_string3 |= MIN_BRIGHTNESS;

  for (int current_led = 0; current_led < NUM_LEDS; current_led++) {
    //LED string 1
    led_string1[current_led].fadeToBlackBy(FADED_INTENSITY);
    //    if (led_string1[current_led] >= CRGB(DELAY_START, DELAY_START, DELAY_START)) {
    Serial.println(nextBoom[current_led]);
    //    } else { // assume led is below x brightness


    /*if ((led_string1[NUM_LEDS - 1] && !led_string1[0])|(led_string1[NUM_LEDS - 1] && led_string1[0])) {//dont do unless both first and last led are off
    } else*/ if (nextBoom[current_led] <= millis()) {//set light if previous timer is 
      led_string1[current_led].setRGB (startingColors[0], startingColors[1], startingColors[2]);
      nextBoom[current_led+1] = millis()+waveDelay;
    }







    //      if (!led_string1[NUM_LEDS - 1] && current_led == 0) {
    //        nextBoom[current_led] =;
    //      } else if (nextBoom[current_led] <= millis() + FADED_DELAY &&  nextBoom[0] == FADED_DELAY) {
    //        led_string1[current_led].setRGB (startingColors[0], startingColors[1], startingColors[2]);
    //        nextBoom[current_led] = millis();
    //      }
    if (current_led >= 1 && current_led < 7 ) {
    led_string1[current_led] = led_string1[1];
    nextBoom[current_led] = nextBoom[1];
  }
  }
  //    if (nextBoom[0] < millis()-startDelay) {
  //        led_string1[0].setRGB (startingColors[0], startingColors[1], startingColors[2]);
  //      }

  
  //  }
  if (!led_string1[NUM_LEDS - 1] && !led_string1[0]) {
    nextBoom[0] = millis()+startDelay;
  }
  FastLED.show();


  delay(DELAY + random8(0, 9));

}

CRGB Dim (int DimBy, CRGB led) {
  led.setRGB (led.r - DimBy, led.g - DimBy, led.b - DimBy);
  return led;
}


