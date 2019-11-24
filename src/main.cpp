#include <ESP8266WiFi.h>
#include <Adafruit_Fingerprint.h>
#include <PubSubClient.h>
#include <LiquidCrystal_I2C.h>

SoftwareSerial SENSOR(5, 4);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&SENSOR);
LiquidCrystal_I2C lcd(0x27, 16, 2);

IPAddress server(10,0,0,33);
char ssid[] = "bary";
char pass[] = "22042001";

WiFiClient espClient;
PubSubClient client(espClient);

void setup(){
  Serial.begin(9600);
  Wire.begin(2, 0);
  lcd.init();
  lcd.backlight();
  WiFi.begin(ssid, pass);
  delay(3000);
  lcd.print("Pripojuji se k");
  lcd.setCursor(0, 1);
  lcd.print("Wifi ");
  lcd.print(ssid);
  delay(2000);
  while (WiFi.status() != WL_CONNECTED)
    delay(100);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Pripojeno k");
  lcd.setCursor(0, 1);
  lcd.print("Wifi ");
  lcd.print(ssid);
  delay(1000);
  client.setServer(server, 1883);
  finger.begin(57600);
  if (!finger.verifyPassword()){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Nenalezen senzor");
    delay(2000);
  }
}

String downloadFingerpintTemplate(uint16_t id){
  uint8_t p = finger.loadModel(id);
  switch (p){
    case FINGERPRINT_OK:
      break;
    default:
      Serial.print("Error");
      return "Error";
  }
  p = finger.getModel();
  switch (p) {
    case FINGERPRINT_OK:
      break;
   default:
      Serial.print("Error");
      return "Error";
  }
  uint32_t starttime = millis();
  int i = 0;
  String otisk = "";
  while ((i < 534) && ((millis() - starttime) < 20000)){ 
    if (SENSOR.available()){
      otisk += SENSOR.read();
      i++;
    }
  }
  return otisk;
}

void sendToDB(uint16_t id){
  String otisk = downloadFingerpintTemplate(id);
  char otiskConv[otisk.length()];
  otisk.toCharArray(otiskConv, otisk.length() + 1);
  while (!client.connected()) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Pripojuji se k");
    lcd.setCursor(0, 1);
    lcd.print("MQTT...");
    delay(1000);
    if (client.connect("espClient")) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Pripojeno");
      client.publish("fingerprint", otiskConv);
      delay(2000);
    }
    else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Nepodarilo se");
      lcd.setCursor(0, 1);
      lcd.print("pripojit");
      delay(1500);
    }
  }
}

void addFinger(){
  int id = 1;
  uint8_t p = -1;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Prilozte otisk");
  lcd.setCursor(0, 1);
  lcd.print("pro pridani");
  while (p != FINGERPRINT_OK){
    p = finger.getImage();
    switch (p){
      case FINGERPRINT_OK:
        break;
      case FINGERPRINT_NOFINGER:
        break;
      default:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Error 1");
        return;
    }
  }
  p = finger.image2Tz(1);
  switch (p){
    case FINGERPRINT_OK:
      break;
    default:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error 2");
      delay(500);
      return;
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Odeberte prst");
  delay(2000);
  p = 0;
  while(p != FINGERPRINT_NOFINGER)
    p = finger.getImage();
  p = -1;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Prilozte otisk");
  lcd.setCursor(0, 1);
  lcd.print("znovu");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p){
      case FINGERPRINT_OK:
        break;
      case FINGERPRINT_NOFINGER:
        break;
      default:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Error 3");
        delay(500);
        return;
    }
  }
  p = finger.image2Tz(2);
  switch (p){
    case FINGERPRINT_OK:
      break;
    default:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error 4");
      delay(500);
      return;
  }
  p = finger.createModel();
  if(p == FINGERPRINT_OK){}
  else if(p == FINGERPRINT_ENROLLMISMATCH){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Otisky se");
    lcd.setCursor(0, 1);
    lcd.print("neshoduji");
    delay(500);
    return;
  }
  else{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Error 5");
    delay(500);
    return;
  }
  p = finger.storeModel(id);
  if (p != FINGERPRINT_OK){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Error 6");
    delay(500);
    return;
  }
  sendToDB(id);
}

void loop(){
  addFinger();
  delay(2000);
}