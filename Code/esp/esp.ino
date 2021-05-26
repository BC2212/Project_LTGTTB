#include <SocketIOClient.h>
#include <SoftwareSerial.h>
#include <SerialCommand.h>
#include <ESP8266WiFi.h>

const long int Baudrate = 9600;  //Dùng cho cả Serial và SoftwareSerial

//Include thư viện để kiểm tra free RAM trên ESP8266
extern "C" {
  #include "user_interface.h"
}

//Dùng để khai báo chân RX/TX cho Serial ảo trên ESP8266
const byte RX = 1;
const byte TX = 2;

SoftwareSerial mySerial = SoftwareSerial(RX, TX, false, 256);
SerialCommand sCmd(mySerial);

SocketIOClient client;
//WiFi nhà Q
const char* ssid = "BC22QN";
const char* password = "300736253";
//WiFi C202
//const char* ssid = "C202";
//const char* password = "anti.nms";

char host[] = "192.168.10.22"; //Nếu kết nối đến heroku thì dùng subdomain
int port = 3484;  //Nếu kết nối đến heroku thì dùng port 80
char namespace_esp8266[] = "esp8266"; //Thêm Arduino

//Từ khóa extern: dùng để include các biến toàn cục từ một số thư viện khác.
//Trong thư viện SocketIOClient có hai biến toàn cục cần quan tâm là: RID (tên hàm/tên sự kiện), Rfull (danh sách biến - được đóng gói lại thành chuỗi JSON)
extern String RID;
extern String Rfull;

void defaultCommand(String command){
  char *json = sCmd.next();
  client.send(command, (String) json);  //Gửi dữ liệu về cho Socket Server
  Serial.print("--------------");
  //In ra serial monitor để debug
  Serial.print(command);
  Serial.print(' ');
  Serial.println(json);
}

void setup() {
  Serial.begin(Baudrate);
  mySerial.begin(Baudrate); //Bật SoftwareSerial để giao tiếp với Arduino, baudrate trùng với SoftwareSerial trên Arduino
  delay(50);

  //Kết nối WiFi
  Serial.print("WiFi Connecting: ");
  Serial.print(ssid);
  
  WiFi.begin(ssid, password);
  //Chờ đến khi kết nối thành công
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.println(F("WiFi Connected"));
  Serial.print(F("IP Address: "));
  Serial.println(WiFi.localIP());

  if (!client.connect(host, port, namespace_esp8266)){
    Serial.println(F("Failed to connect to server"));
    return;
  }

//  Khi đã kết nối thành công
//  if (client.connected()){
//    //Gửi sự kiện ("connection") đến SocketServer
//    client.send("connection", "message", "Connected !!!!");
//  }

  sCmd.addDefaultHandler(defaultCommand); //Bắt bất kì lệnh nào đi qua, rồi chuyển xuống hàm defaultComand
  Serial.println("Ready");
}

void loop() {
  //Khi bắt được bất kì sự kiện nào thì chúng ta có hai tham số:
  //+RID: Tên sự kiện
  //+Rfull: Danh sách tham số được nén thành chuỗi JSON
  if (client.monitor()){
    //In ra Serial cho Arduino
    mySerial.print(RID);
    mySerial.print('\r');
    mySerial.print(Rfull);
    mySerial.print('\r');

    //In ra Serial Monitor
    Serial.print(RID);
    Serial.print(' ');
    Serial.println(Rfull);

    //Kiểm tra em còn dư bao nhiêu RAM để debug
    uint32_t free = system_get_free_heap_size();
//    Serial.println(free);
  }

  //Kết nối lại
  if (!client.connected()){
    client.reconnect(host, port, namespace_esp8266);
  }
  sCmd.readSerial();
}
