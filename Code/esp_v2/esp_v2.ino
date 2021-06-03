#include <SoftwareSerial.h> //Thư viện sử dụng softwareserial. SoftwareSerial sẽ tạo ra một port Serial ảo
#include <SocketIOClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>  //Thư viện tạo json

#define RX D1
#define TX D2
#define SerialBaudrate 9600
#define SSBaudrate 4800
#define DEBUG true

SoftwareSerial arduinoSerial(D1, D2, false, 256);
SocketIOClient client;

//char* ssid = "BC22QN";
//char* pwd = "300736253";
//char host[] = "192.168.10.22";

char* ssid = "TTCisco";
char* pwd = "cisco@2020";
char host[] = "arcane-basin-01889.herokuapp.com";

//int port = 3484;
int port = 80;
char namespace_esp8266[] = "esp8266";

StaticJsonDocument<200> jsonDoc;
JsonObject root = jsonDoc.to<JsonObject>();

void setup() {
  Serial.begin(SerialBaudrate);
  arduinoSerial.begin(SSBaudrate);  //Khởi tạo port serial ảo
  pinMode(RX, INPUT);
  pinMode(TX, OUTPUT);

  Serial.print("WiFi Connecting: ");
  Serial.println(ssid);
  WiFi.begin(ssid, pwd);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

//Kiểm tra kết nối đến server. Câu lệnh connect dùng thêm namespace vì server chia ra làm 2 socketio: webapp và esp
  if (!client.connect(host, port, namespace_esp8266)) {
    Serial.println("Failed to connnect to server");
    return;
  }
  Serial.println("Ready");
}

void loop() {
  while (arduinoSerial.available() > 0) {
    float val = arduinoSerial.parseFloat();
    if (arduinoSerial.read() == '\n') {
      root["digital"] = val;  //Gán key vào json

      //Chuyển json thành string, vì hàm client.send() cần truyền tham số là string
      String jsonString;
      serializeJson(jsonDoc, jsonString); //Câu lệnh này có thể dùng để in (như dòng dưới) hoặc dùng để tạo ra gán giá trị cho biến
      
      if (DEBUG){
        serializeJson(jsonDoc, Serial);
        Serial.println();
      }

      //Sau khi tạo chuỗi json, gửi đến socketserver
      if (client.connect(host, port, namespace_esp8266)){
        //Nếu kết nối thành công, thực hiện gửi đi
        client.send("TEMPERATURE", jsonString);
      }
    }
  }
  delay(500);
}
