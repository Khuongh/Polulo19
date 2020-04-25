#include <Zumo32U4.h>

Zumo32U4LCD lcd;
Zumo32U4ProximitySensors proxSensors;
Zumo32U4Motors motors;
Zumo32U4ButtonA buttonA;

int lastDir = 0;
unsigned long followTime = 10000;
unsigned long startTime;
bool followMe = true;
String myString = "Follow";

void setup() {
  // put your setup code here, to run once:
  proxSensors.initThreeSensors();
  lcd.gotoXY(0,0);
  lcd.print("Press A");
  lcd.gotoXY(0,1);
  lcd.print("to start");
  buttonA.waitForPress();
  startTime = millis();
  

}

void loop() {
  // put your main code here, to run repeatedly:
  proxSensors.read();
 // int left = proxSensors.countsLeftWithLeftLeds();
  int cent_left = proxSensors.countsFrontWithLeftLeds();
  int cent_right = proxSensors.countsFrontWithRightLeds();
 // int right = proxSensors.countsRightWithLeftLeds();

  if ( (millis()-startTime) >= followTime){
    if( followMe){
    followMe = false;
    myString = "Turn";
    }
    else{
      followMe = true;
      myString = "Follow";
    }
    startTime = millis();   
  }
  
  lcd.clear();
  lcd.gotoXY(0,0);
  lcd.print(myString);
  lcd.gotoXY(0,1);
//lcd.print(left);
  lcd.print(" ");
  lcd.print(cent_left);
  lcd.print(" ");
  lcd.print(cent_right);
  lcd.print(" ");
//lcd.print(right);

  if ( followMe) follower(cent_left, cent_right);
  else if( !followMe ) turner(cent_left, cent_right); 

  delay(50);
  
}

void follower(int myCentLeft, int myCentRight){

  if ( myCentLeft > myCentRight && lastDir != 1){
    motors.setSpeeds(0,0);
    delay(20);
    motors.setSpeeds(50, 150);
    lastDir = 1;
  }
  
  else if ( myCentLeft > myCentRight){
    motors.setSpeeds(50, 150);
    lastDir = 1;
  }
  else if ( myCentRight > myCentLeft && lastDir != 2){
    motors.setSpeeds(0,0);
    delay(20);
    motors.setSpeeds(150, 50);
    lastDir = 2;
  }
  else if (myCentRight > myCentLeft){
    motors.setSpeeds(150, 50);
    lastDir = 2;
  }
  else if ( myCentRight == myCentLeft && lastDir != 0){
    motors.setSpeeds(0,0);
    delay(20);
    motors.setSpeeds(100,100);
    lastDir = 0;
  }
  else if ( myCentRight == myCentLeft){
    motors.setSpeeds(100, 100);
    lastDir = 0;
  }
}

void turner(int myCentLeft, int myCentRight){
  if ( myCentLeft > myCentRight && lastDir != 1){
    motors.setSpeeds(0,0);
    delay(20);
    motors.setSpeeds(-150, 150);
    lastDir = 1;
  }
  
  else if ( myCentLeft > myCentRight){
    motors.setSpeeds(-150, 150);
    lastDir = 1;
  }
  else if ( myCentRight > myCentLeft && lastDir != 2){
    motors.setSpeeds(0,0);
    delay(20);
    motors.setSpeeds(150, -150);
    lastDir = 2;
  }
  else if (myCentRight > myCentLeft){
    motors.setSpeeds(150, -150);
    lastDir = 2;
  }
  else{
    motors.setSpeeds(0,0);
  }
  
}

    
