#include <SPI.h>
#include <LoRa.h>

#define ADDR 1;

uint16_t counter = 0;
byte MSG[10]={0,0,255,255,255,255,1,0,0,0};

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("LoRa Sender");
  LoRa.setPins(5, 21, 16);
  //LoRa.setSPIFrequency(frequency);
  if (!LoRa.begin(866E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  
  Serial.print("Sending packet: ");
  Serial.println(counter);
  MSG[0]=counter & 0xFF;
  MSG[1]=counter >>8 & 0xFF;
  //Serial.print(MSG[9]);
  //Serial.print("-");
  //Serial.print(MSG[8]);
  //Serial.print("-");
  //Serial.print(MSG[7]);
  //Serial.print("-");
  //Serial.print(MSG[6]);
  //Serial.print("-");
  //Serial.print(MSG[5]);
  //Serial.print("-");
  //Serial.print(MSG[4]);
  //Serial.print("-");
  //Serial.print(MSG[3]);
  //Serial.print("-");
  //Serial.print(MSG[2]);
  //Serial.print("-");
  //Serial.print(MSG[1]);
  //Serial.print("-");
  //Serial.println(MSG[0]);
  //Serial.println(MESG[c-10]);
  // send packet
  LoRa.beginPacket();
  //LoRa.print("hello ");
  LoRa.write(MSG, 10);
  //LoRa.println(counter);
  LoRa.endPacket();

  counter++;

  delay(200);
}
