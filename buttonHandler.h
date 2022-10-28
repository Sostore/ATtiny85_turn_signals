#define LEFT_IN 2
#define RIGHT_IN 3
#define LEFT_OUT 4
#define RIGHT_OUT 5

#define HOLD_TIME_MIN 100
#define HOLD_TIME_MAX 1000
#define SIGNAL_OUT_TIME 3000

class buttonHandler
{
  public:
    buttonHandler(int pin, int out);
    bool tick();
    void reset();
  private:
    unsigned long signalInTime = 0;
    unsigned long lastDebounceTime = 0;
    unsigned long debounceDelay = 50;

    int outPin;
    int readPin;
    int signalTime = 0;
    int state;
    int lastButtonState = HIGH;
    int timerEnd;
    
    bool isReading = false;
    bool isTimerStarted = false;
};

buttonHandler::buttonHandler(int pin, int out)
{
  outPin = out;
  readPin = pin;
}

void buttonHandler::reset()
{
  timerEnd = 0;
}

bool buttonHandler::tick()
{
  int reading = !digitalRead(readPin);

  if (reading != lastButtonState)
  {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != state) {
      state = reading;

      if (state == HIGH && !isReading) {
        signalInTime = millis();
        isReading = true;
      }

      if (state == LOW && isReading)
      {
        isReading = false;
      }
    }
    else if (state == HIGH && isReading)
    {
      signalTime = millis() - signalInTime;
    }
  }

  lastButtonState = reading;
  
  if (!isReading && (signalTime >= HOLD_TIME_MIN) && (signalTime <= HOLD_TIME_MAX))
  {
    if (!isTimerStarted)
    {
      
      digitalWrite(outPin, HIGH);
      timerEnd = millis() + SIGNAL_OUT_TIME;
      isTimerStarted = true;
    }

    if (isTimerStarted && millis() >= timerEnd)
    {
      digitalWrite(outPin, LOW);
      signalTime = 0;
      isTimerStarted = false;
    }
  }
  
  return isTimerStarted;
}