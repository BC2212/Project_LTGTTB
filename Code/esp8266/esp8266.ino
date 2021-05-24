#include <SocketIoClient.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <SerialCommand.h>
#include <WiFiClient.h>

//Include thư viện để kiểm tra free RAM trên ESP8266
extern "C" {
  #include "user_interface.h"
}

int RX = 1;
int TX = 2;
const long int SerialBaud = 9600; //Dùng cho cả Serial và SoftwareSerial

SoftwareSerial mySerial(1, 2, false);
SerialCommand sCmd(mySerial);

char* namespace_esp8266 = "esp8266"; //Thêm Arduino

//Keyword extern: dùng để include các biến toàn cục ở một số thư viện khác. Trong thư viện SocketIOclient có hai biến toàn cục
//cần quan tâm là:
//1. RID: Tên hàm (tên sự kiện)
//2. Rfull: Danh sách biến (được đóng gói lại là chuỗi JSON)
extern String RID;
extern String Rfull;

const char* ssid = "C202"; //Tên WiFi sẽ kết nối đến, nếu chạy trong local thì phải chung mạng với socket server

char* host = "192.168.10.45"; //Địa chỉ IP của socket server, nếu chạy trên Internet thì có thể dùng domain
int port = 3484;  //Port của socket server. Dùng 3484 nếu chạy trong mạng local, 80 nếu chạy trên heroku
WiFiClient client;

void setup() {
  Serial.begin(SerialBaud);
  mySerial.begin(SerialBaud);
  delay(10);

  //Kết nối vào mạng WiFi
  Serial.print("WiFi Connecting: ");
  Serial.println(ssid);

  //Chờ cho đến khi kết nối thành công
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print('.');
  }

  Serial.println();
  Serial.println(F("WiFi Connected!!!"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());

  if (!client.connect(host, port)){
    Serial.println(F("Cannot connect to Socket Server"));
    return;
  }

//  sCmd.addDefaultHandler(defaultCommand); //Bắt mọi lệnh đi qua, rồi chuyển xuống hàm defaultCommand
  Serial.println("Ready");
}

void loop() {
  //Khi bắt được bất kì sự kiện nào thì sẽ có 2 tham sô:
  //+RID: Tên sự kiện
  //+RFull: Danh sách tham số được nén thành chuỗi JSON
  if (client.available()){
    //In ra Serial cho Arduino
    mySerial.print(RID);
    mySerial.print('\r');
    mySerial.print(Rfull);
    mySerial.print('\r');

    //In ra Serial Monitor
    Serial.print(RID);
    Serial.print(' ');
    Serial.println(Rfull);

    //Kiểm tra xem còn bao nhiêu RAM để debug
    uint32_t free = system_get_free_heap_size();
    Serial.println(free);
  }

  //Kết nối lại
  if (!client.connected()){
    client.connect(host, port);
  }

  sCmd.readSerial();
}

//void defaultCommand(){
//  char *json = sCmd.next();
//  client.print("TEMPERATURE", (String) json);  //Gửi dữ liệu về cho Socket Server
//
//  //In ra Serial Monitor để debug
//  Serial.print("TEMPERATURE");
//  Serial.print(' ');
//  Serial.println(json);
//}
