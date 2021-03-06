// Copyright (c) 2020 Kingtous
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

/*
 * @Author: Kingtous
 * @Date: 2020-04-04 22:08:24
 * @LastEditors: Kingtous
 * @LastEditTime: 2020-04-07 20:58:06
 * @Description: Kingtous' Code
 */

#include "wifi_manager.h"

WiFiManager::WiFiManager()
{
    this->server = new ESP8266WebServer(AP_PORT);
    this->udpServer = new WiFiUDP();
}

void WiFiManager::handleServerClient(){
    if (this->server != nullptr)
    {
        this->server->handleClient();
    }
}

void WiFiManager::setWiFiConnectedCallback(Callback callback)
{
    this->onWiFiConnectedCallback = callback;
}

bool WiFiManager::closeAP()
{
    WiFi.mode(WIFI_STA);
    return WiFi.softAPdisconnect();
}

void WiFiManager::openAP()
{
    Serial.println("AP: Opening AP mode.");
    WiFi.mode(WIFI_AP);
    while (!WiFi.softAP("ESP8266_CONFIG", "kingtous"))
    {
        Serial.println("AP:AP Create Failed.");
        delay(5000);
    }
    startWebServer();
}

void WiFiManager::setWiFiConnectFailedCallback(Callback callback){
    this->onWiFiConnectFailedCallback = callback;
}

void WiFiManager::init()
{
    this->connectWiFi();
}

void WiFiManager::startWebServer()
{
    if (this->server != nullptr)
    {
        // home page
        this->server->on("/", [this]() {
            String htmlContent;
            int statusCode;
            Serial.println("HandleClient on /");
            IPAddress ip = WiFi.softAPIP();
            String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
            htmlContent = "<!DOCTYPE HTML>\r\n<html>Hello from ESP8266 at ";
            htmlContent += ipStr;
            htmlContent += "<p><form method='get' action='setWiFi'><label>SSID: </label><input name='s' length=32><label>Password:</label><input name='p' length=64><input type='submit'></form></p>";
            htmlContent += "<p><br>Weather Setting:</br><form method='get' action='setWeather'><label>City Code (9 numbers): </label><input name='w' length=9><input type='submit'></form></p>";
            htmlContent += "<a href=\"https://github.com/baichengzhou/weather.api/blob/master/src/main/resources/citycode-2019-08-23.json\">View All China Region City Code</a>";
            htmlContent += "</html>";
            statusCode = 200;
            this->server->send(statusCode, "text/html", htmlContent);
        });
        // set up WLAN Page
        this->server->on("/setWiFi", [this]() {
            String htmlContent;
            int statusCode;
            Serial.println("HandleClient on /setWiFi");
            String ssid = this->server->arg("s");
            String password = this->server->arg("p");
            if (ssid.length() != 0)
            {
                Serial.println("clearing eeprom");
                for (int i = 0; i < 128; ++i)
                {
                    EEPROM.write(i, 0);
                }
                Serial.println(ssid);
                Serial.println("");
                Serial.println(password);
                Serial.println("");
                Serial.println("writing eeprom ssid:");
                unsigned int ssid_length = ssid.length();
                unsigned int password_length = password.length();
                for (unsigned int i = 0; i < ssid_length; ++i)
                {
                    EEPROM.write(i, ssid[i]);
                    // Serial.print("Wrote: ");
                    // Serial.println(ssid[i]);
                }
                Serial.println("writing eeprom pass:");
                for (unsigned int i = 0; i < password_length; ++i)
                {
                    EEPROM.write(32 + i, password[i]);
                    // Serial.print("Wrote: ");
                    // Serial.println(password[i]);
                }
                EEPROM.commit();
                htmlContent = "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}";
                statusCode = 200;
            }
            else
            {
                htmlContent = "{\"Error\":\"404 not found\"}";
                statusCode = 404;
                Serial.println("Sending 404");
            }
            server->send(statusCode, "application/json", htmlContent);
            delay(500);
            // close AP mode
            if (this->closeAP())
            {
                this->connectWiFi();
            }
        });
        this->server->on("/setWeather", [this]() {
            String code = this->server->arg("w");
            Serial.print("Get City Code:");
            Serial.println(code);
            if (code.length() != 9)
            {
                this->server->send(404, "application/json", "{\"Error\":404}");
            } else {
                // 写入
                for (int i = 0; i < 9; i++)
                {
                    Serial.println("write a bit");
                    EEPROM.write(96 + i, code[i]);
                }
                EEPROM.commit();
                this->server->send(200, "application/json", "{\"Success\":200}");
            }
        });
        this->server->begin();
    }
}

bool WiFiManager::wifiConnected(){
    return WiFi.status() == WL_CONNECTED;
}

void WiFiManager::connectWiFi()
{
    // read SSID
    Serial.println("Reading EEPROM ssid");
    String esid;
    for (int i = 0; i < 32; ++i)
    {
        esid += char(EEPROM.read(i));
    }
    Serial.print("SSID: ");
    Serial.println(esid);
    Serial.println("Reading EEPROM pass");
    String epass = "";
    for (int i = 32; i < 96; ++i)
    {
        epass += char(EEPROM.read(i));
    }
    if (esid.length() > 1)
    {
        WiFi.begin(esid, epass);
    }
    if (testWiFi())
    {
        if (this->onWiFiConnectedCallback != nullptr)
        {
            this->onWiFiConnectedCallback();
        }
    }
    else
    {
        this->onWiFiConnectFailedCallback();
        openAP();
    }
}

bool WiFiManager::testWiFi()
{
    int c = 0;
    Serial.println("Waiting for WiFi to connect");
    while (c < 20)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            Serial.print("current IP:");
            Serial.println(WiFi.localIP());
            return true;
        }
        digitalWrite(2, LOW);
        delay(500);
        digitalWrite(2, HIGH);
        delay(500);
        Serial.println(WiFi.status());
        c++;
    }
    Serial.println("");
    Serial.println("Connect timed out");
    return false;
}


void udpListen(Callback callback){

}

void stopUdpListen(){

}