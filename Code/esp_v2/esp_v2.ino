#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

SoftwareSerial arduinoSerial(D1,D2);

void setup(){
  Serial.begin(9600);
  arduinoSerial.begin(4800);
  pinMode(D1, INPUT);
  pinMode(D2, OUTPUT);
}

void loop(){
  arduinoSerial.println("Hello");
  Serial.println("Hello");
  delay(30);
}