#include <SoftwareSerial.h>
#include <LiquidCrystal.h>      //Knihovna pro LCD

#define NAME ""             //Wifi SSID
#define PASS ""             //Wifi heslo

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
SoftwareSerial ESP(6, 7);

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

  ESP.println("AT+RST");
  lcd.setCursor(0, 0);
  lcd.print("Connecting");
  lcd.setCursor(0, 1);
  lcd.print("to WiFi");
  //Připojení k wifi
  while(1){
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
  lcd.clear();
  //Stisknutím 3. tlačítka se vrátí do menu
  while(digitalRead(buttons[2]) == 0){
    //Zde bude kód pro přidání nového otisku do databáze
  }
  menuReset();
}

void checkFinger(){
  lcd.clear();
  while(digitalRead(buttons[2]) == 0){
    //Zde bude kód pro ověření existujícího otisku v databázi
  }
  menuReset();
}

void deleteFinger(){
  lcd.clear();
  while(digitalRead(buttons[2]) == 0){
    //Zde bude kód pro smazání existujícího otisku v databázi
  }
  menuReset();
}


