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

void printLC(String first, String second){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(first);
  lcd.setCursor(0, 1);
  lcd.print(second);
  delay(1500);
}

void initWifi(){
  WiFi.begin(ssid, pass);
  printLC("Pripojuji se k", "WiFi");
  while (WiFi.status() != WL_CONNECTED)
    delay(100);
  printLC("Pripojeno k", ssid);
}

void initFinger(){
  if (!finger.verifyPassword())
    printLC("Nenalezen sensor", "");
}

String downloadFingerpintTemplate(uint16_t id){
  uint8_t p = finger.loadModel(id);
  switch (p){
    case FINGERPRINT_OK:
      break;
    default:
      return "Error";
  }
  p = finger.getModel();
  switch (p) {
    case FINGERPRINT_OK:
      break;
   default:
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

void mqttpublish(uint16_t id){
  String otisk = downloadFingerpintTemplate(id);
  char otiskConv[otisk.length()];
  otisk.toCharArray(otiskConv, otisk.length() + 1);
  while (!client.connected()) {
    printLC("Pripojuji se k", "MQTT...");
    if (client.connect("espClient")) {
      client.publish("fingerprint", otiskConv);
      printLC("Data uspesne", "odeslana");
    }
    else
      printLC("Nepodarilo se", "pripojit");
  }
}

void addFinger(){
  int id = 1;
  uint8_t p = -1;
  printLC("Prilozte otisk", "pro pridani");
  while (p != FINGERPRINT_OK){
    p = finger.getImage();
    switch (p){
      case FINGERPRINT_OK:
        break;
      case FINGERPRINT_NOFINGER:
        break;
      default:
        return;
    }
  }
  p = finger.image2Tz(1);
  switch (p){
    case FINGERPRINT_OK:
      break;
    default:
      printLC("Error 1", "");
      return;
  }
  printLC("Odeberte prst", "");
  p = 0;
  while(p != FINGERPRINT_NOFINGER)
    p = finger.getImage();
  p = -1;
  printLC("Prilozte otisk", "znovu");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p){
      case FINGERPRINT_OK:
        break;
      case FINGERPRINT_NOFINGER:
        break;
      default:
        printLC("Error 2", "");
        return;
    }
  }
  p = finger.image2Tz(2);
  switch (p){
    case FINGERPRINT_OK:
      break;
    default:
      printLC("Error 3", "");
      return;
  }
  p = finger.createModel();
  if(p == FINGERPRINT_OK){}
  else if(p == FINGERPRINT_ENROLLMISMATCH){
    printLC("Otisky se", "neshoduji");
    return;
  }
  else{
    printLC("Error 4", "");
    return;
  }
  p = finger.storeModel(id);
  if (p != FINGERPRINT_OK){
    printLC("Error 5", "");
    return;
  }
  mqttpublish(id);
}

void setup(){
  Serial.begin(9600);
  finger.begin(57600);
  Wire.begin(2, 0);
  client.setServer(server, 1883);
  lcd.init();
  lcd.backlight();
  initWifi();
  initFinger();
}

void loop(){
  addFinger();
}