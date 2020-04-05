/*
 * @Author: Kingtous
 * @Date: 2020-04-05 08:03:27
 * @LastEditors: Kingtous
 * @LastEditTime: 2020-04-05 23:25:17
 * @Description: Kingtous' Code
 */
#include <Arduino.h>
#include <FS.h>
#include "wifi_manager.h"
#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0,SCL,SDA);

#define BAUD_RATE 115200
#define PIN_LED 2
WiFiManager *manager;

// WiFi Connected Callback
void onWiFiConnected()
{
  Serial.println("wifi connected");
  digitalWrite(PIN_LED, LOW);
}

void initScreen()
{
  u8g2.begin();
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
  // put your setup code here, to run once:
  manager = new WiFiManager();
  //
  initScreen();
  initSerial();
  initFileSystem();
  manager->setWiFiConnectedCallback(&onWiFiConnected);
  manager->init();
}

void loop()
{
  // put your main code here, to run repeatedly:
  manager->handleServerClient();
  u8g2.firstPage();
  do {
    Serial.println("draw");
    u8g2.setFont(u8g2_font_unifont_t_chinese1);
    u8g2.drawUTF8(0,15,"你好,MDL机器人:");
  } while ( u8g2.nextPage() );
  delay(1000);
}
