/* !!! WORK-IN-PROGRESS */

/*
   ARDUINO STUDY TRACKER WITH 128*64 OLED LCD, SD1307 RTC and DHT11
   github.com/ahmetemindilben - ahmetemindilben.com.tr - ahmetemindilben.com - instructables.com/member/ahmeted
   18.03.2022 - 28.06.2022
   CC BY-NC-SA 4.0
*/
/*
  v1: just a basic chronometer with an OLED LCD with one button
  v2: real time and date screen, temperature and humidity screen adition
  v3: remaining battery addition
  v4: v3 + analiz et soru no üretici
  v5: v4 + deneme sayaçları
*/

// --- CONFIGURATION --- //
// 0:DISABLE 1: ENABLE

int openingScreen = 0;
int timeAndDateScreen = 0;
int temperatureAndHumadityScreen = 0;
int stopwatchScreen = 1;
int dateCountdownScreen = 1;
int motivationalQuotesScreen = 1;
int remainingBatteryScreen = 1;

//configure this section if dateCountdownScreen is enable.
int deadlineDay = 30;
int deadlineMonth = 7;
int deadlineYear = 2022;

//OLED display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//DS1307 RTC
#include <SPI.h>
#include <Time.h>
#include <DS1307RTC.h>
//DHT11
#include "DHT.h"
#define DHT11Pin 3
#define DHTType DHT11  // if you are using dht22, change DHT11 to DHT22
DHT dht(DHT11Pin, DHTType);
int humidity;
int temperature;
//Buttons
int buttonMode = 4;
int buttonModeState = 0;
int buttonStart = 5;
int buttonStartState = 0;
int buttonReset = 6;
//Stopwatch variables
long int secondsStopwatch = 0;
int minutesStopwatch = 0;
int millisecond = 0;
//Countdown screen variables
tmElements_t tm;


int currentDay = tm.Day;
int currentMonth = tm.Month;
int currentYear = tm.Year;

int remainingYear = 0;
int remainingMonth = 0;
int remainingDayInMonth = 0;

int remainingDay = 0;
//Remaining battery variables
float referans = 5.0;
#define batteryStatus 0;



//--- CONFIGURE THIS SECTION FOR YOUR OLED DISPLAY ---//
//define section for OLED display
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// opening logo 1
const unsigned char logo1[] PROGMEM = {
  // This icon cover has been designed using resources from Flaticon.com
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x0f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfc, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xfe, 0x00, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x3f, 0x80, 0x01, 0xfc, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x07, 0xfc, 0x0f, 0xc0, 0x03, 0xf0, 0x3f, 0xe0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0x81, 0xe0, 0x07, 0x81, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xe0, 0x70, 0x0e, 0x07, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x83, 0xff, 0xfc, 0x30, 0x0c, 0x3f, 0xff, 0xc1, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xe0, 0x7f, 0xfe, 0x08, 0x10, 0x7f, 0xfe, 0x07, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xf8, 0x1f, 0xff, 0x80, 0x01, 0xff, 0xf8, 0x1f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0x07, 0xff, 0xe0, 0x07, 0xff, 0xe0, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xc0, 0xff, 0xf0, 0x0f, 0xff, 0x03, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xf0, 0x3f, 0xfc, 0x3f, 0xfc, 0x0f, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xfc, 0x0f, 0xfe, 0x3f, 0xf0, 0x3f, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x83, 0xfe, 0x7f, 0xc1, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xef, 0xff, 0xe0, 0x7e, 0x7e, 0x07, 0xff, 0xf7, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xc1, 0xff, 0xf8, 0x10, 0x08, 0x1f, 0xff, 0x83, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xc0, 0x7f, 0xff, 0x00, 0x00, 0xff, 0xfe, 0x03, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xc0, 0x1f, 0xff, 0xc0, 0x03, 0xff, 0xf8, 0x03, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xc7, 0x07, 0xff, 0xc7, 0xe3, 0xff, 0xe0, 0xe3, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xc7, 0xc1, 0xff, 0xc7, 0xe3, 0xff, 0x83, 0xe3, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xc7, 0xf0, 0x7f, 0xc7, 0xe3, 0xfe, 0x0f, 0xe3, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xc7, 0xfc, 0x1f, 0xc7, 0xe3, 0xf8, 0x3f, 0xe3, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xc7, 0xff, 0x07, 0xc0, 0x03, 0xe0, 0xff, 0xe3, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xc7, 0xff, 0xc1, 0xc0, 0x03, 0x83, 0xff, 0xe3, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xc7, 0xff, 0xf0, 0xc7, 0xe3, 0x0f, 0xff, 0xe3, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xc7, 0xff, 0xfc, 0xc7, 0xe3, 0x3f, 0xff, 0xe3, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xc3, 0xff, 0xfc, 0xc7, 0xe3, 0x3f, 0xff, 0xc3, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xe0, 0xff, 0xfc, 0xc0, 0x03, 0x3f, 0xff, 0x07, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xf8, 0x3f, 0xfc, 0xc0, 0x03, 0x3f, 0xfc, 0x1f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfe, 0x0f, 0xfc, 0xc7, 0xe3, 0x3f, 0xf0, 0x7f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0x83, 0xfc, 0xc7, 0xe3, 0x3f, 0xc1, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xef, 0xe0, 0xfc, 0xc7, 0xe3, 0x3f, 0x07, 0xf7, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xc3, 0xf8, 0x3c, 0xc7, 0xe3, 0x3c, 0x1f, 0xc3, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xe0, 0xfe, 0x04, 0xc7, 0xe3, 0x20, 0x7f, 0x07, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xf8, 0x1f, 0x80, 0xc7, 0xe3, 0x01, 0xf8, 0x1f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfe, 0x07, 0xe0, 0xc7, 0xe3, 0x07, 0xe0, 0x7f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0x81, 0xfc, 0xc7, 0xe3, 0x3f, 0x81, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xf0, 0x7f, 0xc7, 0xe3, 0xfe, 0x0f, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xf9, 0xfc, 0x1f, 0xc7, 0xe3, 0xf8, 0x3f, 0x9f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xf8, 0x7f, 0x07, 0xc7, 0xe3, 0xe0, 0xfe, 0x1f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfc, 0x1f, 0xc1, 0xc7, 0xe3, 0x83, 0xf8, 0x3f, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0x07, 0xf0, 0xc7, 0xe3, 0x0f, 0xe0, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xc1, 0xfd, 0xc7, 0xe3, 0xbf, 0x83, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xf0, 0x7f, 0xc7, 0xe3, 0xfe, 0x0f, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xfc, 0x0f, 0xc7, 0xe3, 0xf0, 0x3f, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x07, 0xc7, 0xe3, 0xe0, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xe3, 0xc7, 0xe3, 0xc7, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xc7, 0xe3, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xc7, 0xe3, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xc7, 0xe3, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xc0, 0x03, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xc0, 0x03, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xc7, 0xe3, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xc7, 0xe3, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xc0, 0x03, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xc0, 0x03, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xc7, 0xe3, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xc7, 0xe3, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xc7, 0xe3, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xc7, 0xe3, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0xe2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// opening logo 2
const unsigned char logo2[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x07, 0xc0, 0x40, 0x00, 0x01, 0xc0, 0x00, 0x0f, 0xfc, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00,
  0x0f, 0xf0, 0xc0, 0x00, 0x01, 0xc0, 0x00, 0x0f, 0xfc, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00,
  0x1f, 0xf0, 0xc0, 0x00, 0x01, 0xc0, 0x00, 0x0f, 0xfc, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00,
  0x1c, 0x31, 0xf3, 0x0c, 0x1d, 0xdc, 0x38, 0x00, 0xc1, 0xb8, 0xf8, 0x1e, 0x18, 0xe1, 0xe0, 0xd8,
  0x1c, 0x01, 0xf3, 0x0c, 0x3f, 0xcc, 0x30, 0x00, 0xc1, 0xf9, 0xfc, 0x7f, 0x19, 0xc3, 0xf0, 0xf8,
  0x0f, 0x80, 0xc3, 0x0c, 0x73, 0xcc, 0x30, 0x00, 0xc1, 0xc3, 0x9c, 0x63, 0x9b, 0x87, 0x38, 0xe0,
  0x07, 0xf0, 0xc3, 0x0c, 0x61, 0xce, 0x70, 0x00, 0xc1, 0xc0, 0x3c, 0xe2, 0x1f, 0x06, 0x18, 0xc0,
  0x01, 0xf8, 0xc3, 0x0c, 0x61, 0xc6, 0x60, 0x00, 0xc1, 0x81, 0xfc, 0xe0, 0x1f, 0x87, 0xf8, 0xc0,
  0x00, 0x38, 0xc3, 0x0c, 0x61, 0xc6, 0x60, 0x00, 0xc1, 0x83, 0x8c, 0xe0, 0x1f, 0x87, 0xf8, 0xc0,
  0x1c, 0x38, 0xc3, 0x0c, 0x61, 0xc3, 0xe0, 0x00, 0xc1, 0x83, 0x0c, 0xe1, 0x19, 0xc6, 0x00, 0xc0,
  0x1f, 0xf8, 0xc3, 0x1c, 0x73, 0xc3, 0xc0, 0x00, 0xc1, 0x83, 0x9c, 0x63, 0x98, 0xc7, 0x18, 0xc0,
  0x0f, 0xf0, 0xf3, 0xfc, 0x3f, 0xc3, 0xc0, 0x00, 0xc1, 0x83, 0xfc, 0x7f, 0x18, 0xe3, 0xf0, 0xc0,
  0x07, 0xe0, 0x71, 0xec, 0x1d, 0xc1, 0x80, 0x00, 0xc1, 0x81, 0xec, 0x1e, 0x18, 0x61, 0xe0, 0xc0,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x80, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0xcc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0xc8, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x8f, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x98, 0x80, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x80, 0x40, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x40, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x10, 0x40, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x08, 0x80, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x07, 0x08, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x20, 0x00, 0x00, 0x80, 0x00, 0x10, 0x01, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00,
  0x00, 0x20, 0x00, 0x00, 0x80, 0x00, 0x00, 0x01, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00,
  0x0f, 0x3c, 0xf6, 0x39, 0xce, 0x7b, 0x17, 0x87, 0x25, 0x63, 0x8e, 0x03, 0x0c, 0x7b, 0x06, 0x70,
  0x01, 0x24, 0xc9, 0x24, 0x91, 0x64, 0x96, 0x49, 0x25, 0xb4, 0x49, 0x04, 0x92, 0x6d, 0x82, 0x40,
  0x03, 0x22, 0x89, 0x7c, 0x9f, 0x44, 0x94, 0x49, 0x25, 0x17, 0xc9, 0x04, 0x11, 0x48, 0x82, 0x40,
  0x09, 0x22, 0x89, 0x40, 0x90, 0x44, 0x94, 0x49, 0x25, 0x14, 0x09, 0x04, 0x11, 0x48, 0x82, 0x40,
  0x09, 0x22, 0x89, 0x24, 0x93, 0x44, 0x94, 0x49, 0x25, 0xb6, 0x49, 0x04, 0x92, 0x48, 0x82, 0x40,
  0x0d, 0x22, 0x89, 0x38, 0x4e, 0x44, 0x94, 0x47, 0x25, 0xe3, 0x89, 0x23, 0x0c, 0x48, 0x93, 0x40,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x80, 0x09, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x08, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x79, 0xe7, 0xb1, 0xce, 0x73, 0xd8, 0xbc, 0x39, 0x2b, 0x1c, 0x70, 0x18, 0x63, 0xd8, 0x00,
  0x00, 0x09, 0x26, 0x49, 0x24, 0x8b, 0x24, 0xb2, 0x49, 0x2d, 0xa2, 0x48, 0x24, 0x93, 0x6c, 0x00,
  0x00, 0x19, 0x14, 0x4b, 0xe4, 0xfa, 0x24, 0xa2, 0x49, 0x28, 0xbe, 0x48, 0x20, 0x8a, 0x44, 0x00,
  0x00, 0x49, 0x14, 0x4a, 0x04, 0x82, 0x24, 0xa2, 0x49, 0x28, 0xa0, 0x48, 0x20, 0x8a, 0x44, 0x00,
  0x00, 0x49, 0x14, 0x49, 0x24, 0x9a, 0x24, 0xa2, 0x49, 0x2d, 0xb2, 0x48, 0x24, 0x92, 0x44, 0x00,
  0x00, 0x69, 0x14, 0x49, 0xc2, 0x72, 0x24, 0xa2, 0x39, 0x2f, 0x1c, 0x49, 0x18, 0x62, 0x44, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// Temperature icon
const unsigned char tempIcon[] PROGMEM = {
  // This icon cover has been designed using resources from Flaticon.com
  0x00, 0x1e, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x71, 0x80, 0x00, 0x00, 0x60, 0xc0, 0x00,
  0x00, 0x60, 0xcf, 0x00, 0x00, 0x60, 0xc0, 0x00, 0x00, 0x60, 0xc0, 0x00, 0x00, 0x64, 0xce, 0x00,
  0x00, 0x6c, 0xce, 0x00, 0x00, 0x6c, 0xc0, 0x00, 0x00, 0x6c, 0xce, 0x00, 0x00, 0x6c, 0xcf, 0x00,
  0x00, 0x6c, 0xc0, 0x00, 0x00, 0x6c, 0xc0, 0x00, 0x00, 0x6c, 0xcf, 0x00, 0x00, 0x6c, 0xce, 0x00,
  0x00, 0x6c, 0xc0, 0x00, 0x00, 0x6c, 0xc0, 0x00, 0x00, 0xcc, 0xe0, 0x00, 0x01, 0x8c, 0x70, 0x00,
  0x01, 0x9f, 0x30, 0x00, 0x03, 0x33, 0x30, 0x00, 0x03, 0x31, 0x90, 0x00, 0x03, 0x31, 0x90, 0x00,
  0x03, 0x3b, 0x30, 0x00, 0x01, 0x9f, 0x30, 0x00, 0x01, 0x80, 0x60, 0x00, 0x00, 0xe0, 0xe0, 0x00,
  0x00, 0x7f, 0xc0, 0x00, 0x00, 0x1f, 0x00, 0x00
};

// Humidity icon
const unsigned char humiIcon[] PROGMEM = {
  // This icon cover has been designed using resources from Flaticon.com
  0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x0f, 0xc0, 0x00,
  0x00, 0x1f, 0xe0, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00, 0x3f, 0xf0, 0x00, 0x00, 0x7f, 0xf8, 0x00,
  0x00, 0xff, 0xfc, 0x00, 0x00, 0xff, 0xfc, 0x00, 0x01, 0xff, 0xfe, 0x00, 0x03, 0xe7, 0xff, 0x00,
  0x03, 0xc1, 0xff, 0x00, 0x07, 0x99, 0xef, 0x80, 0x07, 0x99, 0xc7, 0x80, 0x0f, 0xc1, 0x8f, 0xc0,
  0x0f, 0xe3, 0x1f, 0xc0, 0x0f, 0xfe, 0x3f, 0xc0, 0x0f, 0xfc, 0x7f, 0xc0, 0x0f, 0xf8, 0xff, 0xc0,
  0x0f, 0xf1, 0xff, 0xc0, 0x0f, 0xe3, 0x0f, 0xc0, 0x07, 0xc6, 0x07, 0xc0, 0x07, 0xce, 0x67, 0x80,
  0x07, 0xfe, 0x0f, 0x80, 0x03, 0xff, 0x0f, 0x00, 0x01, 0xff, 0xfe, 0x00, 0x00, 0xff, 0xfc, 0x00,
  0x00, 0x7f, 0xf8, 0x00, 0x00, 0x0f, 0xc0, 0x00
};

// Stopwatch icon
const unsigned char stopwatchIcon[] PROGMEM = {
  // This icon cover has been designed using resources from Flaticon.com
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00, 0x0f, 0xe0, 0x00,
  0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x00, 0x7f, 0xf9, 0x80,
  0x00, 0xe0, 0x1f, 0x80, 0x01, 0x87, 0x87, 0x00, 0x03, 0x1f, 0xe3, 0x00, 0x06, 0x3c, 0xf1, 0x80,
  0x06, 0x7c, 0xf9, 0x80, 0x0c, 0xfc, 0xfc, 0xc0, 0x0c, 0xfc, 0xfc, 0xc0, 0x0c, 0xfc, 0xfc, 0xc0,
  0x0c, 0xfc, 0xfc, 0xc0, 0x0c, 0xfc, 0x7c, 0xc0, 0x0c, 0xfe, 0x3c, 0xc0, 0x0c, 0xff, 0x1c, 0xc0,
  0x0c, 0xff, 0xbc, 0xc0, 0x06, 0x7f, 0xf9, 0x80, 0x07, 0x3f, 0xf3, 0x80, 0x03, 0x0f, 0xc3, 0x00,
  0x01, 0xc0, 0x0e, 0x00, 0x00, 0xf0, 0x1c, 0x00, 0x00, 0x7f, 0xf8, 0x00, 0x00, 0x1f, 0xe0, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

//* BATTERY INDICATOR ICONS *
const unsigned char batteryIndicator1[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00,
  0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
  0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80,
  0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80,
  0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xb0,
  0x38, 0x3f, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf8,
  0x38, 0x7f, 0xff, 0xc0, 0x6c, 0x0e, 0x26, 0x41, 0xe1, 0xc7, 0xdf, 0x9f, 0x7c, 0xcd, 0x83, 0xfc,
  0x38, 0xff, 0xff, 0xe0, 0x6c, 0x1f, 0x36, 0x41, 0xf1, 0xc7, 0xdf, 0x9f, 0x7e, 0x49, 0x83, 0xfc,
  0x38, 0xff, 0xff, 0xe0, 0x6c, 0x31, 0xb7, 0xc1, 0x31, 0x41, 0x06, 0x10, 0x66, 0x78, 0x83, 0xfc,
  0x38, 0xff, 0xff, 0xe0, 0x6c, 0x31, 0xbf, 0xc1, 0xe3, 0x61, 0x06, 0x1e, 0x7c, 0x30, 0x83, 0xfc,
  0x38, 0xff, 0xff, 0xe0, 0x0c, 0x31, 0x99, 0xc1, 0x33, 0xe1, 0x06, 0x10, 0x7c, 0x30, 0x03, 0xfc,
  0x38, 0xff, 0xff, 0xe0, 0x6f, 0x9f, 0x19, 0x81, 0xf3, 0xe1, 0x06, 0x1f, 0x6c, 0x31, 0x83, 0xfc,
  0x38, 0x7f, 0xff, 0xe0, 0x6f, 0x8e, 0x19, 0x81, 0xf6, 0x31, 0x06, 0x1f, 0x66, 0x31, 0x83, 0xfc,
  0x38, 0x7f, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfc,
  0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf8,
  0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80,
  0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80,
  0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
  0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
const unsigned char batteryIndicator2[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00,
  0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
  0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80,
  0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80,
  0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xb0,
  0x38, 0x3f, 0xff, 0x80, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf8,
  0x38, 0x7f, 0xff, 0xc1, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfc,
  0x38, 0xff, 0xff, 0xe3, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfc,
  0x38, 0xff, 0xff, 0xe3, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfc,
  0x38, 0xff, 0xff, 0xe3, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfc,
  0x38, 0xff, 0xff, 0xe3, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfc,
  0x38, 0xff, 0xff, 0xe3, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfc,
  0x38, 0x7f, 0xff, 0xe1, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfc,
  0x38, 0x7f, 0xff, 0xc1, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfc,
  0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf8,
  0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80,
  0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80,
  0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
  0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
const unsigned char batteryIndicator3[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00,
  0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
  0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80,
  0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80,
  0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xb0,
  0x38, 0x3f, 0xff, 0x80, 0xff, 0xfe, 0x03, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf8,
  0x38, 0x7f, 0xff, 0xc1, 0xff, 0xff, 0x07, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfc,
  0x38, 0xff, 0xff, 0xe3, 0xff, 0xff, 0x8f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfc,
  0x38, 0x7f, 0xff, 0xe3, 0xff, 0xff, 0x8f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfc,
  0x38, 0xff, 0xff, 0xe3, 0xff, 0xff, 0x8f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfc,
  0x38, 0xff, 0xff, 0xe3, 0xff, 0xff, 0x8f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfc,
  0x38, 0xff, 0xff, 0xe3, 0xff, 0xff, 0x8f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfc,
  0x38, 0x7f, 0xff, 0xe1, 0xff, 0xff, 0x87, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfc,
  0x38, 0x7f, 0xff, 0xc1, 0xff, 0xff, 0x07, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfc,
  0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf8,
  0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80,
  0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80,
  0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
  0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
const unsigned char batteryIndicator4[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00,
  0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
  0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xbf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80,
  0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80,
  0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xb0,
  0x38, 0x3f, 0xff, 0x80, 0xff, 0xfe, 0x03, 0xff, 0xf8, 0x0f, 0xff, 0xe0, 0x00, 0x00, 0x03, 0xf8,
  0x38, 0x7f, 0xff, 0xc1, 0xff, 0xff, 0x07, 0xff, 0xfc, 0x1f, 0xff, 0xf0, 0x00, 0x00, 0x03, 0xfc,
  0x38, 0xff, 0xff, 0xe3, 0xff, 0xff, 0x8f, 0xff, 0xfe, 0x3f, 0xff, 0xf8, 0x00, 0x00, 0x03, 0xfc,
  0x38, 0xff, 0xff, 0xe3, 0xff, 0xff, 0x8f, 0xff, 0xfe, 0x3f, 0xff, 0xf8, 0x00, 0x00, 0x03, 0xfc,
  0x38, 0xff, 0xff, 0xe3, 0xff, 0xff, 0x8f, 0xff, 0xfe, 0x3f, 0xff, 0xf8, 0x00, 0x00, 0x03, 0xfc,
  0x38, 0xff, 0xff, 0xe3, 0xff, 0xff, 0x8f, 0xff, 0xfe, 0x3f, 0xff, 0xf8, 0x00, 0x00, 0x03, 0xfc,
  0x38, 0xff, 0xff, 0xe3, 0xff, 0xff, 0x8f, 0xff, 0xfe, 0x3f, 0xff, 0xf8, 0x00, 0x00, 0x03, 0xfc,
  0x38, 0x7f, 0xff, 0xe1, 0xff, 0xff, 0x87, 0xff, 0xfe, 0x1f, 0xff, 0xf8, 0x00, 0x00, 0x03, 0xfc,
  0x38, 0x7f, 0xff, 0xc1, 0xff, 0xff, 0x07, 0xff, 0xfc, 0x1f, 0xff, 0xf0, 0x00, 0x00, 0x03, 0xfc,
  0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf8,
  0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80,
  0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80,
  0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
  0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
const unsigned char batteryIndicator5[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00,
  0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
  0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0x80,
  0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80,
  0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xb0,
  0x38, 0x3f, 0xff, 0x80, 0xff, 0xfe, 0x03, 0xff, 0xf8, 0x0f, 0xff, 0xe0, 0x3f, 0xff, 0x83, 0xf8,
  0x38, 0x7f, 0xff, 0xc1, 0xff, 0xff, 0x07, 0xff, 0xfc, 0x1f, 0xff, 0xf0, 0x7f, 0xff, 0xc3, 0xfc,
  0x38, 0xff, 0xff, 0xe3, 0xff, 0xff, 0x8f, 0xff, 0xfe, 0x3f, 0xff, 0xf8, 0xff, 0xff, 0xe3, 0xfc,
  0x38, 0xff, 0xff, 0xe3, 0xff, 0xff, 0x8f, 0xff, 0xfe, 0x3f, 0xff, 0xf8, 0xff, 0xff, 0xe1, 0xfc,
  0x38, 0xff, 0xff, 0xe3, 0xff, 0xff, 0x8f, 0xff, 0xfe, 0x3f, 0xff, 0xf8, 0xff, 0xff, 0xe3, 0xfc,
  0x38, 0xff, 0xff, 0xe3, 0xff, 0xff, 0x8f, 0xff, 0xfe, 0x3f, 0xff, 0xf8, 0xff, 0xff, 0xe3, 0xfc,
  0x38, 0xff, 0xff, 0xe3, 0xff, 0xff, 0x8f, 0xff, 0xfe, 0x3f, 0xff, 0xf8, 0xff, 0xff, 0xe3, 0xfc,
  0x38, 0x7f, 0xff, 0xe1, 0xff, 0xff, 0x87, 0xff, 0xfe, 0x1f, 0xff, 0xf8, 0x7f, 0xff, 0xe3, 0xfc,
  0x38, 0x7f, 0xff, 0xc1, 0xff, 0xff, 0x07, 0xff, 0xfc, 0x1f, 0xff, 0xf0, 0x7f, 0xff, 0xc3, 0xfc,
  0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf8,
  0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80,
  0x1d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80,
  0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
  0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
//*** BATTERY INDICATOR ICONS ***

//COUNTDOWN ICON
const unsigned char countdownIcon[] PROGMEM = {
  0x03, 0xff, 0xff, 0x00, 0x03, 0xff, 0xff, 0x00, 0x00, 0xc0, 0x04, 0x00, 0x01, 0x80, 0x06, 0x00,
  0x01, 0x80, 0x06, 0x00, 0x01, 0x80, 0x06, 0x00, 0x00, 0x80, 0x04, 0x00, 0x00, 0xc0, 0x0c, 0x00,
  0x00, 0x60, 0x18, 0x00, 0x00, 0x30, 0x38, 0x00, 0x00, 0x38, 0x70, 0x00, 0x00, 0x1c, 0xe0, 0x00,
  0x00, 0x0f, 0xc0, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00,
  0x00, 0x07, 0x80, 0x00, 0x00, 0x0f, 0xc0, 0x00, 0x00, 0x1c, 0xe0, 0x00, 0x00, 0x38, 0x70, 0x00,
  0x00, 0x30, 0x38, 0x00, 0x00, 0x63, 0x98, 0x00, 0x00, 0xcf, 0xcc, 0x00, 0x00, 0x8c, 0xe4, 0x00,
  0x01, 0x9c, 0xe6, 0x00, 0x01, 0x9f, 0xe6, 0x00, 0x01, 0x80, 0x06, 0x00, 0x00, 0x80, 0x04, 0x00,
  0x03, 0xff, 0xff, 0x00, 0x03, 0xff, 0xff, 0x00
};

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(buttonMode, INPUT);
  digitalWrite(buttonMode, HIGH);
  pinMode(buttonStart, INPUT);
  digitalWrite(buttonStart, HIGH);
  pinMode(buttonReset, INPUT);
  digitalWrite(buttonReset, HIGH);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  //initialize with the address 0x3D for the 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }

  if (openingScreen == 1) {
    //display.display(); //Display Adafruit logo
    display.clearDisplay();
    display.invertDisplay(true);
    display.drawBitmap(0, 0, logo1, 128, 64, WHITE);  // displaying opening logo 1
    display.display();
    delay(280);
    display.clearDisplay();
    display.invertDisplay(false);
    display.drawBitmap(0, 0, logo1, 128, 64, WHITE);  // displaying opening logo 1
    display.display();
    delay(280);
    display.clearDisplay();
    display.invertDisplay(true);
    display.drawBitmap(0, 0, logo1, 128, 64, WHITE);  // displaying opening logo 1
    display.display();
    delay(280);
    display.clearDisplay();
    display.invertDisplay(false);
    display.drawBitmap(0, 0, logo1, 128, 64, WHITE);  // displaying opening logo 1
    delay(600);
    display.clearDisplay();
    display.drawBitmap(0, 0, logo2, 128, 64, WHITE);  // displaying opening logo 2
    display.display();
    delay(2200);
    display.clearDisplay();
  }
}



//**START** ! THIS SECTION OF THE CODE IS REMIXED FROM UTSOURCE (https://www.instructables.com/Arduino-Based-Clock-Using-DS1307-Real-Time-Clock-R/) ! **START**
// CC BY-NC-SA 4.0
void print2digits(int number) {
  if (number >= 0 && number < 10) {
    display.write('0');
  }
  display.print(number);
}

void print6digits(int number) {
  if (number >= 0 && number < 100000) {
    display.write('0');
    if (number >= 0 && number < 10000) {
      display.write('0');
      if (number >= 0 && number < 1000) {
        display.write('0');
        if (number >= 0 && number < 100) {
          display.write('0');
          if (number >= 0 && number < 10) {
            display.write('0');
          }
        }
      }
    }
  }
  display.print(number);
}

void screen1() {
  display.clearDisplay();
  tmElements_t tm;
  if (RTC.read(tm)) {
    display.clearDisplay();
  }
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(20, 8);
  print2digits(tm.Hour);
  display.write(':');
  print2digits(tm.Minute);
  display.setTextSize(2);
  display.setCursor(5, 42);
  display.setTextSize(2);
  print2digits(tm.Day);
  display.write('/');
  print2digits(tm.Month);
  display.write('/');
  print2digits(tmYearToCalendar(tm.Year));
  display.display();
  if (digitalRead(buttonMode) == LOW) {
    buttonModeState = 1;
    delay(200);  // If it returns directly to the screen1 after clicking the button, increase this delay
  } else if (digitalRead(buttonStart) == LOW) {
    display.setRotation(2);
    delay(200);
  } else if (digitalRead(buttonReset) == LOW) {
    display.setRotation(0);
    delay(200);
  }
}
//**END** ! THIS SECTION OF THE CODE IS REMIXED FROM UTSOURCE (https://www.instructables.com/Arduino-Based-Clock-Using-DS1307-Real-Time-Clock-R/) ! **END**

void screen2() {
  temperature = dht.readTemperature() - 5;
  humidity = dht.readHumidity();
  display.clearDisplay();
  // temperature
  display.drawBitmap(13, 0, tempIcon, 30, 30, WHITE);
  display.setTextSize(3);
  display.setCursor(6, 40);
  display.print(temperature);
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.setTextSize(2);
  display.print("C");
  // humidity
  display.drawBitmap(82, 0, humiIcon, 30, 30, WHITE);
  display.setTextSize(3);
  display.setCursor(77, 40);
  display.print(humidity);
  display.setTextSize(2);
  display.print("%");
  display.display();
  if (digitalRead(buttonMode) == LOW) {
    buttonModeState = 2;
    delay(200);  // If it returns directly to the screen2 after clicking the button, increase this delay
  }
}

void screen3() {
  display.clearDisplay();
  display.drawBitmap(0, 20, stopwatchIcon, 30, 30, WHITE);
  display.setTextSize(1);
  display.setCursor(24, 4);
  display.print("- Kronometre -");
  display.setTextSize(3);
  display.setCursor(36, 25);
  display.write("--");
  display.setTextSize(3);
  display.write(":");
  display.setTextSize(3);
  display.write("--");
  display.display();
  if (digitalRead(buttonMode) == LOW) {
    buttonModeState = 4;
    delay(200);  // If it returns directly to the screen2 after clicking the button, increase this delay
  } else if (digitalRead(buttonStart) == LOW) {
    buttonStartState = 1;
    delay(200);
  }
}

void coreStopwatch() {
  secondsStopwatch++;
  delay(1000);
  if (secondsStopwatch > 59) {
    secondsStopwatch = 0;
    minutesStopwatch++;
  }
  //Serial.println(secondsStopwatch); //uncomment when debugging
  display.clearDisplay();
  display.drawBitmap(0, 20, stopwatchIcon, 30, 30, WHITE);
  display.setTextSize(1);
  display.setCursor(24, 4);
  display.print("- Kronometre -");
  display.setTextSize(3);
  display.setCursor(36, 25);
  print2digits(minutesStopwatch);
  display.setTextSize(3);
  display.write(":");
  display.setTextSize(3);
  print2digits(secondsStopwatch);
  display.display();
  if (digitalRead(buttonMode) == LOW) {
    buttonModeState = 4;
    delay(200);  // If it returns directly to the screen2 after clicking the button, increase this delay
    buttonStartState = 0;
    //secondsStopwatch = 0; minutesStopwatch = 0; //resetting stopwatch
  } else if (digitalRead(buttonStart) == LOW) {
    buttonStartState = 2;
    delay(100);
  }
}

void stopwatchPaused() {
  Serial.println(secondsStopwatch);
  display.clearDisplay();
  display.drawBitmap(0, 20, stopwatchIcon, 30, 30, WHITE);
  display.setTextSize(1);
  display.setCursor(24, 4);
  display.print("- Kronometre -");
  display.setTextSize(3);
  display.setCursor(36, 25);
  print2digits(minutesStopwatch);
  display.setTextSize(3);
  display.write(":");
  display.setTextSize(3);
  print2digits(secondsStopwatch);
  display.display();
  if (digitalRead(buttonMode) == LOW) {
    buttonModeState = 4;
    delay(200);  // If it returns directly to the screen2 after clicking the button, increase this delay
    //buttonStartState = 0;
    //secondsStopwatch = 0; minutesStopwatch = 0; //resetting stopwatch
  } else if (digitalRead(buttonStart) == LOW) {
    buttonStartState = 1;
    delay(100);
  } else if (digitalRead(buttonReset) == LOW) {
    secondsStopwatch = 0;
    minutesStopwatch = 0;
    delay(50);
  }
}

void randomNumber() {
//  display.clearDisplay();
//  display.setTextColor(WHITE);
//  display.setTextSize(1);
//  display.setCursor(22, 0);
//  display.write("- Rand. Number -");
//  display.setTextSize(3);
//  display.setCursor(11, 25);
//  print6digits(randNumber);
//  display.display();
//  delay(100);
 if (digitalRead(buttonMode) == LOW) {
   buttonModeState = 5;
   delay(200); // If it returns directly to the screen1 after clicking the button, increase this delay
 }
 else if (digitalRead(buttonStart) == LOW) {
   delay(100);
 }
}


void sinavaKalanGun() {

  // nisanKalan = 30 - tm.Day;
  // sinavaKalan = nisanKalan + 48;
  // display.clearDisplay();
  // display.drawBitmap(0, 21, countdownIcon, 30, 30, WHITE);
  // display.setTextColor(WHITE);
  // display.setTextSize(1);
  // display.setCursor(12, 0);
  // display.write("Remaining time:");
  // display.setTextSize(3);
  // display.setCursor(38, 25);
  // print2digits(sinavaKalan);
  // display.setTextSize(2);
  // display.setCursor(68, 30);
  // display.write(" gun");
  // display.display();
  // delay(100);
  if (digitalRead(buttonMode) == LOW) {
    buttonModeState = 6;
    delay(200);  // If it returns directly to the screen1 after clicking the button, increase this delay
  } else if (digitalRead(buttonStart) == LOW) {
    delay(100);
  }
}


void remainingBattery() {
  int deger = analogRead(A0);
  float volt = (deger * referans) / 1023;
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(8, 0);
  display.write("- Remaining Battery -");
  display.setTextSize(2);
  display.setCursor(28, 15);
  display.print(volt);
  display.write(" v");
  if (volt <= 2.8) {
    display.drawBitmap(0, 40, batteryIndicator1, 128, 22, WHITE);
  } else if (volt <= 3) {
    display.drawBitmap(0, 40, batteryIndicator2, 128, 22, WHITE);
  } else if (volt <= 3.2) {
    display.drawBitmap(0, 40, batteryIndicator3, 128, 22, WHITE);
  } else if (volt <= 3.4) {
    display.drawBitmap(0, 40, batteryIndicator4, 128, 22, WHITE);
  } else {
    display.drawBitmap(0, 40, batteryIndicator5, 128, 22, WHITE);
  }
  display.display();
  delay(100);
  if (digitalRead(buttonMode) == LOW) {
    buttonModeState = 0;
    delay(200);  // If it returns directly to the screen1 after clicking the button, increase this delay
  }
}

void loop() {
  if (buttonModeState == 6 && remainingBatteryScreen == 1) {
    remainingBattery();
  } else if (buttonModeState == 5 && dateCountdownScreen == 1) {
    sinavaKalanGun();
  } else if (buttonModeState == 4) {
    randomNumber();
  } else if (buttonModeState == 2 && buttonStartState == 0 && stopwatchScreen == 1) {
    screen3();
  } else if (buttonModeState == 2 && buttonStartState == 1) {
    coreStopwatch();
  } else if (buttonModeState == 2 && buttonStartState == 2) {
    stopwatchPaused();
  } else if (buttonModeState == 1 && temperatureAndHumadityScreen == 1) {
    screen2();
  } else {
    screen1();
  }

  //Serial.println(buttonModeState); //uncomment when debugging
}