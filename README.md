# ESP32 Baby Countdown Timer

## Description

Countdown timer for baby birth using the LilyGO T-Display S3 Pro. The current time is synchronized using an NTP server over Wi-Fi. After synchronization, Wi-Fi and Bluetooth are disabled and the countdown continues using the system time.

## Hardware

* LilyGO T-Display S3 Pro

## Libraries

```cpp
#include <TFT_eSPI.h>
#include <SPI.h>
#include <WiFi.h>
#include <time.h>
```

### Required Arduino Libraries

* TFT_eSPI
* ESP32 Arduino Core
* SPI (included with ESP32 Arduino Core)
* WiFi (included with ESP32 Arduino Core)
* time.h (included with ESP32 Arduino Core)

## Additional Files

* `baby_scan_145x195.h` – Image data
* `NotoSansMonoSCB20.h` – Custom font

## Configuration

Update the Wi-Fi credentials:

```cpp
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
```

Set the timezone:

```cpp
int yourTimeZone = 2;
```

Set the event time (Unix timestamp):

```cpp
unsigned long event = 1778481000;
```

## Build

1. Install the ESP32 board package.
2. Install the `TFT_eSPI` library.
3. Configure `TFT_eSPI` for the display.
4. Update Wi-Fi credentials.
5. Set the event timestamp.
6. Compile and upload the sketch.
