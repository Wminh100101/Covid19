#include <SoftwareSerial.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <PN532_I2C.h>
  #include <PN532.h>
  #include <NfcAdapter.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);
SoftwareSerial SWSerial( 5, 4 ); // RX, TX
SoftwareSerial HCSerial( 3, 2  );
PN532_I2C pn532i2c(Wire);
  PN532 nfc(pn532i2c);

int red = A1;
int green = A0;
int buzzer = 10;

void mot_mui() {

  digitalWrite(red, HIGH);
  digitalWrite(green, LOW);
  digitalWrite(buzzer, LOW);
  delay(100);
  digitalWrite(buzzer, HIGH);
  delay(5000);
  digitalWrite(red, LOW);

}

void hai_mui() {

  digitalWrite(green, HIGH);
  digitalWrite(buzzer, LOW);
  delay(100);
  digitalWrite(buzzer, HIGH);
  delay(5000);
  digitalWrite(green, LOW);

}
void khong_mui() {
  digitalWrite(buzzer,LOW);
  for(int i = 1;i<=8;i++){

  digitalWrite(red,HIGH);
  delay(100);
  digitalWrite(red,LOW);
  delay(100); 
  
  }

  digitalWrite(buzzer, HIGH);


}

void mui_mot(String ten,String date){
  lcd.clear();
  lcd.setCursor(0, 0);
    lcd.print("Name:");
    lcd.print(ten);

    lcd.setCursor(0, 1);
    lcd.print("Type:");
    lcd.print("1");

    lcd.setCursor(0, 2);
    lcd.print("Num 1th");
    lcd.print(date);

    mot_mui();
    waiting();
}
void mui_hai(String ten,String date1, String date2){
  lcd.clear();
  lcd.setCursor(0, 0);
    lcd.print("Name:");
    lcd.print(ten);

    lcd.setCursor(0, 1);
    lcd.print("Type:");
    lcd.print("2");

    lcd.setCursor(0, 2);
    lcd.print("Num 1th:");//
    lcd.print(date1);
  lcd.setCursor(0, 3);
    lcd.print("Num 2th:");
    lcd.print(date2);
    hai_mui();
    waiting();
}
void mui_khong(String ten){
  lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Name:");
    lcd.print(ten);

    lcd.setCursor(0, 1);
    lcd.print("Type:");
    lcd.print("0");

    khong_mui();
    delay(3000);
    waiting();
  
}
 
void xuat_lcd(String uid) {

  const String name1 = "Nguyen Tan Truong";
  const String name2 = "Tran Thi My Linh";
  const String name3 = "Vo Van Tung";
  
 
  if(uid == "202217248165"){
    mui_hai(name1,"23/10/21","03/12/2021");//code tiêm 2 mũi, có cú pháp là mui_hai(tên,ngày chích mũi 1,ngày chính mũi 2);
  }
  else if(uid == "18824524247"){
    mui_mot(name2,"18/10/2021");//code tiêm 1 mũi, có cú pháp là mui_mot(tên,ngày chích mũi 1);
  }
  else if(uid == "262114921"){
    mui_khong(name3);//code tiêm 0 mũi, có cú pháp là mui_khong(tên);
  }
  else if(uid == "436162234120114128"){
    mui_hai("Nhat Minh","25/10/2021","25/11/2021");
  }

void waiting(){
  lcd.clear();
  lcd.setCursor(2, 1);
  lcd.print("Cham the ben duoi");
  lcd.setCursor(2, 2);
  lcd.print("");
}

void setup() {

  Serial.begin(115200);
  HCSerial.begin(9600);
  nfc.begin();
  lcd.init();
  lcd.backlight();

  uint32_t versiondata = nfc.getFirmwareVersion();
  
  if (! versiondata) {

    Serial.print("Didn't Find PN53x Module");

    while (1); // Halt

  }

  // Got valid data, print it out!

  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX);

  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC);

  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);

  

  nfc.SAMConfig();
 
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
  digitalWrite(buzzer, HIGH);

  waiting();
  
}

void loop() {
  
  boolean success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  
  String str1;
  String str2;
  uint8_t uidLength;                       
  int check = 1;

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);

  while (!success)
  {
    delay(100);
    success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
  }
  for (uint8_t i = 0; i < uidLength; i++)
  {
    str1 = String(uid[i]);
    str2 += str1;
  }

  Serial.println(str2);
  HCSerial.print(str2);
  xuat_lcd(str2);
  

  delay(500);
}
