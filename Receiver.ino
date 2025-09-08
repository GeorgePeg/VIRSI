/*
  Name: Receiver ESP
  Date: 17/07/2025
  Description: Here is the code for receiver esp32 board of the project.
*/
#include <WiFi.h>
#include <esp_now.h>

//Callout function for receiving a message
void dataReceive(const esp_now_recv_info_t *recvInfo, const uint8_t * inData, int len) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr),
  "%02X:%02X:%02X:%02X:%02X:%02X",
  recvInfo->src_addr[0],recvInfo->src_addr[1],recvInfo->src_addr[2],recvInfo->src_addr[3],
  recvInfo->src_addr[4],recvInfo->src_addr[5]);
  char msg[len+1];
  memcpy(msg, inData, len);
  msg[len] = '\0';
  Serial.print("Received message from ");
  Serial.print(macStr);
  Serial.print(" -> ");
  Serial.println(msg);
}
void setup() {
  // put your setup code here, to run once:
  //Starting serial communication
  Serial.begin(115200); 
  //STA mode 
  WiFi.mode(WIFI_STA);
  //Disable automatic connection to AP
  WiFi.disconnect();

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    while(true) {
      delay(1000);
    }
  }
  //Registration of the callback
  esp_now_register_recv_cb(dataReceive);
  Serial.println("Receiver ready");

}
void loop() {
  // put your main code here, to run repeatedly:

}
