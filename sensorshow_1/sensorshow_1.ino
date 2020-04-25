// Example on how to read from the gyrometer
// built into the Pololu Zumo 32U4 Arduino-based robot.

#include <Zumo32U4.h>
#include "TurnSensor.h"

L3G gyro;
Zumo32U4LCD lcd;
Zumo32U4Motors motors;
Zumo32U4ButtonA buttonA;
Zumo32U4Buzzer buzzer;

// --- Helper functions
int32_t getAngle() {
  // turnAngle is a variable defined in TurnSensor.cpp
  // This fancy math converts the number into degrees turned since the
  // last sensor reset.
  return (((int32_t)turnAngle >> 16) * 360) >> 16;
}

int32_t angle = getAngle();


// --- Setup Method
void setup() {
  turnSensorSetup();
  delay(500);
  turnSensorReset();
  lcd.clear();
}

// --- Main program loop
void loop() {
  lcd.print("Press A");
  buttonA.waitForButton();
  for (int i=0; i<8; i++){
    ledRed(0);
    ledGreen(1);
    motors.setSpeeds(100,100);
    delay(1000);
    motors.setSpeeds(0,0);
    delay(20);
    turnSensorReset();
    turnSensorUpdate();
    angle = getAngle();

    while ( angle <= 40){
        motors.setSpeeds(-100,100);
        turnSensorUpdate();
        angle = getAngle();
        ledGreen(0);
        ledRed(1);

        if (angle >= 40){
          motors.setSpeeds(0,0);
          delay(20);
          buzzer.play(">g32>>c32");
          break;
        }
    }
  }
  


}
