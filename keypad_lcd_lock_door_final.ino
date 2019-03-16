#include <Keypad.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include<EEPROM.h>
//I2C pins declaration
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns



//for servo 

#include <Servo.h>    
 
Servo servo;
int angle = 0;    
//
int counter=0; 
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

char password[4];
char initial_password[4],new_password[4];
int i=0;
int relay_pin = 11;
char key_pressed=0;

//from star side **=keypad--8,arduino-->3,7-->2,6-->1->0,4-->7,3-->6,2-->5,keypad-1-->arduino-4
byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {9, 8, 7, 6}; //connect to the colum,5-n pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


void setup()

{
  Serial.begin(9600);
  servo.attach(10);
  pinMode(relay_pin, OUTPUT);
  pinMode(23, OUTPUT);
  lcd.begin(16,2);
  lcd.print("Team 360");
  lcd.setCursor(0,1);
  lcd.print("Welcome to IoT");
  delay(1000);
  lcd.clear();
  lcd.print("Enter Password:");
  lcd.setCursor(0,1);
  initialpassword();
}
void loop()
{
//digitalWrite(relay_pin, HIGH);
key_pressed = keypad.getKey();
if(key_pressed=='#')
change();
if (key_pressed)
{
password[i++]=key_pressed;
lcd.print(key_pressed);
}
if(i==4)
{
delay(200);
for(int j=0;j<4;j++)
initial_password[j]=EEPROM.read(j);
if(!(strncmp(password, initial_password,4)))
{
lcd.clear();
lcd.print("Pass Accepted");
digitalWrite(23,HIGH);
for(angle = 0; angle < 90; angle += 1)    // command to move from 0 degrees to 180 degrees 
  {                                  
    servo.write(angle);                 //command to rotate the servo to the specified angle
    delay(30);                       
  } 
 
  delay(1000);
  
  for(angle = 90; angle>=1; angle-=1)     // command to move from 180 degrees to 0 degrees 
  {                                
    servo.write(angle);              //command to rotate the servo to the specified angle
    delay(30);                       
  } 

lcd.setCursor(0,1);
lcd.print("Pres # to change");
delay(2000);
lcd.clear();
lcd.print("Enter Password:");
lcd.setCursor(0,1);
i=0;
}
else
{

  
 
digitalWrite(relay_pin, LOW);
lcd.clear();
lcd.print("Wrong Password");
lcd.setCursor(0,1);
lcd.print("Pres # to Change");
delay(2000);
lcd.clear();
lcd.print("Enter Password");
lcd.setCursor(0,1);
i=0;
 counter=counter+1;
    }
    Serial.println(counter);
    if(counter>=3){
      lcd.clear();
      Serial.println("alarm activated");
      lcd.print("alarm activated..");
      lcd.setCursor(0,0);
      delay(2000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("disabled System ..");
      

      for(int i=15;i>=0;i--){
        lcd.setCursor(0,1);
      lcd.print(i);
      if(i<=9){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("disabled System ..");
        lcd.setCursor(0,1);
        lcd.print(i);
        }
      delay(1000);
        }
      
      counter=0;
  
      
      
      }
    lcd.clear();
    lcd.print("Enter Password :");
    lcd.setCursor(0,1);
  }
}

void change()
{
 int j=0;
  lcd.clear();
  lcd.print("Current Password");
  lcd.setCursor(0,1);
  while(j<4)
  {
    char key=keypad.getKey();
    if(key)
    {
      new_password[j++]=key;
      lcd.print(key);
    }
    key=0;
  }
  delay(500);
  if((strncmp(new_password, initial_password, 4)))
  {
    lcd.clear();
 lcd.print("Wrong Password");
    lcd.setCursor(0,1);
    lcd.print("Try Again");
    delay(1000);
  }
  else
  {
    j=0;
    lcd.clear();
    lcd.print("New Password:");
    lcd.setCursor(0,1);
    while(j<4)
    {
      char key=keypad.getKey();
      if(key)
      {
        initial_password[j]=key;
        lcd.print(key);
        EEPROM.write(j,key);
        j++;
      }
    }
lcd.print("Pass Changed");
delay(1000);
  }
  lcd.clear();
  lcd.print("Enter Password");
  lcd.setCursor(0,1);
  key_pressed=0;
}
void initialpassword(){
  for(int j=0;j<4;j++)
    EEPROM.write(j, j+49);
  for(int j=0;j<4;j++)
    initial_password[j]=EEPROM.read(j);
}
