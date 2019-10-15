#include "Adafruit_Fingerprint.h"
#include "LiquidCrystal_I2C.h"
#include "WiFiEsp.h"
#include "WiFiEspClient.h"
#include "MySQL_Connection.h"
#include "MySQL_Cursor.h"

#define NAME "bary"
#define PASS "22042001"
char MySQLUser[] = "root";
char MySQLPass[] = "";
IPAddress MySQLIP(10,0,0,36); 

LiquidCrystal_I2C lcd(0x3F, 16, 2);
SoftwareSerial ESP(5, 6);
SoftwareSerial SCANNER(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&SCANNER);
WiFiEspClient client;
MySQL_Connection conn(&client);
MySQL_Cursor* cursor;

char INSERT_DATA[] = "INSERT INTO attendance.fingerprints (jmeno, prijmeni, otisk) VALUES ('%s', '%s', '%s')";
char query[1200];
String name = "jmenotest";
String surname = "prijmenitest";
String otisk = "otisktest";
int zdar = 0;

void setup() {
  Serial.begin(9600);
  ESP.begin(9600);
  WiFi.init(&ESP);
  while (WiFi.status() != WL_CONNECTED) {
     WiFi.begin(NAME, PASS);
  }
  Serial.println("WiFi pripojena");
  //lcd.begin(16,2);
  //lcd.init();
  //lcd.backlight();
  finger.begin(57600);
  if (!finger.verifyPassword()){
    Serial.println("Nenalezen senzor");
    lcd.setCursor(0, 0);
    lcd.print("Nenalezen");
    lcd.setCursor(0, 1);
    lcd.print("senzor...");
    delay(2000);
  }
}

int selectID(){
  int id = 1;
  uint8_t p = -1;
  while(1){
    p = finger.loadModel(id);
    if(p == FINGERPRINT_OK)
      id++;
    else
      break;
  }
  return id;
}

bool espListen(){
  return 0;
}

void sendToDB(){
  ESP.begin(9600);
  while(!conn.connect(MySQLIP, 3306, MySQLUser, MySQLPass)){}
  Serial.println("test1");
  cursor = new MySQL_Cursor(&conn);
  finger.begin(57600);
}

uint8_t downloadFingerpintTemplate(uint16_t id){
  uint8_t p = finger.loadModel(id);
  switch (p){
    case FINGERPRINT_OK:
      break;
    default:
      Serial.print("Error");
      Serial.println(p);
      return p;
  }
  p = finger.getModel();
  switch (p) {
    case FINGERPRINT_OK:
      break;
   default:
      Serial.print("Error");
      Serial.println(p);
      return p;
  }
  uint8_t templateBuffer[534];
  memset(templateBuffer, 0xff, 534);
  uint32_t starttime = millis();
  int i = 0;
  while ((i < 534) && ((millis() - starttime) < 20000)){
    if (SCANNER.available()){
      templateBuffer[i] = SCANNER.read();
      i++;
    }
  }
  uint8_t fingerTemplate[512];
  memset(fingerTemplate, 0xff, 512);
  int j = 265;
  i = 9;
  while (i < 534) {
      while (i < j)
          fingerTemplate[i + 1] = templateBuffer[i];
      j += 2;
      while (i < j)
        i++;
      j = i + 9;
  }
  sendToDB();
  return p;
}

void addFinger(){
  int id = selectID();
  uint8_t p = -1;
  Serial.println("Prilozte otisk pro pridani");
  //lcd.clear();
  //lcd.setCursor(0, 0);
  //lcd.print("Prilozte otisk");
  //lcd.setCursor(0, 1);
  //lcd.print("pro pridani");
  while (p != FINGERPRINT_OK){
    p = finger.getImage();
    //if(!espListen)
      //return;
    switch (p){
      case FINGERPRINT_OK:
        break;
      case FINGERPRINT_NOFINGER:
        break;
      default:
        Serial.println("Error");
        //lcd.clear();
        //lcd.setCursor(0, 0);
        //lcd.print("Error");
        delay(1000);
        return;
    }
  }
  p = finger.image2Tz(1);
  switch (p){
    case FINGERPRINT_OK:
      break;
    default:
      Serial.println("Error");
      //lcd.clear();
      //lcd.setCursor(0, 0);
      //lcd.print("Error");
      delay(1000);
      return;
  }
  Serial.println("Odeberte prst");
  //lcd.clear();
  //lcd.setCursor(0, 0);
  //lcd.print("Odeberte prst");
  delay(2000);
  p = 0;
  while(p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  p = -1;
  Serial.println("Prilozte otisk znovu");
  //lcd.clear();
  //lcd.setCursor(0, 0);
  //lcd.print("Prilozte otisk");
  //lcd.setCursor(0, 1);
  //lcd.print("znovu");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    //if(!espListen)
      //return;
    switch (p){
      case FINGERPRINT_OK:
        break;
      case FINGERPRINT_NOFINGER:
        break;
      default:
        Serial.println("Error");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Error");
        delay(1000);
        return;
    }
  }
  p = finger.image2Tz(2);
  switch (p){
    case FINGERPRINT_OK:
      break;
    default:
      Serial.println("Error");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error");
      delay(1000);
      return;
  }
  p = finger.createModel();
  if(p == FINGERPRINT_OK){}
  else if(p == FINGERPRINT_ENROLLMISMATCH){
    Serial.println("Otisky se neshoduji");
    //lcd.clear();
    //lcd.setCursor(0, 0);
    //lcd.print("Otisky se");
    //lcd.setCursor(0, 1);
    //lcd.print("neshoduji");
    delay(1000);
    return;
  }
  else{
    Serial.println("Error");
    //lcd.clear();
    //lcd.setCursor(0, 0);
    //lcd.print("Error");
    delay(1000);
    return;
  } 
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {}
  else{
    Serial.println("Error");
    //lcd.clear();
    //lcd.setCursor(0, 0);
    //lcd.print("Error");
    delay(1000);
    return;
  }
  //downloadFingerpintTemplate(id);
  delay(3000);
}

void loop() {
  //lcd.clear();
  //lcd.setCursor(0, 0);
  //lcd.print("Prilozte otisk");
  //lcd.setCursor(0, 1);
  //lcd.print("pro overeni");
  //delay(3000);
  //if(espListen)
  //addFinger();
  if(zdar == 0){
    sendToDB();
    zdar++;
  }
}