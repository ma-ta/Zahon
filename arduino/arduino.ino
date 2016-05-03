#define VERZE "0.1"

// Displej
#include <Nokia5110.h>
LCDnokia5110 lcd(7, 12, 11, 10, 9);

// Teploměr DS18B20
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS_1 8
OneWire oneWire_1(ONE_WIRE_BUS_1);
DallasTemperature sensor_teplota_1(&oneWire_1);

// vstupní PINy
const uint8_t PIN_SVETLO = A0;
const uint8_t PIN_SRAZKY = A1;
const uint8_t PIN_PUDA = A2;

// napájecí PINy
const int ONBOARD_LED = 13;
const int PIN_NAPAJENI_SVETLO = 2;
const int PIN_NAPAJENI_SRAZKY = 3;
const int PIN_NAPAJENI_PUDA = 4;
const int PIN_NAPAJENI_TEPLOTA = 5;

// nastavení
const int FREKVENCE_MERENI = 1000;
const int PAUZA_STABILIZACE = 150;
const int PAUZA_STABILIZACE_DS18B20 = 1000;

// hodnoty měření
int uroven_osvetleni;
int uroven_srazek;
int vlhkost_pudy;
float teplota_vzduchu;

char* RxZprava = "Wi-Fi: nevys\xcdl\xc1m";
bool nastaveni = true;

void setup() {
  pinMode(ONBOARD_LED, OUTPUT);
  pinMode(PIN_NAPAJENI_SVETLO, OUTPUT);
  pinMode(PIN_NAPAJENI_SRAZKY, OUTPUT);
  pinMode(PIN_NAPAJENI_PUDA, OUTPUT);
  pinMode(PIN_NAPAJENI_TEPLOTA, OUTPUT);

  digitalWrite(ONBOARD_LED, LOW);
  digitalWrite(PIN_NAPAJENI_SVETLO, LOW);
  digitalWrite(PIN_NAPAJENI_SRAZKY, LOW);
  digitalWrite(PIN_NAPAJENI_PUDA, LOW);
  digitalWrite(PIN_NAPAJENI_TEPLOTA, LOW);

  lcd.LcdInitialise();
  lcd.LcdClear();
  lcd.GotoXY(53, 4);
  lcd.LcdString("VERZE");
  lcd.GotoXY(66, 5);
  lcd.LcdString(VERZE);
  delay(2500);
  lcd.LcdClear();
  lcd.ShowLogo(2500);
  lcd.LcdClear();
  lcd.GotoXY(0, 1);
  lcd.LcdString("---- B\xddL\xcd ----");

  Serial.begin(9600);
  while (!Serial) {
  }

}

void loop() {

  if (Serial.available()) {
    delay(100);
    int i = 0;
    for (i; i <= 16; i++) {
      RxZprava[i] = '\0';
    }
    i = 0;
    while (Serial.available() > 0) {
      RxZprava[i++] = Serial.read();
    }
    
  }
  else {
    if ((millis() % FREKVENCE_MERENI) == 0) {
      mereni();
    }
  }

}


// funkce:

void mereni() {
  digitalWrite(ONBOARD_LED, HIGH);

  //Světlo
  uroven_osvetleni = svetlo();
  //Srážky
  uroven_srazek = srazky();
  //Půda
  vlhkost_pudy = puda();
  //Teplota
  teplota_vzduchu = teplota();


  //Odeslání hodnot přes sériový port
  Serial.println(String(uroven_osvetleni) + "," + String(uroven_srazek) + "," + String(vlhkost_pudy) + "," + String(teplota_vzduchu));

  //Zobrazení hodnot na displeji
  lcd.GotoXY(0, 0);
  lcd.CharSpace = 0;
  lcd.LcdString("                ");
//  char* RxZprava_char = "";
//  RxZprava.toCharArray(RxZprava_char, RxZprava.length() + 1);
  lcd.GotoXY(0, 0);
  lcd.CharSpace = 0;
  lcd.LcdString(RxZprava);
  lcd.CharSpace = 1;

  lcd.GotoXY(0, 2);
  lcd.LcdString("Sv\xcatlo:");
  lcd.GotoXY(60, 2);
  lcd.LcdString("    ");
  lcd.GotoXY(60, 2);
  lcd.LcdInt(uroven_osvetleni);

  lcd.GotoXY(0, 3);
  lcd.LcdString("Sr\xc1\x8eky:");
  lcd.GotoXY(60, 3);
  lcd.LcdString("    ");
  lcd.GotoXY(60, 3);
  lcd.LcdInt(uroven_srazek);

  lcd.GotoXY(0, 4);
  lcd.LcdString("P\xdb\da:");
  lcd.GotoXY(60, 4);
  lcd.LcdString("    ");
  lcd.GotoXY(60, 4);
  lcd.LcdInt(vlhkost_pudy);

  lcd.GotoXY(0, 5);
  lcd.LcdString("Teplota:");
  lcd.GotoXY(60, 5);
  lcd.LcdString("    ");
  lcd.GotoXY(60, 5);
  lcd.LcdInt(teplota_vzduchu);
  lcd.LcdString("'C");


  digitalWrite(ONBOARD_LED, LOW);
}

int svetlo() {
  int svetlo;
  digitalWrite(PIN_NAPAJENI_SVETLO, HIGH);
  delay(PAUZA_STABILIZACE);
  svetlo = analogRead(PIN_SVETLO);
  digitalWrite(PIN_NAPAJENI_SVETLO, LOW);
  return svetlo;
}
int srazky() {
  int srazky;
  digitalWrite(PIN_NAPAJENI_SRAZKY, HIGH);
  delay(PAUZA_STABILIZACE);
  srazky = analogRead(PIN_SRAZKY);
  digitalWrite(PIN_NAPAJENI_SRAZKY, LOW);

  // kalibrace
  //  if (srazky < 200) {
  //    srazky = 0;
  //  }
  //  else if (srazky > 660) {
  //    srazky = 100;
  //  }
  //  else {
  //    srazky = map(srazky, 200, 660, 1, 40);
  //  }

  return srazky;
}
int puda() {
  int puda;
  digitalWrite(PIN_NAPAJENI_PUDA, HIGH);
  delay(PAUZA_STABILIZACE);
  puda = analogRead(PIN_PUDA);
  digitalWrite(PIN_NAPAJENI_PUDA, LOW);

  // kalibrace
  //  if (puda > 700) {
  //    puda = 0;
  //  }
  //  else if (puda < 75) {
  //    puda = 100;
  //  }
  //  else {
  //    puda = map(puda, 700, 75, 1, 100);
  //  }

  return puda;
}
float teplota() {
  float teplota;
  digitalWrite(PIN_NAPAJENI_TEPLOTA, HIGH);
  delay(PAUZA_STABILIZACE_DS18B20);
  sensor_teplota_1.begin();
  sensor_teplota_1.requestTemperatures();
  teplota = sensor_teplota_1.getTempCByIndex(0);
  digitalWrite(PIN_NAPAJENI_TEPLOTA, LOW);
  return teplota;
}
