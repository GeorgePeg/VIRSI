/*
  Name: Sender ESP 
  Date: 05/09/2025
  Version: 1.1.0
  Description: This is the final code for the sender. Every time the VRM recognises a voice command, then a beep-beep sound
               is playing and can be heared throug the headphones. We use it as a confirmation effect for the success-
               full delivery of the voice command to the receiver. For the sender we use the ESP32-S3.
*/
#include <WiFi.h>
#include <esp_now.h>
#include <Arduino.h>
#include <VoiceRecognitionV3_ESP32.h>

//Tx & Rx pins of VRM
#define VR_TX_PIN 18
#define VR_RX_PIN 17

//Buzzer pin
#define BUZZER_PIN 2

//VR object
VR myVR(VR_RX_PIN,VR_TX_PIN);
uint8_t buf[64]; //64 bytes buffer 
uint8_t receiverMac[] = {0x88, 0x13, 0xBF, 0x00, 0x61, 0x14}; //MAC address of the receiver

//Message structure 
typedef struct {
  int commandId;
} CommandMessage;
CommandMessage msg;

//Trained commands (For us the commands are 7 -> 0-6)
uint8_t records[] = {0, 1, 2, 3, 4, 5, 6};
//Recognition mode variable
bool recognitionMode = true;

//Function to play the beep-beep sound
void playBeepBeep() {
  tone(BUZZER_PIN, 1000, 100); //Play the first beep at 1000Hz for 100ms
  delay(150); //Small delay if 150 ms between the two beeps
  tone(BUZZER_PIN, 1000, 100); //Play the second beep at 1000Hz for 100ms
  delay(150); //Small delay if 150 ms between the two beeps
}
//Function to see if everything is ok with the ESP-NOW and if the the message sended succesfully
void onSent(const wifi_tx_info_t *tx_info, esp_now_send_status_t status) {
  Serial.printl("Sender status :");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ?? "Success" : "Fail"); 
}
void setup() {
  // put your setup code here, to run once:
  //Initializing the serial communication for the ESP-NOW
  Serial.begin(115200);
  delay(1000);

  //Initialization of the buzzer pin
  pinMode(BUZZER_PIN, OUTPUT);

  //ESP-NOW Setup
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP_NOW initialization failed!");
    return;
  }
  //Register the callback function
  esp_now_register_send_cb(onSent);
  //Inclusion of the peer (the receiver device)
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMac, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  //Check for the connection of the peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Error during peer connection! Failed to add peer!");
    return;
  }
  Serial.println("Sender is ready!");

  //VRM Setup 
  myVR.begin(9600);
  //Loading the records 
  int ret = myVR.load(records, sizeof(records), buf);
  if (ret >= 0) {
   Serial.println("Voice commands have been loaded!");
  }
  else {
    Serial.printf("Load of voice command %d failed", ret);
  }
  Serial.println("Ready to identify commands...");
}

void loop() {
  // put your main code here, to run repeatedly:

  if(recogntitionMode) {
    int ret = myVR.recognize(buf, 50);
    if (ret > 0) {
      //Set the second element of the buffer as the command id
      int word_id = buf[1];
      msg.commandId = word_id;
      Serial.println("Recognized command with ID: ");
      Serial.println(word_id);
      esp_now_send(receiverMac, (uint8_t *)&msg, sizeof(msg)); //Send the command to the receiver device
      //Play the beep-beep sound for confirmation
      playBeepBeep();
      delay(300); //Small delay for stability
    }
  }
}
