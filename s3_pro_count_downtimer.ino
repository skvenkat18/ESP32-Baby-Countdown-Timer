#include <TFT_eSPI.h>
#include <SPI.h>
#include <WiFi.h>
#include "time.h"

#include "baby_scan_145x195.h"
#include "NotoSansMonoSCB20.h"

#define BG        TFT_BLACK
#define CARD      0x0841
#define BABYBLUE  0x5D9F
#define DARKBLUE  0x03BF
#define LIGHTBLUE 0x2C7F
#define DIGITBLUE 0x05FF

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

const char* ssid = "yourssid";
const char* password = "yourpwd";

int yourTimeZone = 2;
unsigned long event = 1778481000;

const char* ntpServer = "pool.ntp.org";
unsigned long epochTime = 0;

String d = "---", h = "--", m = "--", s = "--";

unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return 0;
  time(&now);
  return now;
}

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  unsigned long startAttempt = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 15000) {
    delay(500);
  }
}

void formatData() {
  epochTime = getTime();

  if (epochTime == 0) {
    d = "---"; h = "--"; m = "--"; s = "--";
    return;
  }

  if (event <= epochTime) {
    d = "000"; h = "00"; m = "00"; s = "00";
    return;
  }

  unsigned long ti = event - epochTime;

  int days = ti / 86400;
  ti %= 86400;
  int hours = ti / 3600;
  ti %= 3600;
  int minutes = ti / 60;
  int sec = ti % 60;

  if (days < 10) d = "00" + String(days);
  else if (days < 100) d = "0" + String(days);
  else d = String(days);

  h = hours < 10 ? "0" + String(hours) : String(hours);
  m = minutes < 10 ? "0" + String(minutes) : String(minutes);
  s = sec < 10 ? "0" + String(sec) : String(sec);
}

void drawHeart(int x, int y, uint16_t color, uint16_t bg) {
  sprite.fillSmoothCircle(x - 4, y, 4, color, bg);
  sprite.fillSmoothCircle(x + 4, y, 4, color, bg);
  sprite.fillTriangle(x - 9, y + 2, x + 9, y + 2, x, y + 14, color);
}

void drawSprite() {
  formatData();

  sprite.fillSprite(BG);

  bool pulse = ((millis() / 500) % 2) == 0;
  uint16_t borderColor = pulse ? DIGITBLUE : DARKBLUE;

  sprite.fillRoundRect(6, 6, 468, 210, 18, CARD);
  sprite.drawRoundRect(6, 6, 468, 210, 18, borderColor);
  sprite.drawRoundRect(10, 10, 460, 202, 16, BABYBLUE);

  // Baby scan right side
  sprite.fillRoundRect(322, 14, 145, 195, 14, TFT_BLACK);
  sprite.pushImage(322, 14, 145, 195, baby_scan_145x195);
  sprite.drawRoundRect(322, 14, 145, 195, 14, borderColor);

// Smooth heartbeat hearts
int beat = (millis() / 180) % 6;

uint16_t heartColor;
int heartOffset = 0;

if (beat == 0) {
  heartColor = LIGHTBLUE;
  heartOffset = 0;
} else if (beat == 1) {
  heartColor = DIGITBLUE;
  heartOffset = 1;
} else if (beat == 2) {
  heartColor = LIGHTBLUE;
  heartOffset = 0;
} else {
  heartColor = 0x1082;   // very dark blue, almost hidden
  heartOffset = 0;
}

drawHeart(32, 34 - heartOffset, heartColor, CARD);
drawHeart(292, 34 - heartOffset, heartColor, CARD);

  // Welcome message
// ===== Premium Welcome Header =====
sprite.setTextDatum(4);

// soft glow / shadow
sprite.loadFont(NotoSansMonoSCB20);

sprite.setTextColor(0x1082, CARD);
sprite.drawString("Willkommen", 163, 25);

sprite.setTextColor(TFT_WHITE, CARD);
sprite.drawString("Willkommen", 162, 24);

sprite.setTextColor(0x1082, CARD);
sprite.drawString("kleines Wunder", 163, 53);

sprite.setTextColor(BABYBLUE, CARD);
sprite.drawString("kleines Wunder", 162, 52);

sprite.unloadFont();

// small decorative line
sprite.fillRoundRect(90, 68, 144, 3, 2, DARKBLUE);
sprite.fillRoundRect(124, 68, 76, 3, 2, DIGITBLUE);
  // Countdown boxes
  sprite.fillRoundRect(18, 88, 78, 98, 14, TFT_BLACK);
  sprite.fillRoundRect(104, 88, 62, 98, 14, TFT_BLACK);
  sprite.fillRoundRect(174, 88, 62, 98, 14, TFT_BLACK);
  sprite.fillRoundRect(244, 88, 62, 98, 14, TFT_BLACK);

  sprite.drawRoundRect(18, 88, 78, 98, 14, DARKBLUE);
  sprite.drawRoundRect(104, 88, 62, 98, 14, DARKBLUE);
  sprite.drawRoundRect(174, 88, 62, 98, 14, DARKBLUE);
  sprite.drawRoundRect(244, 88, 62, 98, 14, DARKBLUE);

  // Countdown numbers
  sprite.setTextDatum(4);
  sprite.setTextFont(4);
  sprite.setTextSize(1);
  sprite.setTextColor(DIGITBLUE, TFT_BLACK);

  sprite.drawString(d, 57, 120);
  sprite.drawString(h, 135, 120);
  sprite.drawString(m, 205, 120);
  sprite.drawString(s, 275, 120);

  // Labels
  sprite.loadFont(NotoSansMonoSCB20);
  sprite.setTextDatum(4);
  sprite.setTextColor(TFT_WHITE, TFT_BLACK);

  sprite.drawString("DAYS", 57, 162);
  sprite.drawString("HOUR", 135, 162);
  sprite.drawString("MIN", 205, 162);
  sprite.drawString("SEC", 275, 162);

  sprite.unloadFont();

  sprite.pushSprite(0, 0);
}

void setBacklight(uint8_t brightness) {
  ledcAttachPin(TFT_BL, 0);
  ledcSetup(0, 5000, 8);
  ledcWrite(0, brightness);
}

void setup() {
  setCpuFrequencyMhz(80);
  Serial.begin(115200);
  delay(2000);
  setBacklight(70);
  tft.init();
  tft.setSwapBytes(true);
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  sprite.setColorDepth(16);
  sprite.createSprite(480, 222);
  sprite.setSwapBytes(true);

  drawSprite();

  initWiFi();

  if (WiFi.status() == WL_CONNECTED) {
    configTime(yourTimeZone * 3600, 0, ntpServer);

    unsigned long startTime = millis();
    struct tm timeinfo;

while (!getLocalTime(&timeinfo) && millis() - startTime < 10000) {
  drawSprite();
  delay(500);
}

WiFi.disconnect(true);
WiFi.mode(WIFI_OFF);
btStop();
    }
  }

void loop() {

  static unsigned long lastUpdate = 0;

  if (millis() - lastUpdate >= 250) {
    drawSprite();
    lastUpdate = millis();
  }

  delay(20);
}
