#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4ProximitySensors proxSensors;
Zumo32U4Motors motors;
Zumo32U4ButtonA buttonA;

void setup() {
  // put your setup code here, to run once:
  proxSensors.initThreeSensors();
  lcd.print("Press A");
  buttonA.waitForPress();
  

}

void loop() {
  // put your main code here, to run repeatedly:
  proxSensors.read();
 // int left = proxSensors.countsLeftWithLeftLeds();
  int cent_left = proxSensors.countsFrontWithLeftLeds();
  int cent_right = proxSensors.countsFrontWithRightLeds();
 // int right = proxSensors.countsRightWithLeftLeds();

  lcd.gotoXY(0,1);
//lcd.print(left);
  lcd.print(" ");
  lcd.print(cent_left);
  lcd.print(" ");
  lcd.print(cent_right);
  lcd.print(" ");
//lcd.print(right);

  follower(cent_left, cent_right);
  
}

void follower(int myCentLeft, int myCentRight){

  if ( myCentLeft > myCentRight){
    motors.setSpeeds(-100, 100);
  }
  else if (myCentRight > myCentLeft){
    motors.setSpeeds(100, -100);
  }
 else {
    motors.setSpeeds(0,0);
    delay(10);
    motors.setSpeeds(100, 100);
  }
    
}
