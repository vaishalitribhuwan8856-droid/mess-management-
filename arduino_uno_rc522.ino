#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>
#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN);
// UNO D3 -> ESP8266 RX through voltage divider
SoftwareSerial espSerial(2, 3); // RX, TX
String lastUid=""; unsigned long lastAt=0;
void setup(){Serial.begin(9600);espSerial.begin(9600);SPI.begin();rfid.PCD_Init();Serial.println("RFID ready");}
void loop(){
 if(!rfid.PICC_IsNewCardPresent()||!rfid.PICC_ReadCardSerial()) return;
 String uid="";
 for(byte i=0;i<rfid.uid.size;i++){if(rfid.uid.uidByte[i]<0x10)uid+="0";uid+=String(rfid.uid.uidByte[i],HEX);}
 uid.toUpperCase();
 if(uid!=lastUid || millis()-lastAt>3000){espSerial.println("UID:"+uid);Serial.println("UID:"+uid);lastUid=uid;lastAt=millis();}
 rfid.PICC_HaltA();rfid.PCD_StopCrypto1();delay(500);
}
