/*
  For Digispark ATtiny85
  Made by Sostore
  Github: github.com/sostore
*/

// Порты ввода
#define LEFT_IN 2
#define RIGHT_IN 3
// Порты вывода
#define LEFT_OUT 4
#define RIGHT_OUT 5

// Максимальное и минимальное время сигнала
#define HOLD_TIME_MIN 100
#define HOLD_TIME_MAX 1000

// Время вывода
#define SIGNAL_OUT_TIME 3000

unsigned long signalInTime = 0;
bool isReading = false;
int signalLeftTime, signalRightTime = 0;

bool isTimerStarted = false;
unsigned int timerEnd = 0;

int leftState, rightState;
int lastLeftButtonState, lastRightButtonState = LOW;

unsigned long lastLeftDebounceTime, lastRightDebounceTime = 0;
// Задержка от дребезга кнопки
unsigned long debounceDelay = 50;

void handleButton(
  int* reading,
  unsigned long* lastDebounceTime,
  int* state,
  int* lastButtonState,
  int outPin,
  int* signalTime
)
{
  // Защита от дребезга
  if (*reading != *lastButtonState)
  {
    *lastDebounceTime = millis();
  }

  if ((millis() - *lastDebounceTime) > debounceDelay) {
    if (*reading != *state) {
      *state = *reading;

      if (*state == HIGH && !isReading) {
        signalInTime = millis();
        isReading = true;
      }

      if (*state == LOW && isReading)
      {
        isReading = false;
      }
    }
    else if (*state == HIGH && isReading)
    {
      *signalTime = millis() - signalInTime;
    }
  }

  *lastButtonState = *reading;
  
  // Проверка времени и окончания сигнала
  if (!isReading && (*signalTime >= HOLD_TIME_MIN) && (*signalTime <= HOLD_TIME_MAX))
  {
    // Старт таймера
    if (!isTimerStarted)
    {
      // Отдаем высокий сигнал на нужный пин
      digitalWrite(outPin, HIGH);
      // Время окончания таймера (текущее время + 3 секунды)
      timerEnd = millis() + SIGNAL_OUT_TIME;
      // Флаг старта таймера
      isTimerStarted = true;
    }

    // Конец таймера
    if (isTimerStarted && millis() >= timerEnd)
    {
      digitalWrite(outPin, LOW);
      *signalTime = 0;
      isTimerStarted = false;
    }
  }
}

void setup() {
  pinMode(LEFT_IN, INPUT);
  pinMode(RIGHT_IN, INPUT);

  pinMode(LEFT_OUT, OUTPUT);
  pinMode(RIGHT_OUT, OUTPUT);
}

void loop() {
  // This is where the magic happens
  // Читаем левый и правый сигнал
  int leftButtonReading = !digitalRead(LEFT_IN);
  int rightButtonReading = !digitalRead(RIGHT_IN);

  // Функция, измеряющая время сигнала и убирающая дребезг с кнопки.
  // Кидаем функцию на левую кнопку
  handleButton(&leftButtonReading,
               &lastLeftDebounceTime,
               &leftState, 
               &lastLeftButtonState, 
               LEFT_OUT,
               &signalLeftTime
              );
              
  // Дублируем функцию на правую кнопку.
  handleButton(&rightButtonReading,
               &lastRightDebounceTime,
               &rightState, 
               &lastRightButtonState, 
               RIGHT_OUT,
               &signalRightTime
              );
}