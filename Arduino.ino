#include <SoftwareSerial.h>
#include <LiquidCrystal.h>          //Knihovna pro LCD
#include <Adafruit_Fingerprint.h>   //Knihovna pro scanner

#define NAME "bary"             //Wifi SSID
#define PASS "22042001"         //Wifi heslo

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
SoftwareSerial ESP(6, 7);
SoftwareSerial SCANNER(8, 9);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&SCANNER);

uint8_t id;
byte buttons[] = {A3,A4,A5};
String menu[] = {"Pridat otisk", "Overit otisk", "Smazat otisk"};     //Položky v menu
int menuItems = 3;   //Počet položek v menu
int cursorPos = 0;   //Pozice kurzoru
int menuPos = 0;     //Vybraná položka v menu
int checkConnect = 0;

void setup(){
  Serial.begin(9600); 
  ESP.begin(9600);
  lcd.begin(16,2);
  finger.begin(57600);

  //Ověření scanneru
  if (finger.verifyPassword()){
    lcd.setCursor(0, 0);
    lcd.print("Scanner found...");
  }
  else{
    lcd.setCursor(0, 0);
    lcd.print("Scanner not");
    lcd.setCursor(0, 1);
    lcd.print("found...");
  }
  delay(1000);
  
  ESP.println("AT+RST");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connecting");
  lcd.setCursor(0, 1);
  lcd.print("to WiFi");
  //Připojení k wifi
 /* while(1){
    ESP.print("AT+CWJAP=\"");
    ESP.print(NAME);
    ESP.print("\",\"");
    ESP.print(PASS);
    ESP.print("\"\r\n");
    ESP.setTimeout(5000);
    //Pokud se podaří připojit, cyklus se ukončí
    if(ESP.find("WIFI CONNECTED\r\n") == 1){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Pripojeno k WiFi");
      lcd.setCursor(0, 1);
      lcd.print(NAME);
      delay(5000);
      break;
    }
    //Pokud se nepodaří připojit na 3. pokus, vypíše se hláška a cyklus se ukončí
    checkConnect++;
    if(checkConnect > 3){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Nepodarilo se");
      lcd.setCursor(0, 1);
      lcd.print("pripojit k WiFi");
      delay(5000);
      break;
    }
  }*/

  //Načtení menu
  menuReset();
}

void menuReset(){
  //Vypíše první 2 položky v menu a nastaví kurzor na 1. položku
  lcd.clear();
  for(int i = 0; i < 2; i++){
    lcd.setCursor(2, i);
    lcd.print(menu[i]);
  }
  lcd.setCursor(0, 0);
  lcd.print(">");
  cursorPos = 0;
  delay(500);
}

void selection(){
  //Výběr v menu, 1. tlačítko slouží k posouvání
  if(digitalRead(buttons[0]) == 1){
    menuPos++;
    if(menuPos == 0)
      cursorPos = 0;
    else
      cursorPos = 1;
    //Když dojdeme na konec menu, posune kurzor zpět na začátek
    if(menuPos > menuItems - 1){
      cursorPos = 0;
      menuPos = 0;
    }
    //Posouvání v menu
    lcd.clear();
    for(int i = 0; i < 2; i++){
      lcd.setCursor(2, i);
      if(menuPos == 0)
        lcd.print(menu[i + menuPos]);
      else
        lcd.print(menu[i + menuPos - 1]);
    }
    //Vypsání kurzoru
    lcd.setCursor(0, cursorPos);
    lcd.print(">");
    delay(500);
  }
}

void loop(){
  //Výběr v menu
  selection();
  //Předpřipravené funkce
  if(digitalRead(buttons[1]) == 1){
    switch(menuPos){
      case 0:
        addFinger();
        break;
      case 1:
        checkFinger();
        break;
      case 2:
        deleteFinger();
        break;
    }
  }
}

void addFinger(){
  id = 10;
  int p = -1;
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
      case FINGERPRINT_PACKETRECIEVEERR:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Error");
        delay(1000);
        break;
      case FINGERPRINT_IMAGEFAIL:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Error");
        delay(1000);
        break;
      default:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Unknown error");
        delay(1000);
        break;
    }
  }
  delay(2000);
  p = finger.image2Tz(1);
  switch (p){
    case FINGERPRINT_OK:
      break;
    case FINGERPRINT_IMAGEMESS:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Nelze rozpoznat");
      lcd.setCursor(0, 1);
      lcd.print("otisk");
      delay(1000);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Chyba komunikace");
      delay(1000);
      return p;
    case FINGERPRINT_FEATUREFAIL:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Nelze rozpoznat");
      lcd.setCursor(0, 1);
      lcd.print("otisk");
      delay(1000);
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Nelze rozpoznat");
      lcd.setCursor(0, 1);
      lcd.print("otisk");
      delay(1000);
      return p;
    default:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Unknown error");
      return p;
  }
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Odeberte prst");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
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
     case FINGERPRINT_PACKETRECIEVEERR:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Error");
        delay(1000);
        break;
      case FINGERPRINT_IMAGEFAIL:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Error");
        delay(1000);
        break;
      default:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Unknown error 1");
        delay(1000);
        break;
    }
  }
  delay(2000);
  p = finger.image2Tz(2);
  switch (p){
    case FINGERPRINT_OK:
      break;
    case FINGERPRINT_IMAGEMESS:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Nelze rozpoznat");
      lcd.setCursor(0, 1);
      lcd.print("otisk");
      delay(1000);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Chyba komunikace");
      delay(1000);
      return p;
    case FINGERPRINT_FEATUREFAIL:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Nelze rozpoznat");
      lcd.setCursor(0, 1);
      lcd.print("otisk");
      delay(1000);
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Nelze rozpoznat");
      lcd.setCursor(0, 1);
      lcd.print("otisk");
      delay(1000);
      return p;
    default:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Unknown error");
      return p;
  }
  delay(2000);
  p = finger.createModel();
  if(p == FINGERPRINT_OK){}
  else if(p == FINGERPRINT_PACKETRECIEVEERR){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Chyba komunikace");
    delay(1000);
    return p;
  }
  else if(p == FINGERPRINT_ENROLLMISMATCH){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Otisky se");
    lcd.setCursor(0, 1);
    lcd.print("neshoduji");
    delay(1000);
    menuReset();
    return p;
  }
  else{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Unknown error");
    return p;
  } 
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Otisk uspesne");
    lcd.setCursor(0, 1);
    lcd.print("ulozen");
    delay(3000);
  }
  else if(p == FINGERPRINT_PACKETRECIEVEERR){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Chyba komunikace");
    return p;
  }
  else if(p == FINGERPRINT_BADLOCATION){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Chyba ukladani");
    return p;
  }
  else if (p == FINGERPRINT_FLASHERR){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Chyba ukladani");
    return p;
  }
  else{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Unknown error");
    return p;
  } 
  menuReset();
}
  
void checkFinger(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Prilozte otisk");
  lcd.setCursor(0, 1);
  lcd.print("pro overeni");
  while(digitalRead(buttons[2]) == 0){
    //Zde bude kód pro ověření existujícího otisku v databázi
  }
  menuReset();
}

void deleteFinger(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Smazat otisk");
  while(digitalRead(buttons[2]) == 0){
    //Zde bude kód pro smazání existujícího otisku v databázi
  }
  menuReset();
}


