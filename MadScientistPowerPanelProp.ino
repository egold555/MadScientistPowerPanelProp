
#include <FastLED.h>

#define AMT_OF_METERS 4
#define AMT_OF_LIGHTS 4
#define AMT_OF_FAIRY_LIGHTS 2
#define AMT_OF_LEDS_STRIP 28

//USED PINS: 2, 4, 5 ~6 7 8 ~9 ~10 ~11 12 ~13
//UNUSED PINS: 3, A3, A4, A5

int METER_PINS[AMT_OF_METERS] = {9, 10, 11, 6};
int meterPosition[AMT_OF_METERS];
int meterSpeed[AMT_OF_METERS] = {5, 15, 25, 1};
long meterLastChangeTime;

int LIGHT_PINS[AMT_OF_LIGHTS] = {2, 4, 7, 8};
long lightLastChangeTime;

#define PLASMA_BALL_PIN 12
long plasmaBallLastToggle = 0;
const long PLASMA_BALL_TIMER = 4L * 60L * 1000L; //overflows before converted to a long. dumb C++ man

#define MICROPHONE_PIN A3
const int SENSITIVITY = 100;

#define EMPTY_A_PIN_FOR_RANDOM A0

uint8_t FAIRY_LIGHT_PINS[AMT_OF_FAIRY_LIGHTS] = {A1, A2};

#define LED_STRIP_PIN 5
CRGB ledStrip[AMT_OF_LEDS_STRIP];

void setup()
{

  //init all pins
  for (int i = 0; i < AMT_OF_METERS; i++)
  {
    pinMode(METER_PINS[i], OUTPUT);
  }

  for (int i = 0; i < AMT_OF_LIGHTS; i++)
  {
    pinMode(LIGHT_PINS[i], OUTPUT);
  }

  pinMode(PLASMA_BALL_PIN, OUTPUT);
  //pinMode(MICROPHONE_PIN, INPUT); //Not needed for input A pins randomly?

  for (int i = 0; i < AMT_OF_FAIRY_LIGHTS; i++)
  {
    pinMode(FAIRY_LIGHT_PINS[i], OUTPUT);
    digitalWrite(FAIRY_LIGHT_PINS[i], LOW);
  }

  LEDS.addLeds<WS2812, LED_STRIP_PIN, GRB>(ledStrip, AMT_OF_LEDS_STRIP);

  setLedColor();

  //Random seed
  randomSeed(analogRead(EMPTY_A_PIN_FOR_RANDOM));
}

void setLedColor(){
  for (int i = 0; i < AMT_OF_LEDS_STRIP; i++)
  {
    // Set the i'th led to red
    ledStrip[i] = CRGB(189, 75, 4);
    // Show the leds
    FastLED.show();
  }
}

void loop()
{

  loopMicrophone();
  loopMeters();
  loopLights();
  //loopLedStripRainbowTest();
  loopPlasmaBall();

  //delay(30);
}

void togglePin(int pin)
{
  digitalWrite(pin, !digitalRead(pin));
}

void loopMicrophone()
{
  int sensorValue = analogRead(MICROPHONE_PIN);

  if (sensorValue > (340 + SENSITIVITY) || sensorValue < (340 - SENSITIVITY))
  {
    for (int i = 0; i < AMT_OF_FAIRY_LIGHTS; i++)
    {
      digitalWrite(FAIRY_LIGHT_PINS[random(AMT_OF_FAIRY_LIGHTS)], HIGH);
    }
    delay(20);
    for (int i = 0; i < AMT_OF_FAIRY_LIGHTS; i++)
    {
      digitalWrite(FAIRY_LIGHT_PINS[i], LOW);
    }
  }
}

void loopPlasmaBall()
{
  //Needs to toggle plasma ball pin, 500 ms apart every 40000ms
  long currentMillis = millis();
  if (plasmaBallLastToggle < currentMillis)
  {
    plasmaBallLastToggle = (currentMillis + PLASMA_BALL_TIMER);

    //double click plasma ball button
    togglePin(PLASMA_BALL_PIN);
    delay(200);
    togglePin(PLASMA_BALL_PIN);

    delay(500);

    togglePin(PLASMA_BALL_PIN);
    delay(200);
    togglePin(PLASMA_BALL_PIN);
  }
}

void loopLights()
{
  long currentMillis = millis();
  if (lightLastChangeTime < currentMillis)
  {
    lightLastChangeTime = (currentMillis + random(50, 2000));
    int ledToChange = LIGHT_PINS[random(4)];
    togglePin(ledToChange);
  }
}

void loopMeters()
{

  long currentMillis = millis();
  if (meterLastChangeTime < currentMillis)
  {

    meterLastChangeTime = (currentMillis + 30);

    for (int i = 0; i < AMT_OF_METERS; i++)
    {

      analogWrite(METER_PINS[i], meterPosition[i]);

      meterPosition[i] = meterPosition[i] + meterSpeed[i];

      if (meterPosition[i] < 0)
      {
        meterPosition[i] = 0;
        meterSpeed[i] = -meterSpeed[i];
      }

      if (meterPosition[i] > 255)
      {
        meterPosition[i] = 255;
        meterSpeed[i] = -meterSpeed[i];
      }
    }
  }
}

void loopLedStripRainbowTest()
{
  static uint8_t hue = 0;
  // First slide the led in one direction
  for (int i = 0; i < AMT_OF_LEDS_STRIP; i++)
  {
    // Set the i'th led to red
    ledStrip[i] = CHSV(hue++, 255, 255);
    // Show the leds
    FastLED.show();
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(10);
  }
}

void fadeall()
{
  for (int i = 0; i < AMT_OF_LEDS_STRIP; i++)
  {
    ledStrip[i].nscale8(250);
  }
}
