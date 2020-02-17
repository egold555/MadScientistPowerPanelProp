
#define AMT_OF_METERS 4
#define AMT_OF_LIGHTS 4

int meterPins[AMT_OF_METERS] = {9, 10, 11, 6};
int meterPosition[AMT_OF_METERS];
int meterSpeed[AMT_OF_METERS] = {5, 15, 25, 1};

int lightPins[AMT_OF_LIGHTS] = {2, 4, 7, 8};
boolean lightValues[AMT_OF_LIGHTS];
long lightLastChangeTime;

long plasmaBallLastToggle;

void setup()
{

  for (int i = 0; i < AMT_OF_METERS; i++)
  {
    pinMode(meterPins[i], OUTPUT);
  }
}

void loop()
{

  loopMeeters();
  loopLights();

  delay(30);
}

void loopLights()
{
  for(int i = 0; i < AMT_OF_LIGHTS; i++) {

  }
}

void loopMeeters()
{

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