#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <SoftwareSerial.h>
#include <Adafruit_Fingerprint.h>

SoftwareSerial SENSOR(5, 4);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&SENSOR);

IPAddress server_addr(10,0,0,33);
char ssid[] = "bary";
char pass[] = "22042001";

char user[] = "User";
char password[] = "Password";
char INSERT_DATA[] = "INSERT INTO attendance.fingerprints (jmeno, prijmeni, otisk) VALUES (%s, %s, %s)";
char query[3000];

WiFiClient client;
MySQL_Connection conn(&client);

void setup(){
  Serial.begin(9600);
  WiFi.begin(ssid, pass);
  delay(1000);
  Serial.println();
  Serial.print("Pripojuji se k Wifi ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED)
    delay(100);
  Serial.print("Pripojeno k WiFi ");
  Serial.println(ssid);
  finger.begin(57600);
  if (!finger.verifyPassword())
    Serial.println("Nenalezen senzor");
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

String getName(){
  String input;
  while (!Serial.available ()) {}
  if(Serial.available()){
    input = Serial.readStringUntil('\n');
    return input;
  }
  else
    return "Failed to get name/surname";
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
  uint8_t templateBuffer[534];
  memset(templateBuffer, 0xff, 534);
  uint32_t starttime = millis();
  int i = 0;
  while ((i < 534) && ((millis() - starttime) < 20000)){ 
    if (SENSOR.available()){
      templateBuffer[i] = SENSOR.read();
      i++;
    }
  }
  char buffer[3];
  String otisk;
  i = 0;
  while (i < 534){
    otisk += itoa(templateBuffer[i], buffer, 10);
    i++;
  }
  return otisk;
}

void sendToDB(uint16_t id){
  Serial.println("Zadejte jmeno:");
  String jmeno = getName();
  Serial.println("Zadejte prijmeni:");
  String prijmeni = getName();
  String otisk = downloadFingerpintTemplate(id);
  char jmenoConv[jmeno.length()];
  char prijmeniConv[prijmeni.length()];
  char otiskConv[otisk.length()];
  jmeno.toCharArray(jmenoConv, jmeno.length());
  prijmeni.toCharArray(prijmeniConv, prijmeni.length());
  otisk.toCharArray(otiskConv, otisk.length());
  Serial.println("Pripojuji se k SQL");
  if (conn.connect(server_addr, 3306, user, password)) {
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
    sprintf(query, INSERT_DATA, jmenoConv, prijmeniConv, otiskConv);
    cur_mem->execute(query);
    delete cur_mem;
    Serial.println("Data uspesne ulozena");
  }
  else
    Serial.println("Nepodarilo se pripojit");
  conn.close();
}

void addFinger(){
  int id = selectID();
  uint8_t p = -1;
  Serial.println("Prilozte otisk pro pridani");
  while (p != FINGERPRINT_OK){
    p = finger.getImage();
    switch (p){
      case FINGERPRINT_OK:
        break;
      case FINGERPRINT_NOFINGER:
        break;
      default:
        Serial.println("Error");
        return;
    }
  }
  p = finger.image2Tz(1);
  switch (p){
    case FINGERPRINT_OK:
      break;
    default:
      Serial.println("Error");
      return;
  }
  Serial.println("Odeberte prst");
  p = 0;
  while(p != FINGERPRINT_NOFINGER)
    p = finger.getImage();
  p = -1;
  Serial.println("Prilozte otisk znovu");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p){
      case FINGERPRINT_OK:
        break;
      case FINGERPRINT_NOFINGER:
        break;
      default:
        Serial.println("Error");
        return;
    }
  }
  p = finger.image2Tz(2);
  switch (p){
    case FINGERPRINT_OK:
      break;
    default:
      Serial.println("Error");
      return;
  }
  p = finger.createModel();
  if(p == FINGERPRINT_OK){}
  else if(p == FINGERPRINT_ENROLLMISMATCH){
    Serial.println("Otisky se neshoduji");
    return;
  }
  else{
    Serial.println("Error");
    return;
  }
  p = finger.storeModel(id);
  if (p != FINGERPRINT_OK){
    Serial.println("Error");
    return;
  }
  sendToDB(id);
  p = finger.deleteModel(id);
  if (p != FINGERPRINT_OK){
    Serial.println("Error deleting");
    return;
  }
}

void loop(){
  addFinger();
}