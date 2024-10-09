#include <Arduino.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Wire.h>
#include <SPI.h>
#include <DallasTemperature.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define i2c_Address 0x3c
#define OLED_RESET -1

#define liquidTempPin 17
#define airTempPin 16
#define DHTTYPE DHT11

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT airSensor(airTempPin, DHTTYPE);
OneWire oneWire (liquidTempPin);
DallasTemperature liquidSensor(&oneWire);

int second = 0, minute = 0, hour = 0;

void setup() {
  Serial.begin(115200);

  airSensor.begin();
  liquidSensor.begin();
  
  if(!display.begin(i2c_Address, true)){
    Serial.println("SH110x allocation failed");
    for(;;);
  }
  delay(2000);

  // initialize Display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 10);
  display.print("Sensor 1:");
  display.display();
  delay(500);
  if(!isnan(airSensor.readTemperature())){
    	display.println(" Bereit!");
      display.display();
  } else {
    display.println(" Fehler!");
    display.display();
    while (true){}
  }
  display.setCursor(0, 20);
  delay(100);
  display.print("Sensor 2:");
  display.display();
  delay(500);
  liquidSensor.requestTemperatures();
  if(liquidSensor.getTempCByIndex(0) != -127){
      display.println(" Bereit!");
      display.display();
  } else {
    display.println(" Fehler!");
    display.display();
    while (true){}
  }
  display.setCursor(0, 30);
  display.println("Bereit f. Datenreihe");
  display.setCursor(0, 40);
  display.println("warte auf \"start\"");
  display.display();

  display.setCursor(0, 50);
  String condition = "";
  while (condition != "start")  {
    while (Serial.available()){
      condition = Serial.readString();
      display.print(condition);
      display.display();      
    }
  }

  delay(2000);
}



void loop() {
  float airTemp = airSensor.readTemperature();
  liquidSensor.requestTemperatures();
  float liquidTemp = liquidSensor.getTempCByIndex(0);

  if(second % 60 == 0 && second > 0){
    minute++;
  }
  if(minute % 60 == 0 && minute > 0){
    minute = 0;
    hour++;
  }
  updateDisplay(airTemp, liquidTemp);

  if(second % 10 == 0){
    Serial.printf("%d:%d:%d;%f;%f\n", hour, minute, second % 60, airTemp, liquidTemp);
  }
  second++;
  delay(935);
}


void updateDisplay(float airTemp, float liquidTemp){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setTextWrap(false);
  
  display.setCursor(0, 12);
  
  display.print("Timer: ");
  if(hour < 10){
    display.printf("0%d:", hour);
  } else {
    display.printf("%d:", hour);
  }  
  if(minute < 10) {
    display.printf("0%d:", minute);
  } else {
    display.printf("%d:", minute);
  }
  if(second % 60 < 10){
    display.printf("0%d", second % 60);
  } else {
    display.printf("%d", second % 60);
  }
  
  display.setCursor(0, 22);
  display.print("Raum Temperatur: ");
  display.setCursor(10, 32);
  if (isnan(airTemp)){
    display.println("Reading Error!");
  } else {
    display.print(airTemp);
    display.println(" C");
  }

  display.setCursor(0, 42);
  display.print("Wasser Temperatur: ");
  display.setCursor(10, 52);
  if (liquidTemp == -127){
    display.println("Reading Error!");
  } else {
    display.print(liquidTemp);
    display.println(" C");
  }
  display.display();
}