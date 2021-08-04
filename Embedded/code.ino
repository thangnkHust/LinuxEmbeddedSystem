#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

#define TX_PIN      7
#define RX_PIN      6

LiquidCrystal_I2C lcd(0x27, 20, 4);
SoftwareSerial bluetooth(RX_PIN, TX_PIN);

int baudRate[] = {300, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200};
const int DHTPIN = 4;
const int DHTTYPE = DHT11;
const int RELAY_PIN = 5;

DHT dht(DHTPIN, DHTTYPE);

byte degree[8] = {
  0B01110,
  0B01010,
  0B01110,
  0B00000,
  0B00000,
  0B00000,
  0B00000,
  0B00000
};

String data_relay = "OFF";
String automatic = "auto: off";
String data_response;

void setup() {
  Serial.begin(9600);

// LCD 
  lcd.init();
  lcd.backlight();
  lcd.setCursor(6, 0);
  lcd.print("Team 8");
  lcd.setCursor(0, 1);
  lcd.print("Nhiet do: ");
  lcd.setCursor(0, 2);
  lcd.print("Do am: ");
  lcd.createChar(1, degree);
  lcd.setCursor(0, 3);
  lcd.print("Auto: ");
  lcd.setCursor(10, 3);
  lcd.print("Relay: ");
  dht.begin();
  pinMode(RELAY_PIN, OUTPUT);

//  Bluetooth
//  while (!Serial) {}
//  Serial.println("Configuring, please wait...");
  for (int i = 0 ; i < 9 ; i++) {
     bluetooth.begin(baudRate[i]);
     String cmd = "AT+BAUD4";
     bluetooth.print(cmd);
     bluetooth.flush();
     delay(100);
  }
  
  bluetooth.begin(9600);
//  Serial.println("Config done");
//  while (!bluetooth) {}
//  Serial.println("Enter AT commands:");
}

void loop() {
  if (bluetooth.available()) {
      data_response = bluetooth.readString();
      
      if(data_response == "ON" || data_response == "OFF" || data_response == "mở máy bơm" || data_response == "tắt máy bơm") {
          data_relay = data_response;
      }
     
      if(data_response == "auto: on" || data_response == "auto: off") {
        automatic = data_response;
      }
  }
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  Serial.println(String(round(t)) + '_' + String(round(h)));

  if (isnan(t) || isnan(h)) {
  }
  else {
    lcd.setCursor(10, 1);
    lcd.print(round(t));
    lcd.print(" ");
    lcd.write(1);
    lcd.print("C");

    lcd.setCursor(10, 2);
    lcd.print(round(h));
    lcd.print(" %");

    if(automatic == "auto: on") {
      if(round(h) < 50 || round(t) > 35) {
         data_relay = "ON";
      } else {
         data_relay = "OFF";
      }
      
      lcd.setCursor(5, 3);
      lcd.print("ON ");
    } else {
      lcd.setCursor(5, 3);
      lcd.print("OFF");
    }
  }

  digitalWrite(RELAY_PIN, data_relay == "ON" || data_relay == "mở máy bơm" ? HIGH : LOW);

  if(data_relay == "ON" || data_relay == "mở máy bơm") {
    digitalWrite(RELAY_PIN, HIGH);
    lcd.setCursor(16, 3);
    lcd.print("ON ");
  } else {
    digitalWrite(RELAY_PIN, LOW);
    lcd.setCursor(16, 3);
    lcd.print("OFF");
  }
  
  delay(1000);
  Serial.flush();
}
