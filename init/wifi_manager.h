/*
 * @Author: Kingtous
 * @Date: 2020-04-04 22:08:03
 * @LastEditors: Kingtous
 * @LastEditTime: 2020-04-04 23:31:52
 * @Description: Kingtous' Code
 */
#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H
typedef void (*Callback)();
#include <ESP8266WiFi.h>

class WiFiManager
{
public:
    WiFiManager();
    // {
    //     this->client = WiFiClient();
    // }
    void setWiFiConnectedCallback(Callback callback);
    // {
    //     this->onWiFiConnectedCallback = callback;
    // }

private:
    WiFiClient *client;
    Callback onWiFiConnectedCallback;
};

#endif