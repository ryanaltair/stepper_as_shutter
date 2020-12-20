// 1 round = 6mm
// 40000 pulse = 1 round
// 10 hz
// 1000*1000/400= 2500
const int pulse_per_rev = 40000;
const bool DirCCW = HIGH;
const bool DirCW = LOW;
const bool StatusON = LOW;
const bool StatusOFF = HIGH;
int onoffSteps = 10000;
//int onoffSteps = 400;// switch
bool currentStatus = StatusOFF;
bool lastStatus = StatusOFF;
bool statusEnable = HIGH;
bool statusDisable = LOW;
int pin_onoff = 9;
int pin_work = 8;

// switch
int pin_step = 4;
int pin_dir = 3;
int pin_enable = 12;
int minPos = 0;
int maxPos = 400;
int initPos = 0;
int currentPos = 0;
int pulseInterval = 1; // in us
int pinFake = 13;
void onestep()
{
  delayMicroseconds(pulseInterval);
  digitalWrite(pin_step, HIGH);
  delayMicroseconds(pulseInterval);
  digitalWrite(pin_step, LOW);
}
void manystep(int dir, int count)
{
  digitalWrite(pin_dir, dir);
  delay(1);
  for (int i = 0; i < count; i++)
  {
    onestep();
  }
}
void setup()
{
  pinMode(pin_work, INPUT_PULLUP);
  pinMode(pin_onoff, INPUT_PULLUP);
  pinMode(pin_dir, OUTPUT);
  pinMode(pin_step, OUTPUT);
  pinMode(pin_enable, OUTPUT);
  pinMode(pinFake, OUTPUT);
  Serial.begin(250000);
}

void loop1()
{
  digitalWrite(pin_enable, statusEnable);
  delay(1);
  manystep(DirCW, onoffSteps);
  delay(2000);
  manystep(DirCCW, onoffSteps);
  delay(2000);
}
int readOnOff()
{
  int count = 10;
  bool currentCheck = digitalRead(pin_onoff);
  for (int i = 0; i < count; i++)
  {
    if (digitalRead(pin_onoff) != currentCheck)
    {
      return 2;
    }
    delay(1);
  }
  return currentCheck;
}
int statusFake = 0;
void loop()
{
  statusFake = !statusFake;
  digitalWrite(pinFake, statusFake);
  //  delay(300);
  if (digitalRead(pin_work) == LOW || true)
  {
    digitalWrite(pin_enable, statusEnable);
    bool check = readOnOff();
    if (check == 2)
    {
      return;
    }
    if (check == StatusON)
    {
      currentStatus = StatusON;
    }
    else
    {
      currentStatus = StatusOFF;
    }
    if (lastStatus != currentStatus)
    {
      if (currentStatus == StatusON)
      {
        manystep(DirCCW, onoffSteps);
      }
      else
      {
        manystep(DirCW, onoffSteps);
      }
      lastStatus = currentStatus;
    }
  }
  else
  {
    digitalWrite(pin_enable, statusDisable);
  }
}
