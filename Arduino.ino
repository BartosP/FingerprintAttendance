#include <SoftwareSerial.h>
#include <LiquidCrystal.h>          //Knihovna pro LCD
#include <Adafruit_Fingerprint.h>   //Knihovna pro scanner
#include <WiFiEsp.h>

#define NAME "bary"                 //Wifi SSID
#define PASS "22042001"             //Wifi heslo

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
SoftwareSerial ESP(6, 7);
SoftwareSerial SCANNER(8, 9);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&SCANNER);

byte buttons[] = {A3,A4,A5};
String menu[] = {"Pridat otisk", "Overit otisk", "Smazat otisk"};     //Položky v menu
int menuItems = 3;   //Počet položek v menu
int cursorPos = 0;   //Pozice kurzoru
int menuPos = 0;     //Vybraná položka v menu
int checkConnect = 0;

void setup(){
  Serial.begin(9600);
  lcd.begin(16,2);
  
  //Připojení k wifi
  ESP.begin(9600);
  WiFi.init(&ESP);
  lcd.setCursor(0, 0);
  lcd.print("Pripojuji se k");
  lcd.setCursor(0, 1);
  lcd.print("siti ");
  lcd.print(NAME);
  while (WiFi.status() != WL_CONNECTED){
    lcd.print(".");
    WiFi.begin(NAME, PASS);
    checkConnect++;
    if(checkConnect > 2){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Nepodarilo se");
      lcd.setCursor(0, 1);
      lcd.print("pripojit k siti");
      delay(2000);
      break;
    }
  }
  if(WiFi.status() == WL_CONNECTED){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Pripojeno k siti");
    lcd.setCursor(0, 1);
    lcd.print(NAME);
    delay(2000);
  }

  //Ověření scanneru
  lcd.clear();
  finger.begin(57600);
  if (!finger.verifyPassword()){
    lcd.setCursor(0, 0);
    lcd.print("Nenalezen");
    lcd.setCursor(0, 1);
    lcd.print("senzor...");
    delay(2000);
  }
  
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
  menuPos = 0;
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

int selectID(){
  int id = 1;
  int p = -1;
  while(1){
    p = finger.loadModel(id);
    if(p == FINGERPRINT_OK)
      id++;
    else
      break;
  }
  return id;
}

int selectionDelete(){
  int id = 1;
  int i = 0;
  int p = finger.getTemplateCount();
  if(finger.templateCount - 1 == 0){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Nejsou ulozeny");
    lcd.setCursor(0, 1);
    lcd.print("zadne otisky");
    delay(2000);
    return 0;
  }
  int count = finger.templateCount - 1;
  int idArray[count];
  while(count > 0){
    p = finger.loadModel(id);
    if(p == FINGERPRINT_OK){
      idArray[i] = id;
      count--;
      i++;
      id++;
    }
    else
      id++;
  }
  lcd.clear();
  for(i = 0; i < 2; i++){
    if(finger.templateCount - 1 == 1){
      lcd.setCursor(0, 0);
      lcd.print(">");
      lcd.setCursor(2, 0);
      lcd.print(idArray[0]);
      delay(500);
      while(digitalRead(buttons[1]) == 0){
        if(digitalRead(buttons[2]) == 1)
          return 0;
      }
      return idArray[0];
    }
    else{
      lcd.setCursor(2, i);
      lcd.print(idArray[i]);
    }
  }
  lcd.setCursor(0, 0);
  lcd.print(">");
  cursorPos = 0;
  menuPos = 0;
  delay(500);
  while(digitalRead(buttons[1]) == 0){
    if(digitalRead(buttons[2]) == 1)
      return 0;
    if(digitalRead(buttons[0]) == 1){
      menuPos++;
      if(menuPos == 0)
        cursorPos = 0;
      else
        cursorPos = 1;
      if(menuPos > finger.templateCount - 2){
        cursorPos = 0;
        menuPos = 0;
      }
      lcd.clear();
      for(i = 0; i < 2; i++){
        lcd.setCursor(2, i);
        if(menuPos == 0)
          lcd.print(idArray[i + menuPos]);
        else
          lcd.print(idArray[i + menuPos - 1]);
      }
      lcd.setCursor(0, cursorPos);
      lcd.print(">");
      delay(500);
    }
  }
  return idArray[menuPos];
}

void loop(){
  selection();
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
  int id = selectID();
  Serial.println(id);
  int p = -1;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Prilozte otisk");
  lcd.setCursor(0, 1);
  lcd.print("pro pridani");
  while (p != FINGERPRINT_OK){
    p = finger.getImage();
    if(digitalRead(buttons[2]) == 1){
      menuReset();
      return;
    }
    switch (p){
      case FINGERPRINT_OK:
        break;
      case FINGERPRINT_NOFINGER:
        break;
      default:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Error");
        delay(1000);
        menuReset();
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
      lcd.print("Error");
      delay(1000);
      menuReset();
      return;
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Odeberte prst");
  delay(2000);
  p = 0;
  while(p != FINGERPRINT_NOFINGER) {
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
    if(digitalRead(buttons[2]) == 1){
      menuReset();
      return;
    }
    switch (p){
      case FINGERPRINT_OK:
        break;
      case FINGERPRINT_NOFINGER:
        break;
      default:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Error");
        delay(1000);
        menuReset();
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
      lcd.print("Error");
      delay(1000);
      menuReset();
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
    delay(1000);
    menuReset();
    return;
  }
  else{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Error");
    delay(1000);
    menuReset();
    return;
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
  else{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Error");
    delay(1000);
    menuReset();
    return;
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
  int id = selectionDelete();
  if(id == 0)
    menuReset();
  else{
    int p = -1;
    p = finger.deleteModel(id);
    if (p == FINGERPRINT_OK){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Otisk uspesne");
      lcd.setCursor(0, 1);
      lcd.print("smazan");
      delay(3000);
    }
    else{
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error");
      delay(1000);
    }
    menuReset();
  }
}


