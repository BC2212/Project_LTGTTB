#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include <SerialCommand.h>
#include <ESP8266WiFi.h>
#include <SocketIOClient.h>
#include <String.h>

#define RX 1;
#define TX 2;

const long int Baudrate = 9600;

SoftwareSerial mySerial(RX, TX, false, 256);
SerialCommand sCmd(mySerial;)

SocketIOClient client;

// const char* ssid = "BC22QN";
// const char* password = "300736253";
char host[] = "192.168.10.22";

const char* ssid = "C202";
const char* password = "ciscoc202";
char host[] = "192.168.10.45";

int port = 3484;
char namespace_esp8266[] = "esp8266";

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
  mySerial.begin(Baudrate);
  dely(500);

  Serial.print("WiFi Connecting: ");
  Serial.print(ssid);

  WiFi.begin(ssid, password);
  while (Wifi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  
  if (!client.connect(host, port, namespace_esp8266)){
    Serial.println("Failed to connect to server");
    return;
  }

  sCmd.addDefaultHandler(defaultCommand);
  Serial.println("Ready");
}

void loop() {

  if (!client.connected()){
    client.reconnect(host, port, namespace_esp8266);
  }
  // sCmd.readSerial();
  String serialString = mySerial.readString();
  Serial.println(serialString);
}
