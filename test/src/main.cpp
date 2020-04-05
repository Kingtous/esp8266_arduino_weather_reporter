/*
 * @Author: Kingtous
 * @Date: 2020-04-05 08:03:27
 * @LastEditors: Kingtous
 * @LastEditTime: 2020-04-05 19:04:49
 * @Description: Kingtous' Code
 */
#include <Arduino.h>
#include <FS.h>
#include "wifi_manager.h"
#define BAUD_RATE 115200
#define PIN_LED 2
WiFiManager* manager;

// WiFi Connected Callback
void onWiFiConnected(){
  Serial.println("wifi connected");
  digitalWrite(PIN_LED, LOW);
}

// init Serial Baud Rate to 115200
void initSerial(){
  Serial.begin(BAUD_RATE);
  Serial.println("NodeMCU Node(Kingtous)");
}

void initFileSystem(){
  SPIFFS.begin();
  EEPROM.begin(512);
}

void setup() {
  // init LED 
  pinMode(PIN_LED, OUTPUT);
  //
  initSerial();
  initFileSystem();
  // put your setup code here, to run once:
  manager = new WiFiManager();
  manager->setWiFiConnectedCallback(&onWiFiConnected);
  manager->init();
}

void loop() {
  // put your main code here, to run repeatedly:
  manager->handleServerClient();
}
