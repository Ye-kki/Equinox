#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <FS.h>
#include <math.h>
#include <stdio.h>

ESP8266WebServer server;
WebSocketsServer webSocket = WebSocketsServer(81);

#define LED_PIN  D2
#define LED_PIN2 D3
#define CLK D5
#define DT D6
#define SW D7
#define LED_COUNT 43

int red = 255 , green = 255, blue = 255;
int brightness = 255;
int LED_mode = 1;
bool wifiAccess = false;
int serverTime = 0;

int counter = 0;           // 회전 카운터 측정용 변수
int currentStateCLK;       // CLK의 현재 신호상태 저장용 변수
int lastStateCLK;          // 직전 CLK의 신호상태 저장용 변수
String currentDir = "";     // 현재 회전 방향 출력용 문자열 저장 변수
unsigned long lastButtonPress = 0;     // 버튼 눌림 상태 확인용 변수
bool btnState = false;
int temp = 6600;

Adafruit_NeoPixel strip_top(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_bottom(LED_COUNT, LED_PIN2, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  WiFi.begin("Yekki6264", "123123123");
  SPIFFS.begin();
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/" , handleRoot);
  server.onNotFound(handleWebRequests);
  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  Serial.println("HTTP server started");
  serverTime = millis();

  strip_top.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip_top.show();            // Turn OFF all pixels ASAP
  strip_bottom.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip_bottom.show();

  // 엔코더의 핀들을 입력으로 설정
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(SW, INPUT_PULLUP);     // 스위치핀은 내부 풀업저항 사용

  // CLK핀의 현재 상태 확인
  lastStateCLK = digitalRead(CLK);
}

void loop() {
  //  while (!wifiAccess) {
  //    delay(500);
  //    Serial.print(".");
  //    if (WiFi.status() == WL_CONNECTED) {
  //      wifiAccess = true;
  //      Serial.println();
  //      Serial.print("Connected, IP address: ");
  //      Serial.println(WiFi.localIP());
  //
  //      server.on("/" , handleRoot);
  //      server.onNotFound(handleWebRequests);
  //      server.begin();
  //      webSocket.begin();
  //      webSocket.onEvent(webSocketEvent);
  //
  //      Serial.println("HTTP server started");
  //      serverTime = millis();
  //      break;
  //    }
  //  }
  webSocket.loop();

  currentStateCLK = digitalRead(CLK);

  if (LED_mode == 1 && currentStateCLK != lastStateCLK  && currentStateCLK == 1) {
    if (digitalRead(DT) != currentStateCLK) {
      brightness += 10;
      if (brightness > 254) brightness = 255;
      strip_top.setBrightness(brightness);
      strip_bottom.setBrightness(brightness);
      for (int i = 0; i < LED_COUNT; i++) {
        strip_top.setPixelColor(i, red, green, blue);
        strip_bottom.setPixelColor(i, red, green, blue);
      }
      strip_top.show();
      strip_bottom.show();
    }
    else {
      brightness -= 10;
      if (brightness < 1) brightness = 0;
      strip_top.setBrightness(brightness);
      strip_bottom.setBrightness(brightness);
      for (int i = 0; i < LED_COUNT; i++) {
        strip_top.setPixelColor(i, red, green, blue);
        strip_bottom.setPixelColor(i, red, green, blue);
      }
      strip_top.show();
      strip_bottom.show();
    }
    Serial.println(brightness);
    Serial.println(digitalRead(DT));
    Serial.println(currentStateCLK);
  }

  if (LED_mode == 2 && currentStateCLK != lastStateCLK  && currentStateCLK == 1) {
    if (digitalRead(DT) != currentStateCLK) {
      if (temp >= 6600) temp += 400;
      else temp += 200;
      if (temp > 11000) temp = 11000;
      kelvinToRGB(temp);
      strip_top.setBrightness(brightness);
      strip_bottom.setBrightness(brightness);
      for (int i = 0; i < LED_COUNT; i++) {
        strip_top.setPixelColor(i, red, green, blue);
        strip_bottom.setPixelColor(i, red, green, blue);
      }
      strip_top.show();
      strip_bottom.show();

    }
    else {
      if (temp >= 6600) temp -= 400;
      else temp -= 200;
      if (temp < 700) temp = 700;
      kelvinToRGB(temp);
      strip_top.setBrightness(brightness);
      strip_bottom.setBrightness(brightness);
      for (int i = 0; i < LED_COUNT; i++) {
        strip_top.setPixelColor(i, red, green, blue);
        strip_bottom.setPixelColor(i, red, green, blue);
      }
      strip_top.show();
      strip_bottom.show();
    }
    Serial.println(temp);
  }
  lastStateCLK = currentStateCLK;

  if (digitalRead(SW)) btnState = true;

  if (btnState == true && digitalRead(SW) == LOW) {
    LED_mode++;
    if (LED_mode > 2) LED_mode = 1;
    Serial.println(LED_mode);
    btnState = false;
    delay(50);
  }
  server.handleClient();
  if (Serial.available() > 0) {
    char c[] = {(char)Serial.read()};
    webSocket.broadcastTXT(c, sizeof(c));
  }
}

void kelvinToRGB(int Temperature) {
  Temperature = Temperature / 100;

  if (Temperature <= 66) red = 255;
  else {
    red = Temperature - 60;
    red = int(329.698727446 * pow(red, -0.1332047592));
    if (red < 0) red = 0;
    if (red > 255) red = 255;
  }

  if (Temperature <= 66) {
    green = Temperature;
    green = int(99.4708025861 * log(green) - 161.1195681661);
    if (green < 0) green = 0;
    if (green > 255) green = 255;
  }
  else {
    green = Temperature - 60;
    green = int(288.1221695283 * pow(green, -0.0755148492));
    if (green < 0) green = 0;
    if (green > 255) green = 255;
  }

  if (Temperature >= 66) blue = 255;
  else {
    if (Temperature <= 19) blue = 0;
    else {
      blue = Temperature - 10;
      blue = int(138.5177312231 * log(blue) - 305.0447927307);
      if (blue < 0) blue = 0;
      if (blue > 255)  blue = 255;
    }
  }
  delay(20);
}

void handleRoot() {
  server.sendHeader("Location", "/index.html" , true);
  server.send(302, "text/html", "555");
}

void handleWebRequests() {
  if (loadFromSpiffs(server.uri()))return; // SPIFFS에 요청한 파일 확인
  String message = "File Not Detected\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " NAME:" + server.argName(i) + "\n VALUE:" + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  Serial.println(message);
}

bool loadFromSpiffs(String path) { // SPIFFS 에서 파일 확인
  String dataType = "text/plain";

  // 요청한 파일의 확장자에 따라 데이터 타입 정의
  if (SPIFFS.exists(path)) {
    // 주소의 확장자에따라 데이터 타입 지정
    if (path.endsWith(".src")) path = path.substring(0, path.lastIndexOf("."));
    else if (path.endsWith(".html")) dataType = "text/html";
    else if (path.endsWith(".css")) dataType = "text/css";
    else if (path.endsWith(".js")) dataType = "application/javascript";
    File dataFile = SPIFFS.open(path.c_str(), "r"); // SPIFFS 에서 파일 읽기
    if (server.hasArg("download")) dataType = "application/octet-stream";
    if (server.streamFile(dataFile, dataType) != dataFile.size()) {
    }
    dataFile.close();
    return true;
  }
  else return false;
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {   //웹소켓에서 받는 신호
  if (type == WStype_TEXT) {
    stingToInt(payload);
  }
}

void stingToInt(uint8_t* s) {
  int num = 0;
  if (s[0] == 'K') { //온도
    for (int i = 1; i <= 9; i++) num = num * 10 + (s[i] - '0');
    red = num / 1000000;
    green = (num % 1000000) / 1000;
    blue = (num % 1000);
    strip_top.setBrightness(brightness);
    strip_bottom.setBrightness(brightness);
    for (int i = 0; i < LED_COUNT; i++) {
      strip_top.setPixelColor(i, red, green, blue);
      strip_bottom.setPixelColor(i, red, green, blue);
    }
    strip_top.show();
    strip_bottom.show();
  }
  else if (s[0] == 'B') {
    for (int i = 2; i <= 4; i++) num = num * 10 + (s[i] - '0');
    brightness = num;
    strip_top.setBrightness(brightness);
    strip_bottom.setBrightness(brightness);
    for (int i = 0; i < LED_COUNT; i++) {
      strip_top.setPixelColor(i, red, green, blue);
      strip_bottom.setPixelColor(i, red, green, blue);
    }
    strip_top.show();
    strip_bottom.show();
  }
  else if (s[0] == 'D') {
    strip_top.setBrightness(0);
    strip_bottom.setBrightness(0);
    strip_top.show();
    strip_bottom.show();
    for (int i = 2; i <= 4; i++) num = num * 10 + (s[i] - '0');
    if (num > 188 && num < 353) {
      strip_top.setBrightness(255);
      strip_top.setPixelColor(sunPos(num) - 1, 200, 100, 0);
      strip_top.setPixelColor(sunPos(num), 255, 146, 38);
      strip_top.setPixelColor(sunPos(num) + 1, 200, 100, 0);
      strip_top.show();
    }
    else if (num > 7 && num < 172) {
      strip_bottom.setBrightness(255);
      strip_bottom.setPixelColor(sunPos(num) - 1, 200, 100, 0);
      strip_bottom.setPixelColor(sunPos(num), 255, 146, 38);
      strip_bottom.setPixelColor(sunPos(num) + 1, 200, 100, 0);
      strip_bottom.show();
    }
    else if (num >= 172 && num <= 188) {
      strip_top.setBrightness(255);
      strip_bottom.setBrightness(255);
      strip_top.setPixelColor(41, 200, 100, 0);
      strip_top.setPixelColor(42, 255, 146, 38);
      strip_bottom.setPixelColor(42, 255, 146, 38);
      strip_top.setPixelColor(41, 200, 100, 0);
      strip_top.show();
      strip_bottom.show();
    }
    else {
      strip_top.setBrightness(255);
      strip_bottom.setBrightness(255);
      strip_top.setPixelColor(1, 200, 100, 0);
      strip_top.setPixelColor(0, 255, 146, 38);
      strip_bottom.setPixelColor(0, 255, 146, 38);
      strip_top.setPixelColor(1, 200, 100, 0);
      strip_top.show();
      strip_bottom.show();
    }
  }
}

int sunPos(int deg) {
  int ledPos;
  if (deg >= 180 && deg < 360) ledPos = int(float(360 - deg) / 180 * 45) - 1;
  else ledPos = int(float(deg) / 180 * 45) - 1;
  return ledPos;
}
