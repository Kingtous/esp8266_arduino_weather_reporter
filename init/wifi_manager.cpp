/*
 * @Author: Kingtous
 * @Date: 2020-04-04 22:08:24
 * @LastEditors: Kingtous
 * @LastEditTime: 2020-04-04 23:32:35
 * @Description: Kingtous' Code
 */

#include "wifi_manager.h"

WiFiManager::WiFiManager(){
    this->client = new WiFiClient();
}

void WiFiManager::setWiFiConnectedCallback(Callback callback){
    this->onWiFiConnectedCallback = callback;
}
