#ifndef Buttons_h
#define Buttons_h

#include "Arduino.h"

class Button
{
  public:
    Button(int _buttonPin, int _buttonNum);
    
    void process();
    void pressHandler(void (*f)(int));
    void releaseHandler(void (*f)(int));
    void (*pressCallback)(int);
    void (*releaseCallback)(int);

  private:
    int buttonPin;
    int buttonNum;
    bool buttonState;
    bool lastButtonState;
};

#endif
