#include <SoftwareSerial.h>
#include <SerialCommand.h>

#define _ESP_RX 2;
#define _ESP_TX 3;

const long int Baudrate = 9600;

SoftwareSerial espSerial(_ESP_RX, _ESP_TX);
SerialCommand espCmd(espSerial);

void setup()
{
  Serial.begin(Baudrate);
  espSerial.begin(Baudrate);
  espCmd.addCommand("TEMPERATURE", GetTemperature);
}

unsigned long chuky1 = 0;
const unsigned long CHU_KY_1_LA_BAO_NHIEU = 3000;

void loop()
{
  if (millis() - chuky1 > CHU_KY_1_LA_BAO_NHIEU)
  {
    chuky1 = millis();
    GetTemperature();
  }

  sCmd.readSerial();
}

void GetTemperature()
{
  espCmd.print("TEMPERATURE");
  espCmd.print('\r');
  espCmd.print('a');
  espCmd.print('\r');

  Serial.println("Da chay den day");s
}