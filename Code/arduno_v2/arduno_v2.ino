#include <SoftwareSerial.h>
#include <DHT.h>

#define _ESP_RX 2
#define _ESP_TX 3
#define SerialBaudrate 9600 //Baudrate dùng để debug
#define SSBaudrate 4800 //Baudrate dùng cho SoftwareSerial

SoftwareSerial espSerial(_ESP_RX, _ESP_TX);

const int DHTPin = 12;
const int DHTType = DHT11;
DHT dht(DHTPin, DHTType);

void setup(){
  pinMode(_ESP_RX, INPUT);
  pinMode(_ESP_TX, OUTPUT);
  Serial.begin(SerialBaudrate);
  espSerial.begin(SSBaudrate);
  dht.begin();
}

void loop(){
  espSerial.print(dht.readTemperature());
  espSerial.println("\n");
  delay(500);
}
