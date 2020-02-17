
#define AMT_OF_METERS 4
#define AMT_OF_LIGHTS 4

//USED PINS: 2 4 6P 7 8 9P 10P 11P 12
//UNUSED PINS: 3P 5P 13

int meterPins[AMT_OF_METERS] = {9, 10, 11, 6};
int meterPosition[AMT_OF_METERS];
int meterSpeed[AMT_OF_METERS] = {5, 15, 25, 1};
long meterLastChangeTime;

int lightPins[AMT_OF_LIGHTS] = {2, 4, 7, 8};
long lightLastChangeTime;

int plasmaBallButtonPin = 12;
long plasmaBallLastToggle;

void setup()
{

  //init all pins
  for (int i = 0; i < AMT_OF_METERS; i++)
  {
    pinMode(meterPins[i], OUTPUT);
  }

  for (int i = 0; i < AMT_OF_LIGHTS; i++)
  {
    pinMode(lightPins[i], OUTPUT);
  }

  pinMode(plasmaBallButtonPin, OUTPUT);

  //Random seed
  randomSeed(analogRead(0));
}

void loop()
{

  loopMeters();
  loopLights();
  loopPlasmaBall();

  //delay(30);
}

void togglePin(int pin)
{
  digitalWrite(pin, !digitalRead(pin));
}

void loopPlasmaBall()
{
  //Needs to toggle plasma ball pin, 500 ms apart every 40000ms
  long currentMillis = millis();
  if (plasmaBallLastToggle < currentMillis)
  {
    plasmaBallLastToggle = (currentMillis + 10000);

    //double click plasma ball button
    togglePin(plasmaBallButtonPin);
    delay(200);
    togglePin(plasmaBallButtonPin);

    delay(500);

    togglePin(plasmaBallButtonPin);
    delay(200);
    togglePin(plasmaBallButtonPin);
    
  }
}

void loopLights()
{
  long currentMillis = millis();
  if (lightLastChangeTime < currentMillis)
  {
    lightLastChangeTime = (currentMillis + random(50, 2000));
    int ledToChange = lightPins[random(4)];
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

      analogWrite(meterPins[i], meterPosition[i]);

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
