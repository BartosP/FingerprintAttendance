#include <ESP8266WiFi.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <SoftwareSerial.h>
#include <Adafruit_Fingerprint.h>
#include <LiquidCrystal_I2C.h>

SoftwareSerial SENSOR(5, 4);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&SENSOR);
LiquidCrystal_I2C lcd(0x27, 16, 2);

IPAddress server_addr(10,0,0,33);
char ssid[] = "bary";
char pass[] = "22042001";
char user[] = "User";
char password[] = "Password";

char INSERT_DATA[] = "INSERT INTO attendance.fingerprints (jmeno, prijmeni, otisk) VALUES ('%s', '%s', '%s')";
char query[1500];
WiFiClient client;
MySQL_Connection conn(&client);

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
  finger.begin(57600);
  if (!finger.verifyPassword()){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Nenalezen senzor");
    delay(2000);
  }
}

String getName(){
  String input;
  while (!Serial.available()) {}
  while (Serial.available()) {
    input = Serial.readStringUntil('\n');
    return input;
  }
  return "Error getting name/surname";
}

String downloadFingerpintTemplate(uint16_t id){
  uint8_t p = finger.loadModel(id);
  switch (p){
    case FINGERPRINT_OK:
      break;
    default:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error 7");
      delay(500);
      return "Error getting data";
  }
  p = finger.getModel();
  switch (p) {
    case FINGERPRINT_OK:
      break;
   default:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error 8");
      delay(500);
      return "Error getting data";
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
  Serial.println("");
  Serial.println("Zadejte jmeno:");
  String jmeno = getName();
  Serial.println("Zadejte prijmeni:");
  String prijmeni = getName();
  String otisk = downloadFingerpintTemplate(id);
  char jmenoConv[jmeno.length()];
  char prijmeniConv[prijmeni.length()];
  char otiskConv[otisk.length()];
  jmeno.toCharArray(jmenoConv, jmeno.length() + 1);
  prijmeni.toCharArray(prijmeniConv, prijmeni.length() + 1);
  otisk.toCharArray(otiskConv, otisk.length() + 1);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Pripojuji se k");
  lcd.setCursor(0, 1);
  lcd.print("SQL...");
  delay(1000);
  if (conn.connect(server_addr, 3306, user, password)) {
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
    sprintf(query, INSERT_DATA, jmenoConv, prijmeniConv, otiskConv);
    cur_mem->execute(query);
    delete cur_mem;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Data uspesne");
    lcd.setCursor(0, 1);
    lcd.print("ulozena");
    delay(2000);
  }
  else{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Nepodarilo se");
    lcd.setCursor(0, 1);
    lcd.print("ulozit data");
    delay(2000);
  }
  conn.close();
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
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Cekani na");
  lcd.setCursor(0, 1);
  lcd.print("udaje...");
  sendToDB(id);
}

void loop(){
  addFinger();
}