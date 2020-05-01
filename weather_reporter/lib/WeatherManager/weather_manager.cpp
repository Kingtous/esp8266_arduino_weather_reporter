// Copyright (c) 2020 Kingtous
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

/*
 * @Author: Kingtous
 * @Date: 2020-04-06 09:36:24
 * @LastEditors: Kingtous
 * @LastEditTime: 2020-05-01 21:42:56
 * @Description: Kingtous' Code
 */

#include "weather_manager.h"

WeatherManager::WeatherManager(/* args */)
{
    this->client = new WiFiClient();
}

Weather WeatherManager::getWeather()
{
    // read
    Serial.print("citycode:");
    String cityCode = "";
    for (int i = 0; i < 9; i++)
    {
        int value = char(EEPROM.read(96 + i));
        Serial.print(value);
        if (value == 0)
        {
            // 没有编码
            return Weather();
        }
        cityCode += char(value);
    }
    // request
    Serial.println("fetching data from " + weather_base_url + weather_api + cityCode + " at port 80");
    fetch:
    if (this->client->connect(weather_base_url, 80))
    {
        Serial.println("weather server connect success.");
        client->print(String("GET ") + weather_api + cityCode + " HTTP/1.1\r\n" +
                      "Host: " + weather_base_url + "\r\n" +
                      "Connection: close\r\n" +
                      "\r\n");
        delay(100);
        String content = "";
        while (client->available()) 
        {
            content += client->readStringUntil('\r'); //按行读取数据
        }
        client->stop(); //关闭当前连接
        if (content.length() == 0)
        {
            goto fetch;
        }
        // start parse Json
        int jsonIndex = content.indexOf("{");
        content = content.substring(jsonIndex);
        const size_t capacity = JSON_ARRAY_SIZE(15) + JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(8) + 9 * JSON_OBJECT_SIZE(11) + 7 * JSON_OBJECT_SIZE(12) + 2480;
        DynamicJsonDocument doc(capacity);

        const char *json = content.c_str();

        deserializeJson(doc, json);

        Weather weather;

        const char *message = doc["message"]; // "success感谢又拍云(upyun.com)提供CDN赞助"
        int status = doc["status"];           // 200
        const char *date = doc["date"];       // "20200406"
        const char *time = doc["time"];       // "2020-04-06 16:13:20"

        JsonObject cityInfo = doc["cityInfo"];
        const char *cityInfo_city = cityInfo["city"];             // "City"
        const char *cityInfo_citykey = cityInfo["citykey"];       // "City Code"
        const char *cityInfo_parent = cityInfo["parent"];         // "Province"
        const char *cityInfo_updateTime = cityInfo["updateTime"]; // "15:21"

        JsonObject data = doc["data"];
        const char *data_shidu = data["shidu"];     // "42%"
        int data_pm25 = data["pm25"];               // 32
        int data_pm10 = data["pm10"];               // 54
        const char *data_quality = data["quality"]; // "良"
        const char *data_wendu = data["wendu"];     // "20"
        const char *data_ganmao = data["ganmao"];   // "极少数敏感人群应减少户外活动"

        JsonArray data_forecast = data["forecast"];
        weather.data = data_forecast;

        JsonObject data_forecast_0 = data_forecast[0];
        const char *data_forecast_0_date = data_forecast_0["date"];       // "06"
        const char *data_forecast_0_high = data_forecast_0["high"];       // "高温 21℃"
        const char *data_forecast_0_low = data_forecast_0["low"];         // "低温 11℃"
        const char *data_forecast_0_ymd = data_forecast_0["ymd"];         // "2020-04-06"
        const char *data_forecast_0_week = data_forecast_0["week"];       // "星期一"
        const char *data_forecast_0_sunrise = data_forecast_0["sunrise"]; // "06:09"
        const char *data_forecast_0_sunset = data_forecast_0["sunset"];   // "18:43"
        int data_forecast_0_aqi = data_forecast_0["aqi"];                 // 72
        const char *data_forecast_0_fx = data_forecast_0["fx"];           // "东北风"
        const char *data_forecast_0_fl = data_forecast_0["fl"];           // "<3级"
        const char *data_forecast_0_type = data_forecast_0["type"];       // "多云"
        const char *data_forecast_0_notice = data_forecast_0["notice"];   // "阴晴之间，谨防紫外线侵扰"
        weather.cityInfoCity = String(cityInfo_city);
        weather.firstPage = weather.cityInfoCity + "\n" + String(cityInfo_updateTime) + "\n" + String(data_forecast_0_low) + " " + String(data_forecast_0_high) + "\n" + String(data_forecast_0_type);

        return weather;
    }
    else
    {
        goto fetch;
    }
}