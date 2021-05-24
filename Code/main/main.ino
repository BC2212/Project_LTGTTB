#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include <SerialCommand.h>
#include <DHT.h>

const int RX = 2; //Dùng làm chân RX cho SoftwareSerial
const int TX = 3;  //Dùng làm chân TX cho SoftwareSerial
const long int SerialBaud = 9600;  //Dùng cho cả Serial mặc định và SoftwareSerial

SoftwareSerial mySerial = SoftwareSerial(RX, TX);
SerialCommand sCmd(mySerial);

//DHT
const int DHTPin = 12;
const int DHTType = DHT11;
DHT dht(DHTPin, DHTType);

//Command
const char* TemperatureCommand = "TEMPERATURE";

void setup() {
  Serial.begin(SerialBaud);
  //Khởi tạo Serial cho cổng Serial thứ 2, dùng cho việc kết nối với ESP8266
  mySerial.begin(SerialBaud);

  dht.begin();

  //Tạo lệnh bằng SerialCommand, khi có lệnh sẽ thực thi hàm tương ứng
  sCmd.addCommand(TemperatureCommand, GetTemperature); //Khi có lệnh TEMPERATURE sẽ thực thi hàm GetTemperature
  Serial.println("Ready");
}

void loop() {
  sCmd.readSerial();
}

void GetTemperature(){
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  //Đọc giá trị cảm biến rồi in ra root
  Serial.print("Temperature: ");
//  Serial.println(dht.readTemperature);
  root["Temperature"] = (float)dht.readTemperature();

  //Gửi đi. In ra cổng SoftwareSerial để ESP8266 nhận
  mySerial.print(TemperatureCommand); //Gửi tên lệnh
  mySerial.print('\r');
  root.printTo(mySerial); //Gửi chuỗi JSON
  mySerial.print('\r');
}
