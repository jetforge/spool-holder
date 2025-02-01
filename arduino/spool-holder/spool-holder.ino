#include "Config.h"
#include <WiFi.h>
#include "ApiClient.h"
#include "Settings.h"
#include <ArduinoJson.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <esp_task_wdt.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>


const unsigned char logo [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf8, 0x07, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x3f, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xc0, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x81, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x03, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x1f, 0x01, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x3e, 0x07, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x7c, 0x1f, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x78, 0x3f, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xf0, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xf0, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xf0, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xe1, 0xe1, 0xe0, 0x3f, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x01, 0xe1, 0xe1, 0xe0, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x01, 0xe1, 0xe3, 0xc1, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x01, 0xe1, 0xc3, 0xc3, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x01, 0xe1, 0xc3, 0xc3, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x01, 0xe1, 0xc3, 0xc7, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x01, 0xe1, 0xc3, 0xc7, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x01, 0xe1, 0xc3, 0xc3, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x01, 0xe1, 0xc3, 0xc3, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x01, 0xe1, 0xe3, 0xc1, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x01, 0xe1, 0xe1, 0xe0, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xe1, 0xe1, 0xe0, 0x3f, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xe0, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xf0, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xf0, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x78, 0x3f, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x7c, 0x1f, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x3e, 0x07, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x1f, 0x01, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x07, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x03, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x81, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xc0, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x3f, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x07, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

#define RFID_RST_PIN         17      // RFID module RST pin
#define RFID_SS_PIN          5       // RDIF module SS pin

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels


SemaphoreHandle_t dataMutex;

SemaphoreHandle_t apiMutex;

Settings settings;

ApiClient apiClient(settings);

Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

MFRC522 rfid(RFID_SS_PIN, RFID_RST_PIN);   // RFID module object
MFRC522::MIFARE_Key key;         // Key object
MFRC522::StatusCode status;      // Status object

uint32_t currentUid = 0;

String pendingTagId = ""; // to write to tag
String commitTagId = "";  // to commit via api

bool wifiConnected = false;

unsigned long previousMillis = 0;  // Store the last time the action was performed

int interval = 5000;

bool spoolInserted = false;
bool isAuthorized = false;
String fatalError = "";
bool invalidateSpoolInfo = false;
String tagStatus = "empty";
int materialWeight = 0;
String materialTitle = "";
bool skipTagStatusUpdate = false;
String probeTagId = "";
bool resetRfid;


void setup() {
  Serial.begin(115200);

  pinMode(25, INPUT); // motion
  pinMode(26, INPUT); // insert
  pinMode(12, INPUT_PULLUP);

  ledcAttach(4, 5000, 8);

  esp_task_wdt_config_t wdt_config = {
      .timeout_ms = 30000,  // 30 seconds
      .idle_core_mask = (1 << portNUM_PROCESSORS) - 1,
      .trigger_panic = true
  };

  esp_task_wdt_init(&wdt_config);
  esp_task_wdt_add(NULL);

  if (!display.begin(0x3C, true)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
  }

  dataMutex = xSemaphoreCreateMutex();
  apiMutex = xSemaphoreCreateMutex();

  settings.begin();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  SPI.begin();

  rfidReset();

  for (byte i = 0; i < 6; i++) { // Наполняем ключ
    key.keyByte[i] = 0xFF;       // Ключ по умолчанию 0xFFFFFFFFFFFF
  }

  delay(500);
  display.clearDisplay();
  display.drawBitmap(0, 0, logo, 128, 64, SH110X_WHITE);
  display.display();


}


void statusRequest(void* parameter) {
  Serial.println("start status task");

  if (xSemaphoreTake(apiMutex, portMAX_DELAY)) {
    Serial.println("status task got api mutex");
    
    DynamicJsonDocument jsonRequest(128);
    DynamicJsonDocument jsonResponse(1024);
  
    jsonRequest["spool_is_inserted"] = spoolInserted;
  
    apiClient.postRequest("/status", jsonRequest, jsonResponse);
  
    if (xSemaphoreTake(dataMutex, portMAX_DELAY)) {
      if (jsonResponse.containsKey("interval")) {
        interval = jsonResponse["interval"].as<int>();
      }
      
      if (jsonResponse.containsKey("material_title")) {
        materialTitle = jsonResponse["material_title"].as<String>();
        materialWeight = jsonResponse["weight"].as<int>();
      } else {
        materialTitle = "";
      }
    
      if (jsonResponse.containsKey("status") && !skipTagStatusUpdate) {
        tagStatus = jsonResponse["status"].as<String>();
      }
    
      invalidateSpoolInfo = true;
      skipTagStatusUpdate = false;
      xSemaphoreGive(dataMutex);
    }
  
    xSemaphoreGive(apiMutex);
  }
  
  Serial.println("end status task");
  vTaskDelete(NULL);


}

void confirmRequest(void* parameter) {
  Serial.println("start confirm task");

  if (xSemaphoreTake(apiMutex, portMAX_DELAY)) {
    Serial.println("confirm task got api mutex");

    DynamicJsonDocument confirmJsonRequest(8);
    DynamicJsonDocument confirmJsonResponse(64);

    apiClient.postRequest("/confirm", confirmJsonRequest, confirmJsonResponse);
    xSemaphoreGive(apiMutex);
  }

  Serial.println("end confirm task");
  vTaskDelete(NULL);
}

void tagCommitRequest(void* parameter) {
  Serial.println("start tag commit task");
  
  if (xSemaphoreTake(apiMutex, portMAX_DELAY)) {
    Serial.println("tag commit task got api mutex");

    DynamicJsonDocument jsonRequest(128);
    DynamicJsonDocument jsonResponse(256);

    if (xSemaphoreTake(dataMutex, portMAX_DELAY)) {
      jsonRequest["tag_id"] = commitTagId;
      jsonRequest["spool_is_inserted"] = spoolInserted;
      xSemaphoreGive(dataMutex);
    }

    apiClient.postRequest("/tag/commit", jsonRequest, jsonResponse);
    
    xSemaphoreGive(apiMutex);
  }

  Serial.println("end tag commit task");
  vTaskDelete(NULL);

}

void tagProbeRequest(void* parameter) {
  
  Serial.println("start tag probe task");
  
  if (xSemaphoreTake(apiMutex, portMAX_DELAY)) {
    Serial.println("tag probe task got api mutex");

    DynamicJsonDocument jsonRequest(256);
    DynamicJsonDocument jsonResponse(2048);

    if (xSemaphoreTake(dataMutex, portMAX_DELAY)) {
      jsonRequest["tag_id"] = probeTagId;
      jsonRequest["spool_is_inserted"] = spoolInserted;

      xSemaphoreGive(dataMutex);
    }

    apiClient.postRequest("/tag/probe", jsonRequest, jsonResponse);

    if (xSemaphoreTake(dataMutex, portMAX_DELAY)) {
      if (jsonResponse.containsKey("tag_id")) {
          String newTagId = jsonResponse["tag_id"].as<String>();
  
          Serial.print("got tag id from server ");
          Serial.println(newTagId);
    
          if (newTagId != probeTagId) {
            Serial.println("will write new tag id");
            resetRfid = true;
            pendingTagId = newTagId;
          } else {
            Serial.println("tag exists!");
          }
        }  
  
        if (jsonResponse.containsKey("weight")) {
          materialTitle = jsonResponse["material_title"].as<String>();
          materialWeight = jsonResponse["weight"].as<int>();
          invalidateSpoolInfo = true;
        }
      
      xSemaphoreGive(dataMutex);
    }
    
    xSemaphoreGive(apiMutex);
  }

  Serial.println("end tag probe task");
  vTaskDelete(NULL);

}

void authRequest()
{
  DynamicJsonDocument jsonRequest(256);
  DynamicJsonDocument jsonResponse(1024);

  String authToken = String(API_ID) + ":" + String(API_AUTH_KEY);

  jsonRequest["auth_token"] = authToken;
  
  if (!apiClient.postRequest("/auth", jsonRequest, jsonResponse))
  {
    fatalError = "Auth request failed";
  }
  else
  {
    if (jsonResponse.containsKey("error") || !jsonResponse.containsKey("refresh_token"))
    {
      fatalError = "Auth failed. Invalid auth id or auth secret.";
    }
    else
    {
      String accessToken = jsonResponse["access_token"].as<String>();
      strncpy(settings.accessToken, accessToken.c_str(), sizeof(settings.accessToken) - 1);
      settings.accessToken[sizeof(settings.accessToken) - 1] = '\0';

      String refreshToken = jsonResponse["refresh_token"].as<String>();
      strncpy(settings.refreshToken, refreshToken.c_str(), sizeof(settings.refreshToken) - 1);
      settings.refreshToken[sizeof(settings.refreshToken) - 1] = '\0';

      strncpy(settings.authKey, API_AUTH_KEY, sizeof(settings.authKey) - 1);
      settings.authKey[sizeof(settings.authKey) - 1] = '\0';
      
      Serial.print("access token: ");
      Serial.println(settings.accessToken);
      Serial.print("refresh token: ");
      Serial.println(settings.refreshToken);
      Serial.print("auth key: ");
      Serial.println(settings.authKey);

      settings.commit();

    }
  }

  if (fatalError != "")
  {
    showStatus(fatalError);
  }

}

void loop()
{
  esp_task_wdt_reset();

  bool buttonStatus = digitalRead(12) == LOW;

  handleLedFade();

  if (fatalError != "")
  {
    if (buttonStatus)
    {
      display.clearDisplay();
      display.display();
      ledcWrite(4, 0);
      delay(200);
      esp_restart();
    }
    delay(50);
    return;
  }


  if (xSemaphoreTake(dataMutex, portMAX_DELAY))
  {
    bool _spoolInserted = digitalRead(26);
  
    if (!_spoolInserted && spoolInserted)
    {
      materialTitle = "";
      invalidateSpoolInfo = true;
    }

    spoolInserted = _spoolInserted;
    xSemaphoreGive(dataMutex);
  }

  if (WiFi.status() != WL_CONNECTED)
  {
    if (wifiConnected)
    {
      showStatus("WiFi is not connected");
      wifiConnected = false;
    }
    delay(1000);
    return;
  }

  if (!isAuthorized)
  {
    if (strcmp(settings.authKey, API_AUTH_KEY) == 0)
    {
      isAuthorized = true;
    }
    else
    {
      showStatus("Performing first time authorization...");
      authRequest();
      delay(500);
      return;
    }
  }  

  if (xSemaphoreTake(dataMutex, portMAX_DELAY)) {
    if (tagStatus == "detected" && materialTitle != "" && buttonStatus) {
      ledcWrite(4, 0);
  
      xTaskCreatePinnedToCore(
        confirmRequest,    // Task function
        "ConfirmRequest",  // Name of the task
        8192,             // Stack size (bytes)
        NULL,             // Task input parameters (none)
        1,                // Priority
        NULL,             // Task handle
        1                 // Core to run the task on (0 or 1)
      );
  
      tagStatus = "confirmed";
      invalidateSpoolInfo = true;
      skipTagStatusUpdate = true;
    }

    xSemaphoreGive(dataMutex);
  }

  if (!wifiConnected) {
    wifiConnected = true;
  }

  bool _invalidateSpoolInfo = false;
  if (xSemaphoreTake(dataMutex, portMAX_DELAY)) {
    _invalidateSpoolInfo = invalidateSpoolInfo;
    invalidateSpoolInfo = false;
    xSemaphoreGive(dataMutex);
  }

  if (_invalidateSpoolInfo) {
    showSpoolInfo();
  }
    
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    xTaskCreatePinnedToCore(
      statusRequest,    // Task function
      "StatusRequest",  // Name of the task
      8192,             // Stack size (bytes)
      NULL,             // Task input parameters (none)
      1,                // Priority
      NULL,             // Task handle
      1                 // Core to run the task on (0 or 1)
    );    
  }


  if (xSemaphoreTake(dataMutex, portMAX_DELAY)) {
    if (resetRfid) {
      rfidReset();
      resetRfid = false;
    }
    xSemaphoreGive(dataMutex);
  }

  if (!rfid.PICC_IsNewCardPresent()) {
    delay(50);
    return;
  }

  if (!rfid.PICC_ReadCardSerial()) {
    delay(50);
    return;
  }

  uint32_t uid = readUIDToUint32();

  if (uid == currentUid && pendingTagId != "") {
    if (writeUUIDToRFID(pendingTagId.c_str())) {

      if (xSemaphoreTake(dataMutex, portMAX_DELAY)) {
        commitTagId = pendingTagId;
        xSemaphoreGive(dataMutex);
      }
      
      xTaskCreatePinnedToCore(
        tagCommitRequest,    // Task function
        "TagCommitRequest",  // Name of the task
        8192,             // Stack size (bytes)
        NULL,             // Task input parameters (none)
        1,                // Priority
        NULL,             // Task handle
        1                 // Core to run the task on (0 or 1)
      );

      pendingTagId = "";

      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
    } else {
      rfidReset();
      delay(100);
    }

    return;
  }

  DynamicJsonDocument jsonRequest(1024);
  DynamicJsonDocument jsonResponse(2048);

  pendingTagId = "";
  currentUid = uid;

  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

  // Print the card type
  Serial.print("Card Type: ");
  Serial.println(rfid.PICC_GetTypeName(piccType));

  if (readUUIDFromRFID(probeTagId)) {
    Serial.println("read success");
    Serial.println(probeTagId);

    xTaskCreatePinnedToCore(
      tagProbeRequest,    // Task function
      "TagProbeRequest",  // Name of the task
      8192,             // Stack size (bytes)
      NULL,             // Task input parameters (none)
      1,                // Priority
      NULL,             // Task handle
      1                 // Core to run the task on (0 or 1)
    );    

  } else {
    Serial.println("read failed");
  }


  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  delay(50);
}

void showSpoolInfo() {
  
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);

  if (xSemaphoreTake(dataMutex, portMAX_DELAY)) {
    if (spoolInserted && materialTitle != "") {
      printWrappedText(materialTitle, 0, 0, SCREEN_WIDTH, 1);
  
      display.setCursor(0, 30); // Adjust position as needed
      if (tagStatus == "confirmed") {
        display.setTextSize(2);
        display.print(materialWeight);
        display.println(" g");
      }
      if (tagStatus == "detected") {
        display.setTextSize(1);
        display.println("Same spool?");
      }
    } else {
      if (tagStatus == "detected" && spoolInserted) {
        printWrappedText("Initialize spool in JetForge", 0, 0, SCREEN_WIDTH, 1);
      } else {
        display.drawBitmap(0, 0, logo, 128, 64, SH110X_WHITE);
      }
    }
    xSemaphoreGive(dataMutex);
  }
  
  display.display();
}



void handleLedFade() {
  static int brightness = 0;
  static int fadeAmount = 10; // Adjust fade step size
  static bool increasing = true;

  if (xSemaphoreTake(dataMutex, portMAX_DELAY)) {
    bool blink = false;

    if (tagStatus == "detected" && materialTitle != "")
    {
      blink = true;
      fadeAmount = 10;
    }

    if (fatalError != "") {
      blink = true;
      fadeAmount = 20;
    }
    
    if (blink) {
      ledcWrite(4, brightness);
  
      if (increasing) {
        brightness += fadeAmount;
        if (brightness >= 240) {
          increasing = false;
        }
      } else {
        brightness -= fadeAmount;
        if (brightness <= 0) {
          increasing = true;
        }
      }
    } else {
      ledcWrite(4, 0); // Turn off the LED
    }
    xSemaphoreGive(dataMutex);
  }
}



void printWrappedText(String text, int x, int y, int maxWidth, int textSize) {
  display.setTextSize(textSize);
  int cursorX = x;
  int cursorY = y;
  int charWidth = 6 * textSize; // Default character width is 6 pixels for most fonts
  int lineHeight = 8 * textSize; // Default line height is 8 pixels for most fonts
  String word = "";

  for (size_t i = 0; i < text.length(); i++) {
    char c = text[i];

    if (c == ' ' || c == '\n' || i == text.length() - 1) {
      // Add the current character to the word
      if (i == text.length() - 1 && c != ' ' && c != '\n') {
        word += c;
      }

      // Calculate the word's width in pixels
      int wordWidth = word.length() * charWidth;

      // Check if the word fits in the current line
      if (cursorX + wordWidth > maxWidth) {
        // Move to the next line
        cursorX = x;
        cursorY += lineHeight;

        // Check if the next line exceeds the screen height
        if (cursorY >= SCREEN_HEIGHT) {
          break; // Stop drawing if text exceeds the screen height
        }
      }

      // Print the word
      display.setCursor(cursorX, cursorY);
      display.print(word);

      // Move the cursor forward
      cursorX += wordWidth + charWidth;

      // Reset the word
      word = "";

      // Handle line break
      if (c == '\n') {
        cursorX = x;
        cursorY += lineHeight;
      }
    } else {
      word += c; // Add the character to the current word
    }
  }
}

void showStatus(String text) {
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  printWrappedText(text, 0, 0, SCREEN_WIDTH, 1);
  display.display();
}


void rfidReset() {
  rfid.PCD_Reset();
  rfid.PCD_Init();
}

uint32_t readUIDToUint32() {
  if (rfid.uid.size != 4) {
    Serial.println("Error: UID is not 4 bytes.");
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
    return 0;
  }

  uint32_t uid = 0;
  for (byte i = 0; i < 4; i++) {
    uid <<= 8;
    uid |= rfid.uid.uidByte[i];
  }

  return uid;
}

bool writeUUIDToRFID(const char* uuid) {
  // Validate input UUID length
  byte uuidLength = strlen(uuid);
  if (uuidLength != 36) {
    Serial.println("Error: UUID must be exactly 36 characters.");
    return false;
  }

  byte blockData[18]; // Ensure the buffer is large enough for MIFARE_Write
  byte blockNum;

  // Loop to write UUID across 3 blocks
  for (int i = 0; i < 3; i++) {
    blockNum = 4 + i; // Writing to blocks 4, 5, and 6

    // Prepare block data
    memset(blockData, 0, sizeof(blockData)); // Clear block data
    for (int j = 0; j < 16; j++) {
      int index = i * 16 + j;
      if (index < uuidLength) {
        blockData[j] = uuid[index];
      } else {
        blockData[j] = 0x00; // Pad with zeros if needed
      }
    }

    // Authenticate for the block
    MFRC522::StatusCode status = rfid.PCD_Authenticate(
                                   MFRC522::PICC_CMD_MF_AUTH_KEY_A,
                                   blockNum,
                                   &key,
                                   &(rfid.uid)
                                 );
    if (status != MFRC522::STATUS_OK) {
      Serial.print("Authentication failed for block ");
      Serial.print(blockNum);
      Serial.print(": ");
      Serial.println(rfid.GetStatusCodeName(status));
      return false;
    }

    // Write data to the block
    status = rfid.MIFARE_Write(blockNum, blockData, 16);
    if (status != MFRC522::STATUS_OK) {
      Serial.print("Write failed for block ");
      Serial.print(blockNum);
      Serial.print(": ");
      Serial.println(rfid.GetStatusCodeName(status));
      return false;
    }

    Serial.print("Block ");
    Serial.print(blockNum);
    Serial.println(" written successfully.");
  }

  return true; // Return true if all blocks are written successfully
}

bool readUUIDFromRFID(String &uuid) {
  byte buffer[18]; // Ensure the buffer is large enough for MIFARE_Read
  byte blockNum;
  uuid = ""; // Clear the UUID string

  // Loop through 3 blocks to reconstruct the 36-character UUID
  for (int i = 0; i < 3; i++) {
    blockNum = 4 + i; // Reading blocks 4, 5, and 6

    // Authenticate for the block
    MFRC522::StatusCode status = rfid.PCD_Authenticate(
                                   MFRC522::PICC_CMD_MF_AUTH_KEY_A,
                                   blockNum,
                                   &key,
                                   &(rfid.uid)
                                 );
    if (status != MFRC522::STATUS_OK) {
      Serial.print("Authentication failed for block ");
      Serial.print(blockNum);
      Serial.print(": ");
      Serial.println(rfid.GetStatusCodeName(status));
      return false;
    }

    // Initialize buffer size
    byte size = sizeof(buffer);

    // Read data from the block
    status = rfid.MIFARE_Read(blockNum, buffer, &size);
    if (status != MFRC522::STATUS_OK) {
      Serial.print("Read failed for block ");
      Serial.print(blockNum);
      Serial.print(": ");
      Serial.println(rfid.GetStatusCodeName(status));
      return false;
    }

    // Append block data to UUID string (convert to characters)
    for (int j = 0; j < 16; j++) {
      if (buffer[j] == 0x00) break; // Stop if padding (optional)
      uuid += (char)buffer[j];
    }
  }

  // Check if the UUID is empty
  if (uuid.length() == 0) {
    Serial.println("Authentication successful, but no data found.");
    return true;
  }

  // Ensure the UUID is exactly 36 characters
  //    if (uuid.length() != 36) {
  //        Serial.println("Error: UUID length mismatch.");
  //        return false;
  //        uuid = "";
  //    }

  return true; // Successfully read a valid UUID
}
