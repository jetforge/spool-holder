#include "ApiClient.h"
#include "Settings.h"
#include <ArduinoJson.h>
#include "Config.h"

#define API_HOST "my.jetforge.app"


ApiClient::ApiClient(Settings & settings) {
    this->settings = & settings;
    client.setTimeout(5000);
    client.setCACert(rootCa);
}

bool ApiClient::refreshAccessToken()
{
    const int port = 443;
    String refreshEndpoint = "/api/spool/token";

    if (!client.connect(API_HOST, port))
    {
        return false;
    }

    DynamicJsonDocument refreshPayload(256);
    refreshPayload["refresh_token"] = String(API_ID) + ":" + String(settings->refreshToken);

    String payloadString;
    serializeJson(refreshPayload, payloadString);

    String request = String("POST ") + refreshEndpoint + " HTTP/1.1\r\n" +
                     "Host: " + API_HOST + "\r\n" +
                     "Content-Type: application/json\r\n" +
                     "Content-Length: " + payloadString.length() + "\r\n" +
                     "Connection: close\r\n\r\n" +
                     payloadString;

    client.print(request);

    while (client.connected() || client.available())
    {
        String line = client.readStringUntil('\n');
        if (line == "\r")
        {
            break;
        }
    }

    String responseString;
    int braceCount = 0;
    bool jsonStarted = false;
    while (client.available())
    {
        char c = client.read();

        if (c == '{')
        {
            jsonStarted = true;
            braceCount++;
        }

        if (jsonStarted)
        {
            responseString += c;
        }

        if (c == '}')
        {
            braceCount--;
            if (braceCount == 0 && jsonStarted) {
                break;
            }
        }
    }

    Serial.println(responseString);

    client.stop();

    DynamicJsonDocument refreshResponse(256);
    DeserializationError error = deserializeJson(refreshResponse, responseString);
    
    if (error || !refreshResponse.containsKey("access_token"))
    {
        return false;
    }

    String accessToken = refreshResponse["access_token"].as<String>();
    strncpy(settings->accessToken, accessToken.c_str(), sizeof(settings->accessToken) - 1);
    settings->accessToken[sizeof(settings->accessToken) - 1] = '\0';

    String refreshToken = refreshResponse["refresh_token"].as<String>();
    strncpy(settings->refreshToken, refreshToken.c_str(), sizeof(settings->refreshToken) - 1);
    settings->refreshToken[sizeof(settings->refreshToken) - 1] = '\0';

    settings->commit();
    
    return true;
}

bool ApiClient::postRequest(const char* endpoint, const DynamicJsonDocument& jsonPayload, DynamicJsonDocument& jsonResponse)
{
    const int port = 443;
    String fullEndpoint = String("/api/spool") + endpoint;

    if (!client.connect(API_HOST, port))
    {
        return false;
    }

    String payloadString;
    serializeJson(jsonPayload, payloadString);

    String authHeader = String("Bearer ");
    
    authHeader += String(API_ID);
    
    authHeader += ":";
    
    authHeader += String(settings->accessToken);

    // Prepare HTTP request
    String request = String("POST ") + fullEndpoint + " HTTP/1.1\r\n" +
                     "Host: " + API_HOST + "\r\n" +
                     "Content-Type: application/json\r\n" +
                     "Authorization: " + authHeader + "\r\n" +
                     "Content-Length: " + payloadString.length() + "\r\n" +
                     "Connection: close\r\n\r\n" +
                     payloadString;

    client.print(request);

    while (client.connected() || client.available())
    {
        String line = client.readStringUntil('\n');
        if (line == "\r")
        {
          break;
        }
    }

    String responseString;

    int braceCount = 0;
    bool jsonStarted = false;
    while (client.available())
    {
        char c = client.read();

        if (c == '{')
        {
            jsonStarted = true;
            braceCount++;
        }

        if (jsonStarted)
        {
            responseString += c;
        }

        if (c == '}')
        {
            braceCount--;
            if (braceCount == 0 && jsonStarted) {
                break;
            }
        }
    }

    client.stop();

    Serial.println(responseString);

    DeserializationError error = deserializeJson(jsonResponse, responseString);
    
    bool result = true;

    if (error)
    {
        result = false;
    }

    if (result && jsonResponse.containsKey("error"))
    {
      JsonObject errorObj = jsonResponse["error"];
      String errorClass = errorObj["class"].as<String>();
      int errorCode = errorObj["code"].as<int>();
      String errorMessage = errorObj["message"].as<String>();

      if (errorClass == "Oms\\SpoolHolderApi\\Exception" && (errorCode == 2 || errorCode == 1))
      {
        if (refreshAccessToken()) 
        {
            return postRequest(endpoint, jsonPayload, jsonResponse);
        }
        Serial.println("failed to refresh");
        return false;
      }

      result = false;
    }

    return result;
}
