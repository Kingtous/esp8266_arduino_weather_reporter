/*
 * @Author: Kingtous
 * @Date: 2020-04-04 22:08:03
 * @LastEditors: Kingtous
 * @LastEditTime: 2020-04-05 18:19:24
 * @Description: Kingtous' Code
 */
#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H
typedef void (*Callback)();
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <EEPROM.h>

#define AP_PORT 80

class WiFiManager
{
public:
    WiFiManager();

    void init();
    void setWiFiConnectedCallback(Callback callback);
    void openAP();
    bool closeAP();
    void connectWiFi();
    void udpListen(Callback callback);
    void stopUdpListen();
    void handleServerClient();

private:
    WiFiClient *client = nullptr;
    ESP8266WebServer *server = nullptr;
    WiFiUDP *udpServer = nullptr;
    Callback onWiFiConnectedCallback = nullptr;

    void startWebServer();
    bool testWiFi(); // if connected, return True
};

#endif