#include <ESP8266WiFi.h> 
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> 
#include <Adafruit_Fingerprint.h>
#include <PubSubClient.h>
#include <LiquidCrystal_I2C.h>

SoftwareSerial SENSOR(5, 4);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&SENSOR);
LiquidCrystal_I2C lcd(0x27, 16, 2);

IPAddress server(10,0,0,41);
char ssid[] = "bary";
char pass[] = "22042001";
WiFiManager wifiManager;

WiFiClient espClient;
PubSubClient client(espClient);

void printLC(String first, String second, int dl){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(first);
  lcd.setCursor(0, 1);
  lcd.print(second);
  delay(dl);
}

void initWifi(){
  wifiManager.setTimeout(20);
  printLC("Pripojuji se k", "WiFi", 2000);
  while(WiFi.status() != WL_CONNECTED){
    if(!wifiManager.autoConnect("AutoconnectAP")){
      printLC("Nepdarilo se", "pripojit k WiFi", 2000);
      printLC("Pripojte se na", "AutoconnectAP", 10);
    }
  }
  printLC("Pripojeno k", "WiFi", 2000);
}


void initFinger(){
  if (!finger.verifyPassword())
    printLC("Nenalezen sensor", "", 2000);
}

String getName(){
  String input;
  while (!Serial.available()) {}
  while (Serial.available()) {
    input = Serial.readStringUntil('\n');
    input += "//";
    return input;
  }
  return "Error";
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
      otisk += String(SENSOR.read(), HEX);
      otisk += ",";
      i++;
    }
  }
  return otisk;
}

void mqttpublish(uint16_t id){
  printLC("Zadejte udaje", "do terminalu", 200);
  Serial.println("Zadejte jmeno:");
  String message = getName();
  Serial.println("Zadejte prijmeni:");
  message += getName();
  message += downloadFingerpintTemplate(id);
  char messageConv[message.length()];
  message.toCharArray(messageConv, message.length() + 1);

  while (!client.connected()) {
    printLC("Pripojuji se k", "MQTT...", 1500);
    if (client.connect("espClient")) {
      client.publish("fingerprint", messageConv);
      printLC("Data uspesne", "odeslana", 1500);
    }
    else
      printLC("Nepodarilo se", "pripojit", 1500);
  }
}

void addFinger(){
  int id = 1;
  uint8_t p = -1;
  printLC("Prilozte otisk", "pro pridani", 10);
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
      printLC("Error 1", "", 1000);
      return;
  }
  printLC("Odeberte prst", "", 1500);
  p = 0;
  while(p != FINGERPRINT_NOFINGER)
    p = finger.getImage();
  p = -1;
  printLC("Prilozte otisk", "znovu", 10);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p){
      case FINGERPRINT_OK:
        break;
      case FINGERPRINT_NOFINGER:
        break;
      default:
        printLC("Error 2", "", 1000);
        return;
    }
  }
  p = finger.image2Tz(2);
  switch (p){
    case FINGERPRINT_OK:
      break;
    default:
      printLC("Error 3", "", 1000);
      return;
  }
  p = finger.createModel();
  if(p == FINGERPRINT_OK){}
  else if(p == FINGERPRINT_ENROLLMISMATCH){
    printLC("Otisky se", "neshoduji", 1500);
    return;
  }
  else{
    printLC("Error 4", "", 1000);
    return;
  }
  p = finger.storeModel(id);
  if (p != FINGERPRINT_OK){
    printLC("Error 5", "", 1000);
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