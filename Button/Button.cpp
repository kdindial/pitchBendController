#include "Arduino.h"
#include "Button.h"

Button::Button(int _buttonPin, int _buttonNum)
{
  buttonPin = _buttonPin;
  buttonNum = _buttonNum;
  buttonState = 0;
  lastButtonState = 0;

  pinMode(buttonPin, INPUT);
}

void Button::process()
{
  lastButtonState = buttonState;
  buttonState = digitalRead(buttonPin);

  if (lastButtonState == LOW && buttonState == HIGH) {
    pressCallback(buttonNum);
    delay(5);
  }
  if (lastButtonState == HIGH && buttonState == LOW) {
    releaseCallback(buttonNum);
    delay(5);
  }
}

void Button::pressHandler(void (*f)(int))    //button down
{
  pressCallback = *f;
}

void Button::releaseHandler(void (*f)(int))  //button release
{
  releaseCallback = *f;
}
