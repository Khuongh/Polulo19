//#include <Zumo32U4.h>
#include <EEPROM.h>
#include "Zumo32U4Dummy.h"
#include "math.h"
//#include "wire.h"
Zumo32U4ButtonA buttonA;
Zumo32U4ButtonB buttonB;
Zumo32U4Motors motors;
Zumo32U4LCD lcd;
Zumo32U4Encoders encoders(&motors);  
 
unsigned long last_time = 0;
unsigned long last_ten_sec = 0;
unsigned long last_minute = 0;
unsigned long one_second = 1000;
unsigned long ten_seconds = 10000; 
unsigned long one_minute = 60000;

int left_speed = 400; 
int right_speed = 400;

int account_balance = EEPROM.read(0); 
float batteryLevel = EEPROM.read(1);
int charging_cycles = EEPROM.read(2); 
int battery_health = EEPROM.read(3);

float motorSpeed = (left_speed + right_speed) / 2;
//float batteryLevel = 100;
 
float meters = 0;
float meters_per_second = 0;
float average_speed = 0;
float meters_per_minute = 0;

float maxSpeed = 0; 

void setup() 
{  
  Serial.begin(9600);
  motors.setSpeeds(left_speed, right_speed);
}
void loop() 
{
  delay(10);

  if((millis()-last_time) >= one_second)
  {
    int leftCount = encoders.getCountsAndResetLeft(); 
    int rightCount = encoders.getCountsAndResetRight();
    float averageCount = (leftCount+rightCount)/2;
    float this_meter = averageCount/8000.0;
    meters += this_meter;                                   //teller antall meter kjørt
    unsigned long myTime = millis()-last_time;              //får tiden når programmet går inn i if funksjonen
    meters_per_second = (this_meter/myTime)*1000;           //regner ut meter per sekund 
    if(maxSpeed < meters_per_second) 
    {                      //lagrer maks hastighet til en variabel 
      maxSpeed = meters_per_second;
    }
   lcd.clear();
   lcd.gotoXY(0,0);
   lcd.write(String(meters) + "m"); 
   lcd.gotoXY(0,1);
   lcd.write(String(meters_per_second) + "m/s"); 
   lcd.display();
  
   last_time = millis(); 
  
//BatteryLevelCode: 
   if((millis()-last_ten_sec) >= ten_seconds)               //Kode som regner ut og viser batterinivå hvert tiende sekund
   {
    batteryLevel = batteryCalc(batteryLevel, motorSpeed);
    EEPROM.update(1, batteryLevel); 
    
    lcd.clear();
    lcd.gotoXY(0,0);
    lcd.write(String(batteryLevel) + "%"); 
    lcd.gotoXY(0, 1); 
    lcd.write("C:" + String(charging_cycles) + " " + String(battery_health));
    lcd.display();
    
  
    last_ten_sec = millis();
    
//End of batteryLevelCode.
      
  if((millis()-last_minute) >= one_minute)                    // if funksjon som går inn hvert minutt
  {
    unsigned long my_time = millis()-last_minute;
  
    meters_per_minute = meters;
  
    average_speed = (meters/my_time)*1000;                    // regner ut gjennomsnittshastighet i m/s
    average_speed = (meters_per_minute/my_time)*1000;         // regner ut gjennomsnittshastighet i m/s
  
    lcd.clear();
    lcd.gotoXY(0,0);
    lcd.write("M:" + String(maxSpeed) + "m/s");               // printer ut maks hastighet hvert minutt
    lcd.gotoXY(0,1);
    lcd.write("A:" + String(average_speed) + "m/s");          // printer ut gjennomsnittshastighet hvert minutt 
    lcd.display();
  
    meters_per_minute = 0;
    maxSpeed = 0;
    last_minute = millis();  
    }
   }
  }  
if (batteryLevel > 10) 
  { 

 ledYellow(0);
 ledRed(0);
 ledGreen(1);
  }

else if (batteryLevel > 5)
  {
 ledYellow(1);
 ledRed(0);
 ledGreen(0);
  }

else if (batteryLevel > 0) 
  { 
 ledYellow(0);
 ledRed(1);
 ledGreen(0);
 
  }
else if (batteryLevel == 0 )
  { 
    motors.setSpeeds(0 , 0); 
 ledYellow(0);
 ledRed(0);
 ledGreen(0);
  }
  
   if (buttonA.getSingleDebouncedPress())                   //lademodus
   { 
    while(buttonB.getSingleDebouncedPress() == false) 
    {
      if (account_balance > 0) 
      { 
        lcd.clear();
        lcd.gotoXY(0,0);
        lcd.write("A-1kr+10"); 
        lcd.gotoXY(0,1);
        lcd.write("% B)Exit"); 
        lcd.display();
        delay(2000);
        if (buttonA.getSingleDebouncedPress() 
        { 
          batteryLevel += 10; 
          account_balance -= 1; 
          charging_cycles += 1; 
          EEPROM.update(1, batteryLevel); 
          EEPROM.update(0, account_balance);
          EEPROM.update(2, charging_cycles);
  
          lcd.clear();
          lcd.gotoXY(0,0);
          lcd.write(String(batteryLevel) + "%");
          lcd.gotoXY(4,0);
          lcd.write(String(charging_cost)+"kr");  
          lcd.gotoXY(0,1);
          lcd.write(String(account_balance) + "kr"); 
          lcd.display();
          delay(5000); 
        }
      }
      else
      { 
      lcd.clear();
      lcd.gotoXY(0,0);
      lcd.write("Konto"); 
      lcd.gotoXY(0,1);
      lcd.write("er tom"); 
      lcd.display();
      delay(2000); 
      }
    }
  }
  
  if (buttonB.getSingleDebouncedPress())                    //Pengemodus
  {
    while (buttonB.getSingleDebouncedPress() == false) 
    { 
      lcd.clear();
      lcd.gotoXY(0,0);
      lcd.write("A)" + String(account_balance) + "+5"); 
      lcd.gotoXY(0,1);
      lcd.write("B) Exit"); 
      lcd.display();

      if (buttonA.getSingleDebouncedPress())
      {
        account_balance += 5; 
        EEPROM.update(0,account_balance);
      }
    }
  }
  
  if (battery_health <= 25) 
  { 
    motor.setSpeeds(0, 0); 
    lcd.clear();
    lcd.gotoXY(0,0);
    lcd.write("A) Bytt"); 
    lcd.gotoXY(0,1);
    lcd.write(" batteri"); 
    lcd.display(); 

    if (buttonA.getSingleDebouncedPress()) 
    { 
      battery_health = 100; 
      account_balance -= 10; 
       
    }
  }
  if else ( battery_health <= 50)
  { 
    
  }
  
  if (charging_cycles % 5 == 0) 
  { 
    battery_health -= 15;
    EEPROM.update(3, battery_health); 
  }
}

int batteryCalc(float batteryLevel, float motorSpeed)             //Funksjon som regner ut batterinivå som funkson av hastighet og distanse kjørt
{ 
  const int batteryDrain = 10;
  const int topSpeed = 400; 

  float actBatteryDrain = (motorSpeed / topSpeed)* batteryDrain; 

  if (fmod(meters, 1.0) >= 0.0 && fmod(meters, 1.0) < 0.5) 
  { 
    if (batteryLevel > 0) 
    { 
      batteryLevel -= actBatteryDrain; 
      if (batteryLevel <= 0) 
      { 
        batteryLevel = 0; 
      }
    }
  }

  return batteryLevel;
}
