#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  delay(500);

  WiFi.mode(WIFI_STA); // STA mode για MAC
  Serial.println("Πάτησε '1' και πάτα Enter για να δεις τη MAC address");
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    if (c == '1') {
      Serial.print("MAC Address (STA): ");
      Serial.println(WiFi.macAddress());
    }
  }
}
