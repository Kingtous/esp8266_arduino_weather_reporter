// Copyright (c) 2020 Kingtous
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

/*
 * @Author: Kingtous
 * @Date: 2020-04-06 09:35:59
 * @LastEditors: Kingtous
 * @LastEditTime: 2020-04-06 17:04:42
 * @Description: Kingtous' Code
 */

#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

const String weather_base_url = "t.weather.sojson.com";
const String weather_api = "/api/weather/city/";

class WeatherManager
{
private:
    WiFiClient *client = nullptr;
public:
    WeatherManager(/* args */);
    String getWeather();
};

