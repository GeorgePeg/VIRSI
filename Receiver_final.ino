/*
  Name: Receiver ESP 
  Date: 05/09/2025
  Version: 1.1.0
  Description: This is the final code for the receiver. The robot receives the command id from the sender
               and based on the id, it performs the movement (or other feature) we want each time. The 
               robot we are using is a simple car.We are using the ESP32-WROOM-DA Module. 
*/
#include <WiFi.h>
#include <esp_now.h>
#include <Arduino.h>

//Global variables for the circuit
const byte numOfChannels = 4;
const byte channelPins[] = {15, 4, 16, 17};
const byte steeringServoPin = 19;
const byte motorControlPin = 18;
const byte leftLedPin = 21;
const byte rightLedPin = 22;

//Servo variables
Servo steeringServo;
Servo motorServo;
unsigned long pulseWidth[numOfChannels];
bool useEspNow = false; //True -> ESP-NOW Mode, False -> RC Mode
bool espNowLightsOn = false;
unsigned long lastFlashTime = 0;
bool flashState = LOW;
const unsigned long flashInterval = 500; //Duration of flash

//Timed movement variables
unsigned long briefTurnStartTime = 0;
const unsigned long briefTurnDuration = 1000; 
bool isBriefTurning =  false;

//Stop alarms 
bool isStopping = false;
unsigned long stopAlarmStartTime = 0;
const unsigned long stopAlarmDuration = 3000;

//Structure of the ESP-NOW message 
typedef struct struct_message {
  int commandID;
} struct_message;
struct_message incomingMessage;
volatile int lastCommandID = -1;
//Command mapping 
enum Commands {
  COMMAND_OK = 0,
  COMMAND_FORWARD = 1,
  COMMAND_BACKWARD = 2,
  COMMAND_RIGHT = 3,
  COMMAND_LEFT = 4,
  COMMAND_STOP = 5,
  COMMAND_LEDS = 6,
};
//Functions calls
unsigned long readPulseWidth(byte pin);
void handleEspNowCommand(int commandId);
void onDataRecv(const esp_now_recv_info *info, const uint8_t * incomingData, int len);
void performStop();
void performBriefLeftTurn();
void performBriefRightTurn();
void performForward();
void performRevesre();

//Function that reads the PWM pulse width from a given pin
unsigned long readPulseWidth(byte pin) {
  return pulseIn(pin, HIGH, 25000);
}
//Function that makes the robot stop and makes it return back to center. Flash lights show that the car is stopped
void performStop() {
  motorServo.writeMicroseconds(1500);
  steeringServo.writeMicroseconds(1380);
  isStopping = true;
  stopAlarmStartTime = millis();
}
//Function that makes the robot make a brief turn to the left.
void performBriefLeftTurn() {
  steeringServo.writeMicroseconds(1080);
  motorServo.writeMicroseconds(1600);
  delay(1000);
  steeringServo.writeMicroseconds(1380);
  motorServo.writeMicroseconds(1500);
}
//Function that makes the robot make a brief turn to the right
void performBriefRightTurn() {
  steeringServo.writeMicroseconds(1630);
  motorServo.writeMicroseconds(1600);
  delay(1000);
  motorServo.writeMicroseconds(1500);
  steeringMicroseconds(1200);
  delay(200);
  steeringServo.writeMicroseconds(1380);
}
//Function that makes the robot perform a step forward
void performForward() {
  motorServo.writeMicroseconds(1660);
  delay(1000);
  motorServo.writeMicroseconds(1500); // 1500 for us are the 90 degrees for the servo (center)
}
//Function that makes the robot perform a step in reverse
void performRevesre() {
  motorServo.writeMicroseconds(1460);
  delay(1000);
  motorServo.writeMicroseconds(1500);
}
//Function that handles the delivered command ID through ESP-NOW and maps the movements of the car
void handleEspNowCommand(int commandId) {
  //Stop every turns or movements when a new command arrives.
  isBriefTurning = false;
  isStopping = false;
  switch (commandId) {
    case COMMAND_FORWARD: 
      performForward();
      break;
    case COMMAND_BACKWARD:
      performReverse();
      break;
    case COMMAND_STOP:
      performStop();
      break;
    case COMMAND_LEFT: 
      performBriefLeftTurn();
      break;
    case COMMAND_RIGHT: 
      performBriefRightTurn();
      break;
    case COMMAND_LEDS: 
      espNowLightsOn = !espNowLightsOn; //Toggle the light state;
      break;
    default: 
      //If you receive any unknown command, then do nothing
      break;
  }
}
//Callback function to handle the ESP-NOW data
void onDataRecv(const esp_now_recv_info *info, const uint8_t * incomingData, int len) {
  //Check if the received data size matches the message structure 
  if (len == sizeof(incomingMessage)) {
    memcpy(&incomingMessage, incomingData, len);
    lastCommandID = incomingMessage.commandID;
    Serial.print("ESP-NOW Command ID received: ");
    Serial.println(lastCommandID);
    handleEspNowCommand(lastCommandID);
  } else {
    Serial.print("Unknown data length received: "); 
    Serial.println(len);
  }
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //Set up the pins 
  for (byte i=0; i< numOfChannels; i++){
    pinMode(channelPins[i], OUTPUT);
  }
  pinMode(leftLedPin, OUTPUT);
  pinMode(rightLedPin, OUTPUT);
  //Attach the servo pins
  steeringServo.attach(steeringServoPin, 500, 2500);
  motorServo.attach(motorControlPin, 1000, 2000);
  //Set the initial positions of the servo
  steeringServo.write(90); //Center steering
  motorsServo.write(90); //Stop motor
  //ESP-NOW Set up
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.printn("ESP-NOW initialization failed");
  } else {
    Serial.println("ESP-NOW initialization done");
  }
}
void loop() {
  // put your main code here, to run repeatedly:
  //Read the PWM signals from all the channels of the RC controller 
  for (byte i = 0; i < numOfChannels; i++) {
    pulseWidth[i] = readPulseWidth(channelPins[i]);
  }
  //Determine the mode based on the 4th channel
  useEspNow = pulseWidth[3] > 1500;
  //Handle timed brief turns for ESP-NOW Mode
  if (useEspNow && isBriefTurning && (millis() - briefTurnStartTime >= briefTurnDuration)) {
    //Stop the brief turn and return back to center
    isBriefTurning = false;
    performStop();
  }
  //Control the servos based on the mode running (ESP-NOW mode or RC mode)
  if(!useEspNow) {
    //RC mode
    steeringServo.writeMicroseconds(pulseWidth[0]);
    motorServo.writeMicroseconds(pulseWidth[1]);
    lastCommandID = -1; //Reset all commands to avoid accidentaly ESP-NOW control
    //Leds for the RC mode
    bool turningLeft = (pulseWidth[0] < 1090);
    bool turningRight = (pulseWidth[0] > 1620);
    bool areLightsOn = pulseWidth[2] > 1500;

    if (turningLeft || turningRight) {
      unsigned long now = millis();
      if (now - lastFlashTime >= flashInterval) {
        flashState != flashState;
        lastFlashTime = now;
        if (turningLeft) {
          digitalWrite(leftLedPin, flashState ? HIGH : LOW);
          digitalWrite(rightLedPin, areLightsOn ? HIGH : LOW);
        } else {
           digitalWrite(rightLedPin, flashState ? HIGH : LOW);
           digitalWrite(leftLedPin, areLightsOn ? HIGH : LOW);
        }
      }
    } else {
      flashState = LOW;
      digitalWrite(leftLedPin, areLightsOn ? HIGH : LOW);
      digitalWrite(rightLedPin, areLightsOn ? HIGH : LOW);
    }
  }  else {
      //ESP-NOW mode
      //The leds are controlled based on the command ID received.
      bool turningLeftEspNow =  isBriefTurning && (lastCommandID == COMMAND_LEFT);
      bool turningRightEspNow = isBriefTurning && (lastCommandID == COMMAND_RIGHT);
      if (turningLeftEspNow || turningRightEspNow) {
        unsigned long now = millis();
        if (now - lastFlashTime >= flashInterval) {
        flashState != flashState;
        lastFlashTime = now;
        if (turningLeft) {
          digitalWrite(leftLedPin, flashState ? HIGH : LOW);
          digitalWrite(rightLedPin, espNowLightsOn ? HIGH : LOW);
        } else {
           digitalWrite(rightLedPin, flashState ? HIGH : LOW);
           digitalWrite(leftLedPin, espNowLightsOn ? HIGH : LOW);
        }
      } else {
        flashState = LOW;
        digitalWrite(leftLedPin, espNowLightsOn ? HIGH : LOW);
        digitalWrite(rightLedPin, espNowLightsOn ? HIGH : LOW);
      }
    }
  if (isStopping) {
    if(millis() - stopAlarmStartTime >= stopAlarmDuration) {
      isStopping = false;
      digitalWrite(leftLedPin, LOW);
      digitalWrite(rightLedPin, LOW);
    } else {
      unsigned long now = millis();
      if (now - lastFlashTime >= flashInterval) {
        flashState = !flashState;
        lastFlashTime = now;
        digitalWrite(leftLedPin, areLightsOn ? HIGH : LOW);
        digitalWrite(rightLedPin, areLightsOn ? HIGH : LOW);
      }
    }
  }
}
