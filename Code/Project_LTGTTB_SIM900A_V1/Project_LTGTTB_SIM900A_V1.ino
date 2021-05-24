#include "SIM900.h"
#include <SoftwareSerial.h>
#include "sms.h"

SMSGSM sms;

#define _GSM_TXPIN_ 2 
#define _GSM_RXPIN_ 3

int numdata;
boolean started=false; //trang thai modul sim
char smstext[160]; // noi dung sms
char number[20]; // so dien thoai format

void setup(){
  Serial.begin(115200);
  Serial.println("Gui va nhan tin nhan");
  
  if(gsm.begin(9600)){
    Serial.println("status=READY");
    started=true;
  } else
    Serial.println("status=IDLE");

    if(started){
      //sms.SendSMS("0393530264", "Online");
      Serial.println("Online");
    }
}

void loop() {
  if(started){
    int pos; // dia chi bo nho sim (sim luu toi da 40sms nen max pos=40)
    pos=sms.IsSMSPresent(SMS_UNREAD); // kiem tra tin nhan chua doc trong bo nho
    // ham nay se tra ve gia tri trong khoang tu 0-40
    Serial.println(pos);

    if(pos){ //neu co tin nhan chua doc
      if(sms.GetSMS(pos, number, smstext, 160)){
        Serial.print("So dien thoai: ");
        Serial.println(number);
        Serial.print("Noi dung tin nhan: ");
        Serial.println(smstext);
        String _smstext = smstext;
        if (_smstext == "NHIET_DO"){
          Serial.println("Tin nhan dung cu phap");
        } else{
          Serial.println("Tin nhan sai cu phap");
        }
//        sms.SendSMS(number, "Da doc tin");
      }
    }
    //delay(1000);
  } else Serial.println("Offline");
}
