/* to-do list:
 *  
 *  make DoLeds Modulaur
 *    Fix any led_string1's
 *    add a "firework number" inou\\put
 *    make ring selector an array
 *    make rinng time a 2D array
 *    make starting colors a 2D array
 *  make DoBigLeds actually functional
 *    Coppy the ode once you make doLeds work the way ou want
 *    
 *  minimize globals so theres actuallyenough space
 *  
 *  COMMENT. YOUR FUCKING. SHIT.
 *  
 *  
 *  
 *  HO.
 */

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <FastLED.h>
#include <math.h>

Adafruit_LSM303_Accel_Unified sensoor = Adafruit_LSM303_Accel_Unified(54321);

#define NUM_LEDS 22
#define LED_STRING_1 12
#define LED_STRING_2 6
#define LED_STRING_3 7
const int ringStartDelayInit = 4259;
const int ringDelayInit = 100;
const int fadeSpeedInit = 5;

CRGB led_string1[NUM_LEDS];
//CRGB led_string2[NUM_LEDS];
//CRGB led_string3[NUM_LEDS];
int ringStartDelay = 4259;
int ringDelay = 100;
long ringTime [NUM_LEDS];
int startingColors[3];
int fadeSpeed = 5;
long currentTime;
int ringSelector = 0;


int acceleration[15] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
int avgAccel;
int currentAccInd;


void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<NEOPIXEL, LED_STRING_1>(led_string1, NUM_LEDS);
  //  FastLED.addLeds<NEOPIXEL, LED_STRING_2>(led_string2, NUM_LEDS);
  //  FastLED.addLeds<NEOPIXEL, LED_STRING_3>(led_string3, NUM_LEDS);



  Serial.begin(9600);
  Wire.begin();

  sensoor.begin();

  displaySensorDetails();
  delay(1000);

  startingColors[0] = 180;
  startingColors[1] = 255;
  startingColors[2] = 0;


  for (int i = 1; i < NUM_LEDS; i++) {
    led_string1[i].setRGB( 0, 0, 0);
    //    led_string2[i].setRGB( 185, 185, 185);
    //    led_string3[i].setRGB( 185, 185, 185);
  }
  for (int i = 0; i < NUM_LEDS; i++) {
    ringTime[i] = 0;
  }
}






void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 5; i++) {
    doLEDs(led_string1);
    delay (33);
  }
  calSpeed();
  Serial.print("test");
  //  calAvgAccel();

}




void doBigLEDs(CRGB ledString[]) {

  //  if (!ledString[NUM_LEDS - 1] && ringTime[NUM_LEDS - 1] > ringTime[0]) {
  //    currentTime = millis();
  //    ringTime[0] = currentTime;
  //  }
  //
  //  if (currentTime - ringTime[0] >= ringStartDelay && !ledString[0]) {
  //    ledString[0].setRGB (startingColors[0], startingColors[1], startingColors[2]);
  //  }
  //  ledString[0].fadeToBlackBy(fadeSpeed);
  currentTime = millis();//set the current time
  for (int current_led = 0; current_led < NUM_LEDS; current_led++) { //for every LED...
    //    Serial.print("Current led being updated is:"); Serial.print(current_led); Serial.print(" time is :"); Serial.println(currentTime);

    ledString[current_led].fadeToBlackBy(fadeSpeed);// fade to black by FadeSpeed/256'THS
    if (current_led != 0) {// if not the first LED in the string...




      if (currentTime - ringTime[current_led - 1] >= ringDelay && ringSelector == current_led) { // when the timer for the previous ring has exceeded the delay time
        ringTime[current_led] = currentTime;
        ledString[current_led].setRGB (startingColors[0], startingColors[1], startingColors[2]);
        if (current_led == 1) {
          ringSelector += 6;
        } else if (current_led==7) {
          ringSelector += 12;
        }else {
          ringSelector = (ringSelector + 1) % (NUM_LEDS);
        }


      }

      //      Serial.print("Color Values:"); Serial.print(current_led); Serial.print(" Ring Time is :"); Serial.println(ringTime[current_led]);
      if (current_led == 1) {
        led_string1[2] = led_string1[1]; led_string1[3] = led_string1[1]; led_string1[4] = led_string1[1]; led_string1[5] = led_string1[1]; led_string1[6] = led_string1[1];
        ringTime[2] = ringTime[1]; ringTime[3] = ringTime[1]; ringTime[4] = ringTime[1]; ringTime[5] = ringTime[1]; ringTime[6] = ringTime[1];
        current_led += 5;
      }

      if (current_led == 7) {
        for (int i = 0; i<12; i++) {
          ledString[8+i] = ledString[7];
          ringTime[8+i] = ringTime[7];
        }
        current_led += 11;
      }
    } else {



      if (ringSelector == 0 && !ledString[NUM_LEDS - 1] && currentTime - ringTime[0] >= ringStartDelay) { // if the last light has turned off and the timer for the
        //Last Led in the string has been updated more recently than
        //the first...
        currentTime = millis();
        ringTime[0] = currentTime;
        ledString[0].setRGB (startingColors[0], startingColors[1], startingColors[2]);
        ringSelector++;
      }



    }
  }

  Serial.print("Current time is:"); Serial.print(currentTime); Serial.print(" Ring Time is :"); Serial.println(ringTime[0]);
  FastLED.show();
}




void doLEDs(CRGB ledString[]) {

  //  if (!ledString[NUM_LEDS - 1] && ringTime[NUM_LEDS - 1] > ringTime[0]) {
  //    currentTime = millis();
  //    ringTime[0] = currentTime;
  //  }
  //
  //  if (currentTime - ringTime[0] >= ringStartDelay && !ledString[0]) {
  //    ledString[0].setRGB (startingColors[0], startingColors[1], startingColors[2]);
  //  }
  //  ledString[0].fadeToBlackBy(fadeSpeed);
  currentTime = millis();//set the current time
  for (int current_led = 0; current_led < NUM_LEDS; current_led++) { //for every LED...
    //    Serial.print("Current led being updated is:"); Serial.print(current_led); Serial.print(" time is :"); Serial.println(currentTime);

    ledString[current_led].fadeToBlackBy(fadeSpeed);// fade to black by FadeSpeed/256'THS
    if (current_led != 0) {// if not the first LED in the string...




      if (currentTime - ringTime[current_led - 1] >= ringDelay && ringSelector == current_led) { // when the timer for the previous ring has exceeded the delay time
        ringTime[current_led] = currentTime;
        ledString[current_led].setRGB (startingColors[0], startingColors[1], startingColors[2]);
        if (current_led == 1) {
          ringSelector += 6;
        } else {
          ringSelector = (ringSelector + 1) % (NUM_LEDS);
        }


      }

      //      Serial.print("Color Values:"); Serial.print(current_led); Serial.print(" Ring Time is :"); Serial.println(ringTime[current_led]);
      if (current_led == 1) {
        led_string1[2] = led_string1[1]; led_string1[3] = led_string1[1]; led_string1[4] = led_string1[1]; led_string1[5] = led_string1[1]; led_string1[6] = led_string1[1];
        ringTime[2] = ringTime[1]; ringTime[3] = ringTime[1]; ringTime[4] = ringTime[1]; ringTime[5] = ringTime[1]; ringTime[6] = ringTime[1];
        current_led += 5;
      }
    } else {



      if (ringSelector == 0 && !ledString[NUM_LEDS - 1] && currentTime - ringTime[0] >= ringStartDelay) { // if the last light has turned off and the timer for the
        //Last Led in the string has been updated more recently than
        //the first...
        currentTime = millis();
        ringTime[0] = currentTime;
        ledString[0].setRGB (startingColors[0], startingColors[1], startingColors[2]);
        ringSelector++;
      }



    }
  }

  Serial.print("Current time is:"); Serial.print(currentTime); Serial.print(" Ring Time is :"); Serial.println(ringTime[0]);
  FastLED.show();
}



void calSpeed() {
  long temp;
  int tempAvg;
  temp = map(calAvgAccel(), 10, 30, 1, 35);

  Serial.print(calAvgAccel());

  acceleration[currentAccInd] = temp;
  currentAccInd = (currentAccInd + 1) % 15;
  for (int i = 0; i < 15; i++) {
    tempAvg += acceleration[i];
  }
  avgAccel = tempAvg / 15;
  ringStartDelay = ringStartDelayInit / avgAccel;
  fadeSpeed = fadeSpeedInit * avgAccel;
  ringDelay = ringDelayInit / avgAccel;

};
void displaySensorDetails(void)
{
  sensor_t sensor;
  sensoor.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

float calAvgAccel () {
  sensors_event_t accEvent;
  sensoor.getEvent(&accEvent);

  float x, y, z;
  x = abs((float)accEvent.acceleration.x) + 1;
  y = abs((float)accEvent.acceleration.y) + 1;
  z = abs((float)accEvent.acceleration.z) + 1;
  //  Serial.print("x: " + (String)x);
  //  Serial.print("y: " + (String)y);
  //  Serial.print("z: " + (String)z);

  return abs(sqrt(pow(x, 2) + pow(y, 2) + (pow(z, 2))));
}

