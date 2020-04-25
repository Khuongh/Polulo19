#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4ProximitySensors proxSensors;
Zumo32U4Motors motors;

void setup() {
  // put your setup code here, to run once:
  proxSensors.initThreeSensors();
  

}

void loop() {
  // put your main code here, to run repeatedly:
  proxSensors.read();
  int left = proxSensors.countsLeftWithLeftLeds();
  int cent_left = proxSensors.countsFrontWithLeftLeds();
  int cent_right = proxSensors.countsFrontWithRightLeds();
  int right = proxSensors.countsRightWithLeftLeds();

  lcd.gotoXY(0,1);
  lcd.print(left);
  lcd.print(" ");
  lcd.print(cent_left);
  lcd.print(" ");
  lcd.print(cent_right);
  lcd.print(" ");
  lcd.print(right);

  follower(left, cent_left, cent_right, right);

  delay(50);
  
}

void follower(int myLeft, int myCentLeft, int myCentRight, int myRight ){

  if ( myLeft > myCentLeft && myLeft > myCentRight && myLeft > myRight){
    motors.setSpeeds(0, 150);
  }
  else if ( myRight > myCentLeft && myRight > myCentRight && myRight > myLeft){
    motors.setSpeeds(150, 0);
  }
    if ( ((myCentLeft + myCentRight)/2) > myRight && ((myCentLeft + myCentRight)/2) > myLeft){
    motors.setSpeeds(150, 150);
  }
    
}
