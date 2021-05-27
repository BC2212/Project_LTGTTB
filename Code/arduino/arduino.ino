#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include <SerialCommand.h>
#include <DHT.h>

const long int Baudrate = 9600; //Dùng cho cả Serial phần cứng và SoftwareSerial.

const byte _ESPRX = 3;
const byte _ESPTX = 2;

SoftwareSerial mySerial = SoftwareSerial(_ESPRX, _ESPTX);
SerialCommand sCmd(mySerial);

//DHT11
const int DHTPin = 12;
const int DHTType = DHT11;
DHT dht(DHTPin, DHT11);
float humidity;
float temperature;

void setup() {
  //Khởi tạo Serial.
  Serial.begin(Baudrate);
  //Khởi tạo SoftwareSerial, baudrate phải trùng với ESP8266
  mySerial.begin(Baudrate);

  //Khởi tạo dht
  dht.begin();
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  //Một số hàm trong thư viện SerialCommand
  sCmd.addCommand("TEMPERATURE", GetTemperature);
  Serial.println("Ready");
}

unsigned long chuky1 = 0;
const unsigned long CHU_KY_1_LA_BAO_NHIEU = 3000; //Cứ sau 5000ms = 5s thì chu kỳ lặp lại

void loop() {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  if (millis() - chuky1 > CHU_KY_1_LA_BAO_NHIEU){
    chuky1 = millis();
//    GetTemperature();
  char json[] = "{\"hello\":\"world\"}";
StaticJsonBuffer<200> jsonBuffer;
JsonObject& object = jsonBuffer.parseObject(json);
const char* world = object["hello"];
Serial.println(world);
  }
  
//  Serial.println(temperature);
  sCmd.readSerial();
}

void GetTemperature(){
//  Serial.print("Temperature: ");
//  Serial.println(temperature);
  char* json = sCmd.next();
  StaticJsonBuffer<200> jsonBuffer; 
  
  //Tạo buffer json có khả năng chứa tối đa 200 kí t
//  JsonObject& root = jsonBuffer.createObject();
  JsonObject& root = jsonBuffer.parseObject(json);  //Đặt một biến root mang kiểu json

//  root["digital"] = temperature;
    root["digital"] = "Test";
root.printTo(Serial);

  //Gửi đi
  //In ra cổng SoftwareSerial để ESP8266 nhận
  mySerial.print("TEMPERATURE");  //Gửi tên lệnh
  mySerial.print('\r'); //Gửi /r
  root.printTo(mySerial);   //Gửi chuỗi JSON
//  root.printTo(Serial);
  mySerial.print('\r');
}
