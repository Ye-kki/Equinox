#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <FS.h>
#include <math.h>

//ESP8266WebServer server;
//WebSocketsServer webSocket = WebSocketsServer(81);
#define LED_PIN  D2
#define LED_PIN2 D3
#define CLK D5
#define DT D6
#define SW D7
#define LED_COUNT 43

int red = 0 , green = 0, blue = 0;
int brightness = 0;
int LED_mode = 1;
uint16_t redValue[2];
uint16_t greenValue[2];
uint16_t blueValue[2];
int serverTime = 0;

int counter = 0;           // 회전 카운터 측정용 변수
int currentStateCLK;       // CLK의 현재 신호상태 저장용 변수
int lastStateCLK;          // 직전 CLK의 신호상태 저장용 변수
String currentDir = "";     // 현재 회전 방향 출력용 문자열 저장 변수
unsigned long lastButtonPress = 0;     // 버튼 눌림 상태 확인용 변수
int btnState = 0;
int temp = 6600;

Adafruit_NeoPixel strip_top(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_bottom(LED_COUNT, LED_PIN2, NEO_GRB + NEO_KHZ800);

void setup() {
  //  Serial.begin(115200);
  //  WiFi.begin("Yekki6264", "123123123");
  //  SPIFFS.begin();
  //  while (WiFi.status() != WL_CONNECTED)
  //  {
  //    delay(500);
  //    Serial.print(".");
  //  }
  //  Serial.println();
  //  Serial.print("Connected, IP address: ");
  //  Serial.println(WiFi.localIP());
  //
  //  server.on("/" , handleRoot);
  //  server.onNotFound(handleWebRequests);
  //  server.begin();
  //  webSocket.begin();
  //  webSocket.onEvent(webSocketEvent);
  //
  //  Serial.println("HTTP server started");
  //  serverTime = millis();
  Serial.begin(9600);
  strip_top.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip_top.setBrightness(0);
  strip_top.show();            // Turn OFF all pixels ASAP
  strip_bottom.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip_bottom.setBrightness(0);
  strip_bottom.show();

  // 엔코더의 핀들을 입력으로 설정
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(SW, INPUT_PULLUP);     // 스위치핀은 내부 풀업저항 사용

  // CLK핀의 현재 상태 확인
  lastStateCLK = digitalRead(CLK);
}

void loop() {
  //  webSocket.loop();
  // CLK핀의 상태를 확인
  currentStateCLK = digitalRead(CLK);
  // 버튼(스위치)이 눌렸는지 확인
  btnState = digitalRead(SW);

  if (LED_mode == 1 && currentStateCLK != lastStateCLK  && currentStateCLK == 1) { //회전시
    // DT핀의 신호를 확인해서 엔코더의 회전 방향을 확인함.
    if (digitalRead(DT) != currentStateCLK) {  //시계방향 회전
      brightness += 10;                           // 카운팅 용 숫자 1 증가
      if (brightness > 254) brightness = 255;
      strip_top.setBrightness(brightness);
      strip_bottom.setBrightness(brightness);
      for (int i = 0; i < LED_COUNT; i++) {
        strip_top.setPixelColor(i, red, green, blue);
        strip_bottom.setPixelColor(i, red, green, blue);
      }
      strip_top.show();
      strip_bottom.show();

    } else {                                 //반시계방향 회전
      brightness -= 10;                       // 카운팅 용 숫자 1 감소
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
    Serial.print(red);
    Serial.print(", ");
    Serial.print(green);
    Serial.print(", ");
    Serial.print(blue);
    Serial.print(", ");
    Serial.println(brightness);
  }

  if (LED_mode == 2 && currentStateCLK != lastStateCLK  && currentStateCLK == 1) { //회전시
    // DT핀의 신호를 확인해서 엔코더의 회전 방향을 확인함.
    if (digitalRead(DT) != currentStateCLK) {  //시계방향 회전
      if (temp >= 6600) temp += 400;
      else temp += 200;                           // 카운팅 용 숫자 1 증가
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

    } else {                                 //반시계방향 회전
      if (temp >= 6600) temp -= 400;
      else temp -= 200;                         // 카운팅 용 숫자 1 감소
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
    Serial.print(temp);
    Serial.print(", ");
    Serial.println(brightness);
  }
  // 현재의 CLK상태를 저장
  lastStateCLK = currentStateCLK;

  // 버튼(스위치)가 눌리면
  if (btnState == LOW) {
    //버튼이 눌린지 50ms가 지났는지 확인, 즉 버튼이 한번 눌린 후 최소 50 ms는 지나야 버튼이 다시 눌린것으로 감지
    if (millis() - lastButtonPress > 50) {  // 50ms 이상 지났다면
      LED_mode++;
      if (LED_mode > 2) LED_mode = 1;
      Serial.println(LED_mode);
    }

    // 마자막 버튼이 눌린 시간 저장
    lastButtonPress = millis();
  }

  // 잠시 대기
  delay(1);


  //  server.handleClient();
  //  if (Serial.available() > 0) {
  //    char c[] = {(char)Serial.read()};
  //    webSocket.broadcastTXT(c, sizeof(c));
  //  }

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
}

//void sunPos(int deg) {
//  int ledPos;
//  if (deg >= 180 && deg < 360) ledPos = int(float(360 - deg) / 180 * 45) - 1;
//  else ledPos = int(float(deg) / 180 * 45) - 1;
//
//}

//
//void handleRoot() {
//  server.sendHeader("Location", "/index.html" , true);
//  server.send(302, "text/html", "555");
//}
//
//void handleWebRequests() {
//  if (loadFromSpiffs(server.uri()))return; // SPIFFS에 요청한 파일 확인
//  String message = "File Not Detected\n\n";
//  message += "URI: ";
//  message += server.uri();
//  message += "\nMethod: ";
//  message += (server.method() == HTTP_GET) ? "GET" : "POST";
//  message += "\nArguments: ";
//  message += server.args();
//  message += "\n";
//  for (uint8_t i = 0; i < server.args(); i++) {
//    message += " NAME:" + server.argName(i) + "\n VALUE:" + server.arg(i) + "\n";
//  }
//  server.send(404, "text/plain", message);
//  Serial.println(message);
//}
//
//bool loadFromSpiffs(String path) { // SPIFFS 에서 파일 확인
//  String dataType = "text/plain";
//
//  // 요청한 파일의 확장자에 따라 데이터 타입 정의
//  if (SPIFFS.exists(path)) {
//    // 주소의 확장자에따라 데이터 타입 지정
//    if (path.endsWith(".src")) path = path.substring(0, path.lastIndexOf("."));
//    else if (path.endsWith(".html")) dataType = "text/html";
//    else if (path.endsWith(".css")) dataType = "text/css";
//    else if (path.endsWith(".js")) dataType = "application/javascript";
//    File dataFile = SPIFFS.open(path.c_str(), "r"); // SPIFFS 에서 파일 읽기
//    if (server.hasArg("download")) dataType = "application/octet-stream";
//    if (server.streamFile(dataFile, dataType) != dataFile.size()) {
//    }
//    dataFile.close();
//    return true;
//  }
//  else return false;
//}
//
//void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {   //웹소켓에서 받는 신호
//  if (type == WStype_TEXT) {
//    Serial.println(payload[1]);
//    if (payload[0] == '#') { //hex코드 일때(색)
//      colorOption = false;
//      option[0] = false;
//      option[1] = false;
//      option[2] = false;
//      option[3] = false;
//      if (payload[1] >= 48 && payload[1] <= 57) redValue[0] = (payload[1] - 48) * 16;
//      else redValue[0] = (payload[1] - 87) * 16;
//
//      if (payload[2] >= 48 && payload[2] <= 57) redValue[1] = payload[2] - 48;
//      else redValue[1] = (payload[2] - 87);
//      red = redValue[0] + redValue[1];
//
//      if (payload[3] >= 48 && payload[3] <= 57) greenValue[0] = (payload[3] - 48) * 16;
//      else greenValue[0] = (payload[3] - 87) * 16;
//
//      if (payload[4] >= 48 && payload[4] <= 57) greenValue[1] = (payload[4] - 48);
//      else greenValue[1] = (payload[4] - 87);
//      green = greenValue[0] + greenValue[1];
//
//      if (payload[5] >= 48 && payload[5] <= 57) blueValue[0] = (payload[5] - 48) * 16;
//      else blueValue[0] = (payload[5] - 87) * 16;
//
//      if (payload[6] >= 48 && payload[6] <= 57) blueValue[1] = payload[6] - 48;
//      else blueValue[1] = (payload[6] - 87);
//      blue = blueValue[0] + blueValue[1];
//
//      strip.begin();
//      for (int i = 0; i < LED_COUNT; i++) {
//        strip.setPixelColor(i, red, green, blue);
//      }
//      strip.setBrightness(255);
//      strip.show();
//      LED_on = true;
//    }
//  }
//}
