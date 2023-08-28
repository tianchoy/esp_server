#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "DHT.h"

const char* ssid = "ESP8266-Access-Point";
const char* password = "123456789";

DHT dht(4, DHT11);

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String readTemp() {
  return String(dht.readTemperature());
  
}

String readHumi() {
  return String(dht.readHumidity());
}

String readPres() {
  return String(dht.readTemperature(true) / 100.0F);
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial.println();
  dht.begin();
  
  // Setting the ESP as an access point
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/temp", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readTemp().c_str());
  });
  server.on("/humi", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readHumi().c_str());
  });
  server.on("/pres", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readPres().c_str());
  });
  

  
  // Start server
  server.begin();
}
 
void loop() {
  // 测量之间等待2秒钟。
  delay(1000);
  float h = dht.readHumidity();
  // 以摄氏度读取温度(默认值)
  float t = dht.readTemperature();
  // 将温度读取为华氏温度(isFahrenheit = true)
  float f = dht.readTemperature(true);

  // 检查是否有读取失败并提前退出(再试一次)。
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  //打印湿度
  Serial.print(F("Humidity: "));
  Serial.print(h);
  //打印温度
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
}