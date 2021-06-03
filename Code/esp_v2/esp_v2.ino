#include <SoftwareSerial.h>
#include <SocketIOClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

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

char* ssid = "C202";
char* pwd = "ciscoc202";
char host[] = "192.168.10.45";

int port = 3484;
char namespace_esp8266[] = "esp8266";

StaticJsonDocument<200> jsonDoc;
JsonObject root = jsonDoc.to<JsonObject>();

void setup() {
  Serial.begin(SerialBaudrate);
  arduinoSerial.begin(SSBaudrate);
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
      root["digital"] = val;
      String jsonString;
      serializeJson(jsonDoc, jsonString);
      
      if (DEBUG){
        serializeJson(jsonDoc, Serial);
        Serial.println();
      }

      if (client.connect(host, port, namespace_esp8266)){
        client.send("TEMPERATURE", jsonString);
      }
    }
  }
  delay(500);
}
