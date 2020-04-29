// Copyright (c) 2020 Kingtous
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

/*
 * @Author: Kingtous
 * @Date: 2020-04-06 09:35:59
 * @LastEditors: Kingtous
 * @LastEditTime: 2020-04-29 18:54:54
 * @Description: Kingtous' Code
 */

#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

const String weather_base_url = "t.weather.sojson.com";
const String weather_api = "/api/weather/city/";


typedef struct Weather
{
    String cityInfoCity;
    String firstPage;
    JsonArray data; // 15天
}Weather;


class WeatherManager
{
private:
    WiFiClient *client = nullptr;
public:
    WeatherManager(/* args */);
    Weather getWeather();
};

