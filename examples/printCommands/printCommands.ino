#include <IR_Connect.h>

#define IR_CONNECT_NUMBER "0000"
#define DEVICE_NAME "LED STRIPE"

IR_Connect hub(IR_CONNECT_NUMBER, DEVICE_NAME);

void setup() {
  Serial.begin(115200);
  if (!hub.connect())
    ESP.restart();

    hub.on("on", onEnable);
    hub.on("off", onDisable);
}

void loop() {
  if (!hub.isConnected())
    ESP.restart();

  hub.getCommand();
}

void onEnable() {
  Serial.println("ON");
}

void onDisable() {
  Serial.println("OFF");
}