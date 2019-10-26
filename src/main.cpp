#include "Adafruit_Fingerprint.h"
#include "LiquidCrystal_I2C.h"
#include "WiFiEsp.h"
#include "WiFiEspClient.h"

#define NAME "bary"
#define PASS "22042001"
#define SRV "10.0.0.33"

LiquidCrystal_I2C lcd(0x3F, 16, 2);
SoftwareSerial ESP(5, 6);
SoftwareSerial SCANNER(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&SCANNER);
WiFiEspClient client;
char name[] = "testjmeno";
char surname[] = "prijmenitest";
uint8_t templateBuffer[534] = {};

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
    //lcd.setCursor(0, 0);
    //lcd.print("Nenalezen");
    //lcd.setCursor(0, 1);
    //lcd.print("senzor...");
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
  return false;
}

bool sendToDB(){
  ESP.begin(9600);
  if (client.connect(SRV, 80)) {
    Serial.println("Probiha ukladani...");
    client.print("GET /ArduinoProjekt/fingerprint.php?otisk=");
    for(int i = 0; i < 534; i++)
      client.print(templateBuffer[i]);
    client.print(" HTTP/1.1");
    client.println();
    String host = "Host: ";
    host += SRV;
    client.println(host);
    client.println("Connection: close");
    client.println();
    finger.begin(57600);
    return true;
  }
  finger.begin(57600);
  return false;
}

uint8_t downloadFingerpintTemplate(uint16_t id, uint8_t *templateBuffer){
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
  memset(templateBuffer, 0xff, 534);
  uint32_t starttime = millis();
  int i = 0;
  while ((i < 534) && ((millis() - starttime) < 20000)){ 
    if (SCANNER.available()){
      templateBuffer[i] = SCANNER.read();
      i++;
    }
  }
  return *templateBuffer;
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
  if (p == FINGERPRINT_OK){
    downloadFingerpintTemplate(id, templateBuffer);
  }
  else{
    Serial.println("Error");
    //lcd.clear();
    //lcd.setCursor(0, 0);
    //lcd.print("Error");
    delay(1000);
    return;
  }
  if(sendToDB())
    Serial.println("Uspesne ulozeno");
  else
    Serial.println("Nepodarilo se ulozit otisk");
  p = finger.deleteModel(id);
  if (p == FINGERPRINT_OK){}
  else{
    Serial.println("Error deleting");
    //lcd.clear();
    //lcd.setCursor(0, 0);
    //lcd.print("Error deleting");
    delay(1000);
    return;
  }
}

void loop() {
  //lcd.clear();
  //lcd.setCursor(0, 0);
  //lcd.print("Prilozte otisk");
  //lcd.setCursor(0, 1);
  //lcd.print("pro overeni");
  //delay(3000);
  //if(espListen())
    addFinger();
}