/*
 * @Author: Kingtous
 * @Date: 2020-04-04 21:36:25
 * @LastEditors: Kingtous
 * @LastEditTime: 2020-04-04 23:56:48
 * @Description: Kingtous' Code
 */
#include "wifi_manager.h"

WiFiManager* manager;

// WiFi Connected Callback
void onWiFiConnected(){
  
}

void setup() {
  // put your setup code here, to run once:
  manager = new WiFiManager();
  manager->setWiFiConnectedCallback(&onWiFiConnected);
}

void loop() {
  // put your main code here, to run repeatedly:

}
