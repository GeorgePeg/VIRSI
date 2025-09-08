/*
  Name: Sender ESP with voice commands for the VRM
  Date: 28/08/2025
  Version: 1.0.1
  Description: The code for the sender esp
*/
#include <WiFi.h>
#include <esp_now.h>
#include <Arduino.h>
#include <VoiceRecognitionV3_ESP32.h>

//Tx & Rx Pins of VRM3
#define VR_RX_PIN 16
#define VR_TX_PIN 17  

VR myVR(VR_RX_PIN, VR_TX_PIN); //VR object
uint8_t buf[64]; //buffer 64 Bytes
uint8_t receiverMac[] = {0x88, 0x13, 0xBF, 0x00, 0x61, 0x14};

//Message structure 
typedef struct {
  int commandId;
} CommandMessage; 
CommandMessage msg;

//Trained commands and their IDs
uint8_t records[] = {1, 2, 3, 4, 5, 6, 7};
//Recognition Mode 
bool recognitionMode = true;
//Callback function for sending the message
void onSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Send status is: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}


//Broadcast MAC Address
//uint8_t broadcastAddr[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);
  //ESP-NOW setup
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  //First check
  if(esp_now_init() != ESP_OK) {
    Serial.println("Error during initialization.");
    return;
  }
  //Inclusion of the peer (broadcast)
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr,receiverMac,6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  //Final check
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  Serial.println("Sender is ready.");

  //VRM Setup 
  myVR.begin(9600);
  int ret =  myVR.load(records, sizeof(records), buf);
  if (ret >= 0) {
    Serial.println("Voice Commands loaded into VRM3!");
  } else {
    Serial.printf("Load failed! code=%d\n", ret);
  }
  Serial.println("Ready to identify commands...");

}

void loop() {
  // put your main code here, to run repeatedly:
  if(recognitionMode) {
    int ret = myVR.recognize(buf,50); 
    if(ret > 0) {
      int word_id = buf[1];
      msg.commandId = word_id;
      Serial.print("Recognized command ID: ");
      Serial.println(word_id);
      esp_err_t result = esp_now_send(receiverMac, (uint8_t *)&msg, sizeof(msg));
      delay(300);
      if(result == ESP_OK) {
        Serial.println("Message sended succesfully");
      }
      else {
        Serial.print("Error sending: ");
        Serial.println(result);
      }
    }
  }
}
