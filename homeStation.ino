#include <DHT.h>
#define DHTPIN 7
#define DHTTYPE DHT11   // DHT 11 
DHT dht(7, DHT11);
int DHTmoisture = 0;
int DHThumidity = 0;

/* LIGHT */
int LIGHTpin = 2;

/* GAS SENSOR */
const int GasAOUTpin = 0; //the AOUT pin of the CO sensor goes into analog pin A0 of the arduino
const int GasDOUTpin = 8; //the DOUT pin of the CO sensor goes into digital pin D8 of the arduino
int GasLimit;
int GasValue;

/* LCD */
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int LCDcols = 20;
int LCDrows = 4;

/* HODINY */
#include <Wire.h>
#include "RTClib.h"
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Ne", "Po", "Ut", "St", "Ct", "Pa", "So"};

void setup()
{
  Serial.begin(9600);
  
  /* HODINY */
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  /* GAS SENSOR */ 
  pinMode(GasDOUTpin, INPUT); //sets the pin as an input to the arduino

  /* DHT */
  dht.begin();
  
  /* LCD */
  lcd.begin(20, 4); //define o LCD 20 cols and 4 lines
  
  lcd.setCursor(0, 1);
  lcd.print("Svetlo: ");
  
  lcd.setCursor(0, 2);
  lcd.print("Plyn: ");
}

void loop()
{  
  /* GAS SENSOR */
  GasValue = analogRead(GasAOUTpin); //reads the analaog value from the CO sensor's AOUT pin
  GasLimit = digitalRead(GasDOUTpin); //reads the digital value from the CO sensor's DOUT pin

  /* LCD - GAS */
  clearRowFrom(6, 2);
  lcd.setCursor(6, 2);
  lcd.print(String(GasValue) + " (" + String(GasLimit ? "OK":"PROBLEM") + ")");
  
  /* LCD HODINY */
  clearRow(0);
  lcd.setCursor(0, 0);
  lcd.print(getDateTimeString());

  /* LIGHT */
  int LIGHTval = analogRead(LIGHTpin);
  lcd.setCursor(8, 1);
  lcd.print(LIGHTval);

  /* DHT */
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  /* LOOP */
  delay(1000);

  /* DHT */
  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT");
  } else {
    lcd.setCursor(0, 3);
    lcd.print("H: "+ String(h) + "% " + "T: " + String(t) + "'C");
  }
}

void clearRow(int row)
{
  clearRowFrom(0, row);
}

void clearRowFrom(int col, int row)
{
  lcd.setCursor(col, row);
  for (int i = 0; i < LCDcols - col; i++){
    lcd.print(" ");
  }
}

String zeroLeading(int num)
{
  if (num >= 10) return ""; 
  return "0";
}

String getDateTimeString() {
 
  DateTime now = rtc.now();
  String string = String("");
  string += daysOfTheWeek[now.dayOfTheWeek()];
  string += " ";
  string += now.day();
  string += '.';
  string += now.month();
  string += '.';
  string += now.year() - 2000;
  string += " ";
  string += zeroLeading(now.hour()) + now.hour();
  string += ':';
  string += zeroLeading(now.minute()) + now.minute();
  string += ':';
  string += zeroLeading(now.second()) + now.second();
  return string;
}
