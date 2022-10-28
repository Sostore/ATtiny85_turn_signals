#include "buttonHandler.h"

buttonHandler left(LEFT_IN, LEFT_OUT);
buttonHandler right(RIGHT_IN, RIGHT_OUT);

void setup() {
  pinMode(LEFT_IN, INPUT_PULLUP);
  pinMode(RIGHT_IN, INPUT_PULLUP);

  pinMode(LEFT_OUT, OUTPUT);
  pinMode(RIGHT_OUT, OUTPUT);
}

void loop() {
  if (left.tick())
  {
    right.reset();
  }

  if (right.tick())
  {
    left.reset();
  }

}