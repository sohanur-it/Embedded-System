
#include <Servo.h> 
Servo servo;  
 


#include "SPI.h"
#include "MFRC522.h"
/*Impport following Libraries*/
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
//I2C pins declaration
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

#define SS_PIN 10
#define RST_PIN 9
#define LED_PIN1 7 
#define buzzer 8
int servoPin = 3;
int servoAngle=0;

MFRC522 rfid(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  servo.attach(servoPin);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  lcd.begin(16,2);//Defining 16 columns and 2 rows of lcd display
lcd.backlight();//To Power ON the back light
//lcd.backlight();// To Power OFF the back light
  Serial.println("I am waiting for card...");
}

void loop() {

  //Write your code
lcd.setCursor(0,0); 
lcd.print("Group 360"); 
delay(1000);
  // put your main code here, to run repeatedly:
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;

  // Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  // Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }
  String strID = "";
  for (byte i = 0; i < 4; i++) {
    strID +=
      (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
      String(rfid.uid.uidByte[i], HEX) +
      (i != 3 ? ":" : "");
  }

  strID.toUpperCase();
  Serial.print("Tap card key: ");
  Serial.println(strID);
  delay(1000);

  if (strID.indexOf("10:9E:E5:A4") >= 0) {  //put your own tap card key;
    Serial.println("********************");
    Serial.println("**Authorised acces**");
    Serial.println("********************");
    digitalWrite(LED_PIN1,HIGH);
    delay(2000);
    digitalWrite(LED_PIN1,LOW);
    Serial.println("Door open");
    lcd.setCursor(0,0); 
    lcd.print("Door open"); 
   
//     for(servoAngle = 0; servoAngle < 90; servoAngle++)  //move the micro servo from 0 degrees to 180 degrees
//  {                                  
//    servo.write(servoAngle);              
//    delay(5);                  
//  }
//delay(5000);
//  for(servoAngle = 90; servoAngle > 0; servoAngle--)  //now move back the micro servo from 0 degrees to 180 degrees
//  {                                
//    servo.write(servoAngle);          
//    delay(5);      
// }

    return;
  }
  else {
    Serial.println("****************");
    Serial.println("**Acces denied**");
    Serial.println("****************");
    lcd.setCursor(0,0); 
    lcd.print("Accessed denied"); 
    delay(1000);
    digitalWrite(buzzer, HIGH);
    delay (2000);
    digitalWrite(8, LOW);
    lcd.clear();
    return;
  }

}
