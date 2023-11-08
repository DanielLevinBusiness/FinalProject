#include <Wire.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_INA219.h>
Adafruit_INA219 ina219;
SoftwareSerial sim(2, 3);
LiquidCrystal_I2C lcd(0x27, 16, 2);

String _buffer;
int _timeout;
String number = "+972507168509";
float shuntvoltage = 0;
float busvoltage = 0;
float current_mA = 0;
float loadvoltage = 0;
float power_mW = 0;
float current = 0;
float voltage = 0;
float power = 0;
float BAT = 0;
int batread = A2;

void setup() {
  _buffer.reserve(50);
  sim.begin(115200);
  sim.println("AT+CNMI=2,2,0,0,0");
  uint32_t currentFrequency;
  ina219.begin();
  lcd.init();
  pinMode(batread, INPUT);
}

void SendMessage() {
  sim.println("AT+CMGF=1");
  delay(100);
  sim.println("AT+CMGS=\"" + number + "\"\r");
  delay(100);
  String SMS = ("I: ");
  SMS += (current_mA);
  SMS += (" V: ");
  SMS += (loadvoltage);
  SMS += (" P: ");
  SMS += (power_mW);
  SMS += (" BAT: ");
  SMS += (BAT);
  SMS += ("%");
  Serial.println(SMS);
  sim.println(SMS);
  sim.println((char)26);
  delay(100);
}


void LCD() {
  BAT = analogRead(batread) * 0.00488;
  BAT = BAT - 3.7;
  BAT = BAT / 0.0024;
  if ( BAT < 0) {
    BAT = 0;
  }
  if ( BAT > 100) {
    BAT = 100;
  }
  int y = round(BAT);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("I:");
  lcd.print(current_mA);
  lcd.setCursor(10, 0);
  lcd.print("V:");
  lcd.print(loadvoltage);
  lcd.setCursor(0, 1);
  lcd.print("P:");
  lcd.print(power_mW);
  lcd.setCursor(8, 1);
  lcd.print("BAT:");
  lcd.print(y);
  lcd.print("%");
}

void messurement() {
  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);
  delay(1000);
}

void loop() {
  messurement();
  LCD();
  if (sim.read() == 'P') {
    SendMessage();
  }
}
