
#include <SoftwareSerial.h>

SoftwareSerial espSerial(2, 3);

void setup(){
  Serial.begin(9600);
  espSerial.begin(4800);
}

void loop(){
  while(espSerial.available()>0){
//    String val = (String) espSerial.readString();
////    if(espSerial.read() == '\n'){
////      Serial.println(val);
////    }
//    Serial.println(val);
    Serial.println(Serial.read());
  }
  delay(30);
}
