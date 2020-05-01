// Copyright (c) 2020 Kingtous
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

/*
 * @Author: Kingtous
 * @Date: 2020-04-05 08:03:27
 * @LastEditors: Kingtous
 * @LastEditTime: 2020-05-01 21:54:47
 * @Description: Kingtous' Code
 */
#include <Arduino.h>
#include <FS.h>
#include "wifi_manager.h"
#include "weather_manager.h"
#include <U8g2lib.h>
#include <u8g2_wqy.h>
#include <string>
#include <sstream>

#define SSD1306_SDA 5
#define SSD1306_SLK 4
// rotate clock data
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, SSD1306_SLK, SSD1306_SDA);

#define BAUD_RATE 115200
#define PIN_LED 2
#define FLASH_KEY 0
#define FORECAST_DAYS 10

WiFiManager *manager;
WeatherManager *weather;
bool haveWeather = false;

// WiFi Connected Callback
void onWiFiConnected()
{
  Serial.println("wifi connected");
  manager->startWebServer();
  // 输出IP地址
  u8g2.firstPage();
  do
  {
    u8g2.setFont(u8g2_font_wqy12_t_gb2312a);
    u8g2.drawUTF8(0, 14, "已接入WiFi网络");
    String IP = WiFi.localIP().toString();
    String content = "IP：" + IP;
    Serial.println(content);
    u8g2.drawUTF8(0, 28, content.c_str());
  } while (u8g2.nextPage());
  delay(3000);
  digitalWrite(PIN_LED, LOW);
}

// 失败后输出，WiFiManager会自动启动WebServer，所以不需要我们手动启用
void onWiFiConnectFailed()
{
  Serial.println("wifi connect failed.");
  u8g2.firstPage();
  do
  {
    u8g2.setFont(u8g2_font_wqy12_t_gb2312a);
    u8g2.drawUTF8(0, 15, "WiFi连接失败");
    u8g2.drawUTF8(0, 30, "开启AP模式...");
  } while (u8g2.nextPage());
  digitalWrite(PIN_LED, LOW);
}

// 欢迎logo
const unsigned char col[] U8X8_PROGMEM= {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0xff,0xff,0xf8,0x00,0x00,0x00,0x00,0x3f,0xff,0xff,0xf8,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xf0,0x00,0x00,0x00,0x00,0xfc,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xf0,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xfe,0x00,0x00,0x00,0x00,0x3f,0xff,0xff,0xff,0xc0,0x00,0x00,0x00,0x07,0xff,0xff,0xf8,0xf8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7e,0x00,0x00,0x07,0xff,0xff,0xff,0xff,0xfc,0x00,0xf8,0x7f,0xff,0xff,0xff,0xff,0xf0,0x00,0x00,0x0f,0xff,0xff,0xff,0xff,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
void initScreen()
{
  u8g2.begin();
  u8g2.enableUTF8Print();
  u8g2.firstPage();
  do
  {
    u8g2.drawXBMP(0, 0, 62, 59, col);
    u8g2.setFont(u8g2_font_wqy12_t_gb2312a);
    u8g2.drawUTF8(65, 15, "天气助手");
    u8g2.drawUTF8(65, 35, "正在连接");
    u8g2.drawUTF8(65, 50, "WiFi..");
  } while (u8g2.nextPage());
}

// init Serial Baud Rate to 115200
void initSerial()
{
  Serial.begin(BAUD_RATE);
  Serial.println("NodeMCU Node(Kingtous)");
}

void initFileSystem()
{
  SPIFFS.begin();
  EEPROM.begin(512);
}

void setup()
{
  // init LED
  pinMode(PIN_LED, OUTPUT);
  // init FLASK KEY
  pinMode(FLASH_KEY, INPUT);
  // put your setup code here, to run once:
  manager = new WiFiManager();
  weather = new WeatherManager();
  //
  initScreen();
  initSerial();
  initFileSystem();
  manager->setWiFiConnectedCallback(&onWiFiConnected);
  manager->setWiFiConnectFailedCallback(&onWiFiConnectFailed);
  manager->init();
}


int page_index = -1;
int totalIndex = 0;
int current_page_index = -1;
Weather weatherDetail;

void loadIndexPage(int page){
  
  current_page_index = page;
  Serial.println("load next");
    u8g2.firstPage();
    do
      {
        u8g2.setFont(u8g2_font_wqy12_t_gb2312a);
        int y_offset = 12;
        JsonObject object = weatherDetail.data[page];
        if (!object.isNull())
        {
          // const char *data_forecast_0_date = data_forecast_0["date"];       // "06"
          // const char *data_forecast_0_high = data_forecast_0["high"];       // "高温 21℃"
          // const char *data_forecast_0_low = data_forecast_0["low"];         // "低温 11℃"
          // const char *data_forecast_0_ymd = data_forecast_0["ymd"];         // "2020-04-06"
          // const char *data_forecast_0_week = data_forecast_0["week"];       // "星期一"
          // const char *data_forecast_0_sunrise = data_forecast_0["sunrise"]; // "06:09"
          // const char *data_forecast_0_sunset = data_forecast_0["sunset"];   // "18:43"
          // int data_forecast_0_aqi = data_forecast_0["aqi"];                 // 72
          // const char *data_forecast_0_fx = data_forecast_0["fx"];           // "东北风"
          // const char *data_forecast_0_fl = data_forecast_0["fl"];           // "<3级"
          // const char *data_forecast_0_type = data_forecast_0["type"];       // "多云"
          // const char *data_forecast_0_notice = data_forecast_0["notice"];   // "阴晴之间，谨防紫外线侵扰"
          if (!object.getMember("ymd").isNull())
          {
            u8g2.drawUTF8(0, y_offset, object["ymd"]); // date
            u8g2.drawUTF8(70, y_offset, weatherDetail.cityInfoCity.c_str()); // date
            y_offset += 12;
          }
          if (!object.getMember("high").isNull())
          {
            u8g2.drawUTF8(0, y_offset, object["high"]); // high
            y_offset += 12;
          }
          if (!object.getMember("low").isNull())
          {
            u8g2.drawUTF8(0, y_offset, object["low"]); // high
            y_offset += 12;
          }
          if (!object.getMember("type").isNull())
          {
            u8g2.drawUTF8(0, y_offset, object["type"]); // high
          }
        }
        std::stringstream ssTemp;
        ssTemp << page << "/" << FORECAST_DAYS-1;
        std::string st;
        ssTemp >> st;
        u8g2.drawUTF8(80, y_offset, st.c_str()); // type
      } while (u8g2.nextPage());
}

void loop()
{
  // put your main code here, to run repeatedly:
  while (Serial.available()>0)
  {
    char ch = Serial.read();
    if (ch == 'R')
    {
      haveWeather = false;
      break;
    }
    else if (ch == 'N')
    {
      if (haveWeather)
      {
        page_index++;
        if (page_index == FORECAST_DAYS)
        {
          // 14 days forecast
          page_index = 0;
        }
        loadIndexPage(page_index);
      }
      break;
    }
  }
  // 天气服务
  if (!haveWeather && manager->wifiConnected())
  {
    Serial.println("start fetching weather");

    // 输出正在获取天气
    u8g2.firstPage();
    do
    {
      u8g2.setFont(u8g2_font_wqy12_t_gb2312a);
      u8g2.drawUTF8(0, 14, "正在获取天气");
      u8g2.drawUTF8(0, 28, "请稍候...");
    } while (u8g2.nextPage());
    weatherDetail = weather->getWeather();
    if (page_index == -1)
    {
      String w = weatherDetail.firstPage;
        if (w.length() == 0)
      {
        // 没有获取到
        Serial.println("waiting for 0.5s");
        delay(500);
      }
      else
      {
        haveWeather = true;
        u8g2.firstPage();
        do
        {
          u8g2.setFont(u8g2_font_wqy12_t_gb2312a);
          Serial.println(w);
          int y_offset = 14;
          while (w.length() != 0)
          {
            String content = "";
            int index = w.indexOf('\n');
            if (index != -1)
            {
              if (index + 1 < w.length())
              {
                content = w.substring(0, index);
                w = w.substring(index + 1);
              }
              else
              {
                // index 是最后一个\n
                content = w.substring(0, index);
                w = "";
              }
            }
            else
            {
              content = w;
              w = "";
            }
            u8g2.drawUTF8(0, y_offset, content.c_str());
            y_offset += 14;
          }
        } while (u8g2.nextPage());
      }
    } 
  }

  bool isPressed = false;
  while (haveWeather && digitalRead(FLASH_KEY) == LOW)
  {
    isPressed = true;
  }
  if (isPressed)
  {
    page_index++;
    if (page_index == FORECAST_DAYS)
    {
      // 14 days forecast
      page_index = 0;
    }
    loadIndexPage(page_index);
  }

  // Web服务
  //Serial.println("handleServer");
  manager->handleServerClient();
}
