/*
 * @Author: Kingtous
 * @Date: 2020-04-06 09:36:24
 * @LastEditors: Kingtous
 * @LastEditTime: 2020-04-06 19:37:03
 * @Description: Kingtous' Code
 */

#include "weather_manager.h"

WeatherManager::WeatherManager(/* args */)
{
    this->client = new WiFiClient();
}

String WeatherManager::getWeather()
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
            return "";
        }
        cityCode += char(value);
    }
    // request
    Serial.println("fetching data from " + weather_base_url + weather_api + cityCode + " at port 80");
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
        Serial.println("get data:" + content);
        // start parse Json
        int jsonIndex = content.indexOf("{");
        content = content.substring(jsonIndex);
        const size_t capacity = JSON_ARRAY_SIZE(15) + JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(8) + 9 * JSON_OBJECT_SIZE(11) + 7 * JSON_OBJECT_SIZE(12) + 2480;
        DynamicJsonDocument doc(capacity);

        const char *json = content.c_str();

        deserializeJson(doc, json);

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

        String weather = String(cityInfo_city) + "\n" + String(cityInfo_updateTime) + "\n" + String(data_forecast_0_low) +" "+String(data_forecast_0_high) + "\n" + String(data_forecast_0_type);

        JsonObject data_forecast_1 = data_forecast[1];
        const char *data_forecast_1_date = data_forecast_1["date"];       // "07"
        const char *data_forecast_1_high = data_forecast_1["high"];       // "高温 22℃"
        const char *data_forecast_1_low = data_forecast_1["low"];         // "低温 10℃"
        const char *data_forecast_1_ymd = data_forecast_1["ymd"];         // "2020-04-07"
        const char *data_forecast_1_week = data_forecast_1["week"];       // "星期二"
        const char *data_forecast_1_sunrise = data_forecast_1["sunrise"]; // "06:08"
        const char *data_forecast_1_sunset = data_forecast_1["sunset"];   // "18:43"
        int data_forecast_1_aqi = data_forecast_1["aqi"];                 // 66
        const char *data_forecast_1_fx = data_forecast_1["fx"];           // "西南风"
        const char *data_forecast_1_fl = data_forecast_1["fl"];           // "<3级"
        const char *data_forecast_1_type = data_forecast_1["type"];       // "多云"
        const char *data_forecast_1_notice = data_forecast_1["notice"];   // "阴晴之间，谨防紫外线侵扰"

        JsonObject data_forecast_2 = data_forecast[2];
        const char *data_forecast_2_date = data_forecast_2["date"];       // "08"
        const char *data_forecast_2_high = data_forecast_2["high"];       // "高温 22℃"
        const char *data_forecast_2_low = data_forecast_2["low"];         // "低温 11℃"
        const char *data_forecast_2_ymd = data_forecast_2["ymd"];         // "2020-04-08"
        const char *data_forecast_2_week = data_forecast_2["week"];       // "星期三"
        const char *data_forecast_2_sunrise = data_forecast_2["sunrise"]; // "06:06"
        const char *data_forecast_2_sunset = data_forecast_2["sunset"];   // "18:44"
        int data_forecast_2_aqi = data_forecast_2["aqi"];                 // 47
        const char *data_forecast_2_fx = data_forecast_2["fx"];           // "西南风"
        const char *data_forecast_2_fl = data_forecast_2["fl"];           // "<3级"
        const char *data_forecast_2_type = data_forecast_2["type"];       // "多云"
        const char *data_forecast_2_notice = data_forecast_2["notice"];   // "阴晴之间，谨防紫外线侵扰"

        JsonObject data_forecast_3 = data_forecast[3];
        const char *data_forecast_3_date = data_forecast_3["date"];       // "09"
        const char *data_forecast_3_high = data_forecast_3["high"];       // "高温 24℃"
        const char *data_forecast_3_low = data_forecast_3["low"];         // "低温 12℃"
        const char *data_forecast_3_ymd = data_forecast_3["ymd"];         // "2020-04-09"
        const char *data_forecast_3_week = data_forecast_3["week"];       // "星期四"
        const char *data_forecast_3_sunrise = data_forecast_3["sunrise"]; // "06:05"
        const char *data_forecast_3_sunset = data_forecast_3["sunset"];   // "18:44"
        int data_forecast_3_aqi = data_forecast_3["aqi"];                 // 51
        const char *data_forecast_3_fx = data_forecast_3["fx"];           // "东北风"
        const char *data_forecast_3_fl = data_forecast_3["fl"];           // "<3级"
        const char *data_forecast_3_type = data_forecast_3["type"];       // "多云"
        const char *data_forecast_3_notice = data_forecast_3["notice"];   // "阴晴之间，谨防紫外线侵扰"

        JsonObject data_forecast_4 = data_forecast[4];
        const char *data_forecast_4_date = data_forecast_4["date"];       // "10"
        const char *data_forecast_4_high = data_forecast_4["high"];       // "高温 25℃"
        const char *data_forecast_4_low = data_forecast_4["low"];         // "低温 15℃"
        const char *data_forecast_4_ymd = data_forecast_4["ymd"];         // "2020-04-10"
        const char *data_forecast_4_week = data_forecast_4["week"];       // "星期五"
        const char *data_forecast_4_sunrise = data_forecast_4["sunrise"]; // "06:04"
        const char *data_forecast_4_sunset = data_forecast_4["sunset"];   // "18:45"
        int data_forecast_4_aqi = data_forecast_4["aqi"];                 // 52
        const char *data_forecast_4_fx = data_forecast_4["fx"];           // "东南风"
        const char *data_forecast_4_fl = data_forecast_4["fl"];           // "3-4级"
        const char *data_forecast_4_type = data_forecast_4["type"];       // "多云"
        const char *data_forecast_4_notice = data_forecast_4["notice"];   // "阴晴之间，谨防紫外线侵扰"

        JsonObject data_forecast_5 = data_forecast[5];
        const char *data_forecast_5_date = data_forecast_5["date"];       // "11"
        const char *data_forecast_5_high = data_forecast_5["high"];       // "高温 20℃"
        const char *data_forecast_5_low = data_forecast_5["low"];         // "低温 12℃"
        const char *data_forecast_5_ymd = data_forecast_5["ymd"];         // "2020-04-11"
        const char *data_forecast_5_week = data_forecast_5["week"];       // "星期六"
        const char *data_forecast_5_sunrise = data_forecast_5["sunrise"]; // "06:03"
        const char *data_forecast_5_sunset = data_forecast_5["sunset"];   // "18:45"
        int data_forecast_5_aqi = data_forecast_5["aqi"];                 // 50
        const char *data_forecast_5_fx = data_forecast_5["fx"];           // "西风"
        const char *data_forecast_5_fl = data_forecast_5["fl"];           // "3-4级"
        const char *data_forecast_5_type = data_forecast_5["type"];       // "中雨"
        const char *data_forecast_5_notice = data_forecast_5["notice"];   // "记得随身携带雨伞哦"

        JsonObject data_forecast_6 = data_forecast[6];
        const char *data_forecast_6_date = data_forecast_6["date"];       // "12"
        const char *data_forecast_6_high = data_forecast_6["high"];       // "高温 18℃"
        const char *data_forecast_6_low = data_forecast_6["low"];         // "低温 9℃"
        const char *data_forecast_6_ymd = data_forecast_6["ymd"];         // "2020-04-12"
        const char *data_forecast_6_week = data_forecast_6["week"];       // "星期日"
        const char *data_forecast_6_sunrise = data_forecast_6["sunrise"]; // "06:02"
        const char *data_forecast_6_sunset = data_forecast_6["sunset"];   // "18:46"
        const char *data_forecast_6_fx = data_forecast_6["fx"];           // "东北风"
        const char *data_forecast_6_fl = data_forecast_6["fl"];           // "3-4级"
        const char *data_forecast_6_type = data_forecast_6["type"];       // "晴"
        const char *data_forecast_6_notice = data_forecast_6["notice"];   // "愿你拥有比阳光明媚的心情"

        JsonObject data_forecast_7 = data_forecast[7];
        const char *data_forecast_7_date = data_forecast_7["date"];       // "13"
        const char *data_forecast_7_high = data_forecast_7["high"];       // "高温 19℃"
        const char *data_forecast_7_low = data_forecast_7["low"];         // "低温 10℃"
        const char *data_forecast_7_ymd = data_forecast_7["ymd"];         // "2020-04-13"
        const char *data_forecast_7_week = data_forecast_7["week"];       // "星期一"
        const char *data_forecast_7_sunrise = data_forecast_7["sunrise"]; // "06:01"
        const char *data_forecast_7_sunset = data_forecast_7["sunset"];   // "18:46"
        const char *data_forecast_7_fx = data_forecast_7["fx"];           // "东风"
        const char *data_forecast_7_fl = data_forecast_7["fl"];           // "<3级"
        const char *data_forecast_7_type = data_forecast_7["type"];       // "小雨"
        const char *data_forecast_7_notice = data_forecast_7["notice"];   // "雨虽小，注意保暖别感冒"

        JsonObject data_forecast_8 = data_forecast[8];
        const char *data_forecast_8_date = data_forecast_8["date"];       // "14"
        const char *data_forecast_8_high = data_forecast_8["high"];       // "高温 22℃"
        const char *data_forecast_8_low = data_forecast_8["low"];         // "低温 11℃"
        const char *data_forecast_8_ymd = data_forecast_8["ymd"];         // "2020-04-14"
        const char *data_forecast_8_week = data_forecast_8["week"];       // "星期二"
        const char *data_forecast_8_sunrise = data_forecast_8["sunrise"]; // "06:00"
        const char *data_forecast_8_sunset = data_forecast_8["sunset"];   // "18:47"
        const char *data_forecast_8_fx = data_forecast_8["fx"];           // "西南风"
        const char *data_forecast_8_fl = data_forecast_8["fl"];           // "<3级"
        const char *data_forecast_8_type = data_forecast_8["type"];       // "阴"
        const char *data_forecast_8_notice = data_forecast_8["notice"];   // "不要被阴云遮挡住好心情"

        JsonObject data_forecast_9 = data_forecast[9];
        const char *data_forecast_9_date = data_forecast_9["date"];       // "15"
        const char *data_forecast_9_high = data_forecast_9["high"];       // "高温 25℃"
        const char *data_forecast_9_low = data_forecast_9["low"];         // "低温 16℃"
        const char *data_forecast_9_ymd = data_forecast_9["ymd"];         // "2020-04-15"
        const char *data_forecast_9_week = data_forecast_9["week"];       // "星期三"
        const char *data_forecast_9_sunrise = data_forecast_9["sunrise"]; // "05:59"
        const char *data_forecast_9_sunset = data_forecast_9["sunset"];   // "18:47"
        const char *data_forecast_9_fx = data_forecast_9["fx"];           // "西南风"
        const char *data_forecast_9_fl = data_forecast_9["fl"];           // "<3级"
        const char *data_forecast_9_type = data_forecast_9["type"];       // "晴"
        const char *data_forecast_9_notice = data_forecast_9["notice"];   // "愿你拥有比阳光明媚的心情"

        JsonObject data_forecast_10 = data_forecast[10];
        const char *data_forecast_10_date = data_forecast_10["date"];       // "16"
        const char *data_forecast_10_high = data_forecast_10["high"];       // "高温 28℃"
        const char *data_forecast_10_low = data_forecast_10["low"];         // "低温 18℃"
        const char *data_forecast_10_ymd = data_forecast_10["ymd"];         // "2020-04-16"
        const char *data_forecast_10_week = data_forecast_10["week"];       // "星期四"
        const char *data_forecast_10_sunrise = data_forecast_10["sunrise"]; // "05:58"
        const char *data_forecast_10_sunset = data_forecast_10["sunset"];   // "18:48"
        const char *data_forecast_10_fx = data_forecast_10["fx"];           // "东北风"
        const char *data_forecast_10_fl = data_forecast_10["fl"];           // "<3级"
        const char *data_forecast_10_type = data_forecast_10["type"];       // "小雨"
        const char *data_forecast_10_notice = data_forecast_10["notice"];   // "雨虽小，注意保暖别感冒"

        JsonObject data_forecast_11 = data_forecast[11];
        const char *data_forecast_11_date = data_forecast_11["date"];       // "17"
        const char *data_forecast_11_high = data_forecast_11["high"];       // "高温 29℃"
        const char *data_forecast_11_low = data_forecast_11["low"];         // "低温 16℃"
        const char *data_forecast_11_ymd = data_forecast_11["ymd"];         // "2020-04-17"
        const char *data_forecast_11_week = data_forecast_11["week"];       // "星期五"
        const char *data_forecast_11_sunrise = data_forecast_11["sunrise"]; // "05:57"
        const char *data_forecast_11_sunset = data_forecast_11["sunset"];   // "18:48"
        const char *data_forecast_11_fx = data_forecast_11["fx"];           // "东北风"
        const char *data_forecast_11_fl = data_forecast_11["fl"];           // "<3级"
        const char *data_forecast_11_type = data_forecast_11["type"];       // "小雨"
        const char *data_forecast_11_notice = data_forecast_11["notice"];   // "雨虽小，注意保暖别感冒"

        JsonObject data_forecast_12 = data_forecast[12];
        const char *data_forecast_12_date = data_forecast_12["date"];       // "18"
        const char *data_forecast_12_high = data_forecast_12["high"];       // "高温 23℃"
        const char *data_forecast_12_low = data_forecast_12["low"];         // "低温 14℃"
        const char *data_forecast_12_ymd = data_forecast_12["ymd"];         // "2020-04-18"
        const char *data_forecast_12_week = data_forecast_12["week"];       // "星期六"
        const char *data_forecast_12_sunrise = data_forecast_12["sunrise"]; // "05:56"
        const char *data_forecast_12_sunset = data_forecast_12["sunset"];   // "18:49"
        const char *data_forecast_12_fx = data_forecast_12["fx"];           // "东北风"
        const char *data_forecast_12_fl = data_forecast_12["fl"];           // "<3级"
        const char *data_forecast_12_type = data_forecast_12["type"];       // "小雨"
        const char *data_forecast_12_notice = data_forecast_12["notice"];   // "雨虽小，注意保暖别感冒"

        JsonObject data_forecast_13 = data_forecast[13];
        const char *data_forecast_13_date = data_forecast_13["date"];       // "19"
        const char *data_forecast_13_high = data_forecast_13["high"];       // "高温 18℃"
        const char *data_forecast_13_low = data_forecast_13["low"];         // "低温 14℃"
        const char *data_forecast_13_ymd = data_forecast_13["ymd"];         // "2020-04-19"
        const char *data_forecast_13_week = data_forecast_13["week"];       // "星期日"
        const char *data_forecast_13_sunrise = data_forecast_13["sunrise"]; // "05:55"
        const char *data_forecast_13_sunset = data_forecast_13["sunset"];   // "18:50"
        const char *data_forecast_13_fx = data_forecast_13["fx"];           // "东风"
        const char *data_forecast_13_fl = data_forecast_13["fl"];           // "<3级"
        const char *data_forecast_13_type = data_forecast_13["type"];       // "小雨"
        const char *data_forecast_13_notice = data_forecast_13["notice"];   // "雨虽小，注意保暖别感冒"

        JsonObject data_forecast_14 = data_forecast[14];
        const char *data_forecast_14_date = data_forecast_14["date"];       // "20"
        const char *data_forecast_14_high = data_forecast_14["high"];       // "高温 15℃"
        const char *data_forecast_14_low = data_forecast_14["low"];         // "低温 13℃"
        const char *data_forecast_14_ymd = data_forecast_14["ymd"];         // "2020-04-20"
        const char *data_forecast_14_week = data_forecast_14["week"];       // "星期一"
        const char *data_forecast_14_sunrise = data_forecast_14["sunrise"]; // "05:54"
        const char *data_forecast_14_sunset = data_forecast_14["sunset"];   // "18:50"
        const char *data_forecast_14_fx = data_forecast_14["fx"];           // "东风"
        const char *data_forecast_14_fl = data_forecast_14["fl"];           // "<3级"
        const char *data_forecast_14_type = data_forecast_14["type"];       // "小雨"
        const char *data_forecast_14_notice = data_forecast_14["notice"];   // "雨虽小，注意保暖别感冒"

        JsonObject data_yesterday = data["yesterday"];
        const char *data_yesterday_date = data_yesterday["date"];       // "05"
        const char *data_yesterday_high = data_yesterday["high"];       // "高温 18℃"
        const char *data_yesterday_low = data_yesterday["low"];         // "低温 11℃"
        const char *data_yesterday_ymd = data_yesterday["ymd"];         // "2020-04-05"
        const char *data_yesterday_week = data_yesterday["week"];       // "星期日"
        const char *data_yesterday_sunrise = data_yesterday["sunrise"]; // "06:10"
        const char *data_yesterday_sunset = data_yesterday["sunset"];   // "18:42"
        int data_yesterday_aqi = data_yesterday["aqi"];                 // 65
        const char *data_yesterday_fx = data_yesterday["fx"];           // "东风"
        const char *data_yesterday_fl = data_yesterday["fl"];           // "3-4级"
        const char *data_yesterday_type = data_yesterday["type"];       // "阴"
        const char *data_yesterday_notice = data_yesterday["notice"];   // "不要被阴云遮挡住好心情"

        return weather;
    }
    else
    {
        return "";
    }
}