<!--
 * @Author: Kingtous
 * @Date: 2020-04-05 00:03:45
 * @LastEditors: Kingtous
 * @LastEditTime: 2020-04-06 01:03:31
 * @Description: Kingtous' Code
 -->
#### Arduino ESP8266 程序

### 适配配置
- 芯片：`ESP8266 (NodeMcu、CH340)`
- 显示器：`SSD1306OLED SW I2C`
- 语言：C++ 17

### 功能
- 未连接成功，自动开始AP和WebServer，访问 `192.168.4.1` 即可设置新WiFi
- 连接成功，LED灯常亮。正在连接，LED灯闪烁。连接失败开启AP，LED灯熄灭

### 目前使用的第三方库
> 请下载第三方库项目的`src`文件夹，更名后放入`lib`文件夹
- U8g2 (用于显示屏输出)
- U8g2 wqy (适合 u8g2 的中文字体)
