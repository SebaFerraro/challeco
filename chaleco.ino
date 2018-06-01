#include <SPI.h>
#include <LoRa.h>
#include "freertos/event_groups.h"

#define TASKLED_1_BIT        ( 1 << 0 ) //1
#define LED 2
#define ADDR 2;
bool Termina=false;
EventGroupHandle_t eg;

static int taskCore = 0;
uint16_t contador=0;
 
void LEDTask( void * pvParameters ){

    while (!Termina){
        EventBits_t xbit = xEventGroupWaitBits(eg, TASKLED_1_BIT, pdTRUE, pdTRUE, portMAX_DELAY);
        String taskMessage = "TaskLED running on core ";
        taskMessage = taskMessage + xPortGetCoreID();
        //Serial.println(taskMessage);
        digitalWrite(LED, HIGH);
        delay(500);
        digitalWrite(LED, LOW);
    }
    vTaskDelete( NULL );
}

void setup() {
  Serial.begin(115200);
  while (!Serial);
  pinMode(LED, OUTPUT);
  eg = xEventGroupCreate();
  Serial.print("Starting to create task on core ");
  Serial.println(taskCore);
  String taskMessage = "Setup running on core ";
  taskMessage = taskMessage + xPortGetCoreID();
  Serial.println(taskMessage);
  xTaskCreatePinnedToCore(
                    LEDTask,   /* Function to implement the task */
                    "LEDTask", /* Name of the task */
                    10000,      /* Stack size in words */
                    NULL,       /* Task input parameter */
                    1,          /* Priority of the task */
                    NULL,       /* Task handle. */
                    taskCore);  /* Core where the task should run */
 
  Serial.println("Task created...");

  Serial.println("LoRa Receiver");
  LoRa.setPins(5, 21, 16);

  if (!LoRa.begin(866E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
      String taskMessage = "Loop running on core ";
      taskMessage = taskMessage + xPortGetCoreID();
      //Serial.println(taskMessage);
      // received a packet
      //Serial.print("Received packet '");

      // read packet
      byte MESG[128]={0};
      int c=0;
      while (LoRa.available()) {
        MESG[c]=(byte)LoRa.read();
        c++;
      }
      if(c == 10){
         uint32_t origen=(((((uint8_t)MESG[c-1]) <<24) + (((uint8_t)MESG[c-2]) <<16) + (((uint8_t)MESG[c-3]) <<8) + (uint8_t)MESG[c-4]) & 0xFFFFFFFF);
         uint32_t destino=(((((uint8_t)MESG[c-5]) <<24 )+ (((uint8_t)MESG[c-6]) << 16) + (((uint8_t)MESG[c-7]) << 8) + (uint8_t)MESG[c-8]) & 0xFFFFFFFF);
         uint16_t count=((((uint8_t)MESG[c-9]) << 8) + (uint8_t)MESG[c-10]) & 0xFFFF;
         //Serial.print(MESG[9]);
         //Serial.print("-");
         //Serial.print(MESG[8]);
         //Serial.print("-");
         //Serial.print(MESG[7]);
         //Serial.print("-");
         //Serial.print(MESG[6]);
         //Serial.print("-");
         //Serial.print(MESG[5]);
         //Serial.print("-");
         //Serial.print(MESG[4]);
         //Serial.print("-");
         //Serial.print(MESG[3]);
         //Serial.print("-");
         //Serial.print(MESG[2]);
         //Serial.print("-");
         //Serial.print(MESG[1]);
         //Serial.print("-");
         //Serial.println(MESG[0]);
         //Serial.print("-");
         //Serial.println(MESG[c-10]);
         
         int rssi=LoRa.packetRssi();
         // print RSSI of packet
         if ( rssi > -57 ){
             if (destino == 0xFFFFFFFF ){
                if(contador== count -1){
                  Serial.print("Received packet Origen:");
                  Serial.print(origen);
                  Serial.print(" Destino:");
                  Serial.print(destino);
                  Serial.print(" Count:");
                  Serial.print(count);
                  Serial.print(" with RSSI ");
                  Serial.println(rssi);        
                  xEventGroupSetBits(eg,TASKLED_1_BIT);
                }else{
                  contador=count;
                  //Serial.print("Guardado Count:");
                  //Serial.println(contador);
                }
             }else{
              Serial.print("Direccion Equivocada.");
             }
            }
      }else{
        Serial.print("C:"); 
        Serial.println(c); 
      }
  }
} 
