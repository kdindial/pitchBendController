#include "Button.h"
#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
VL53L0X_RangingMeasurementData_t measure;

Button buttonOne(32, 72);

int ratePot = A12; //pot that determines rate of change of the animation

int switchPin = 30;

int distance = 0;
int mappedDistance = 0;


unsigned long lastStepTime = 0;
int steplength = 2;
int currentStep = 0;

int currentPitchBend = 0;
int pitchBendToBe = 0;

int currentControlVal = 0;
int controlValToBe = 0;

int dx = 1000; //rate of change of animation



void setup() {
  Serial.begin(9600);
  buttonOne.pressHandler(onPress);
  buttonOne.releaseHandler(onRelease);
  pinMode(switchPin, INPUT);


  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
  Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1);
  }
  // power
  Serial.println(F("VL53L0X API Simple Ranging example\n\n"));
}

void loop() {

  buttonOne.process();//If the button is pressed it will play a midi note, this is just for testing and demoing the pitchbend.
  if (digitalRead(switchPin) == LOW) {
    pitchBendToBe = getPitchBend(); // get the current pitch bend
    dx = getRatePB();
    animatedPitchBend(); //animate the pitch bend to incremently step up to desired pitchbend value
  }

  if (digitalRead(switchPin) == HIGH) {
    controlValToBe = getControlVal();
    dx = getRateCC();
    animatedControlChange();


  }



}


void onPress(int buttonNumber) {
  usbMIDI.sendNoteOn(buttonNumber, 127, 1);
  Serial.print(buttonNumber);
  Serial.println(" pressed");
}

void onRelease(int buttonNumber) {
  usbMIDI.sendNoteOff(buttonNumber, 0, 1);
  Serial.print(buttonNumber);
  Serial.println(" pressed");
}

int getPitchBend() {
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4 and measure.RangeMilliMeter > 40 and measure.RangeMilliMeter < 300) {
    distance = measure.RangeMilliMeter;
    mappedDistance = map(distance, 5, 300, -8192, 8192);
    return mappedDistance;
  }
  else {
    return 0;
  }
}

int getControlVal() {
  lox.rangingTest(&measure, false);

  if (measure.RangeStatus != 4 and measure.RangeMilliMeter > 40 and measure.RangeMilliMeter < 300) {
    distance = measure.RangeMilliMeter;
    mappedDistance = map(distance, 40, 300, 0, 127);
    Serial.println(mappedDistance);
    return mappedDistance;

  }
  else {
    return 0;
  }
}





void animatedPitchBend() {
  if (millis() > lastStepTime + steplength){ //this means wait "steplength" amount of milliseconds before doing anything again


    lastStepTime = millis(); // update last step time

    if (pitchBendToBe != currentPitchBend) {

      if (pitchBendToBe > currentPitchBend) {
        currentPitchBend = currentPitchBend + dx; //if the pitchbend we want to be is bigger than the current pitchbend step up the current pitchbend
        usbMIDI.sendPitchBend(currentPitchBend, 1);
        Serial.println(currentPitchBend);
      }

      if (pitchBendToBe < currentPitchBend) {
        currentPitchBend = currentPitchBend - dx;//if the pitchbend we want to be is smaller than the current pitchbend step down the current pitchbend
        usbMIDI.sendPitchBend(currentPitchBend, 1);
        Serial.println(currentPitchBend);
      }
    }
  }
}

void animatedControlChange() {
  if (millis() > lastStepTime + steplength) {

    lastStepTime = millis(); // update last step time

    if (controlValToBe != currentControlVal) {

      if (controlValToBe > currentControlVal) {
        currentControlVal = currentControlVal + dx; //if the pitchbend we want to be is bigger than the current pitchbend step up the current pitchbend
        usbMIDI.sendControlChange(1, currentControlVal, 1);
        Serial.println(currentControlVal);
      }

      if (controlValToBe  < currentControlVal) {
        currentControlVal = currentControlVal - dx;//if the pitchbend we want to be is smaller than the current pitchbend step down the current pitchbend
        usbMIDI.sendControlChange(1, currentControlVal, 1);
        Serial.println(currentControlVal);
      }
    }
  }
}



int getRatePB() {
  int mappedval = map(analogRead(ratePot), 0, 1023, 200, 1000);// the rate has to be much faster for pitchbend
  return (mappedval);
}

int getRateCC() {
  int mappedval = map(analogRead(ratePot), 0, 1023, 1, 20);
  return (mappedval);
}
