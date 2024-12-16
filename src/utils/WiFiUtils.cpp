#include "utils/WiFiUtils.h"
#include <vector>
#if defined(ESP32)
#include <esp_task_wdt.h>
#endif
#define TRIESONE 25 // количество попыток подключения к одной сети из несколких
#define TRIES 40    // количество попыток подключения сети если она одна
/* 
IPAddress stringToIp(String strIp)
{
  IPAddress ip;
  ip.fromString(strIp);
  return ip;
} */


void routerConnect()
{
#if  !defined LIBRETINY  
#if defined(esp32c6_4mb) || defined(esp32c6_8mb)
  WiFi.setAutoReconnect(false);
#else
  WiFi.setAutoConnect(false);
#endif
  WiFi.persistent(false);
#endif
/*     String s_staip = "192.168.2.62";
    String s_gateway = "192.168.2.1";
    String s_netmask = "255.255.255.0";
    String s_dns = "192.168.2.1";
    SerialPrint("i", "WIFI", "Use static IP");
    WiFi.config(stringToIp(s_staip), stringToIp(s_gateway), stringToIp(s_netmask), stringToIp(s_dns));
    // bool config(IPAddress local_ip, IPAddress gateway, IPAddress subnet, IPAddress dns1 = (uint32_t)0x00000000, IPAddress dns2 = (uint32_t)0x00000000);
    SerialPrint("i", "WIFI", "Static IP: " + s_staip);
    SerialPrint("i", "WIFI", "Gateway: " + s_gateway);
    SerialPrint("i", "WIFI", "Netmask: " + s_netmask);
    SerialPrint("i", "WIFI", "DNS: " + s_dns); */
  WiFi.mode(WIFI_STA);
  byte triesOne = TRIESONE;

  std::vector<String> _ssidList;
  std::vector<String> _passwordList;
  jsonReadArray(settingsFlashJson, "routerssid", _ssidList);
  jsonReadArray(settingsFlashJson, "routerpass", _passwordList);
  if (_ssidList.size() > 1)
    triesOne = TRIES;

  if (_passwordList.size() == 0 && _ssidList[0] == "" && _passwordList[0] == "")
  {
    #ifndef LIBRETINY
    WiFi.begin();
    #endif
  }
  else
  {
    WiFi.begin(_ssidList[0].c_str(), _passwordList[0].c_str());
#if defined (ESP32)
    WiFi.setTxPower(WIFI_POWER_19_5dBm);
#elif defined (ESP8266)
    WiFi.setOutputPower(20.5);
#endif
    String _ssid;
    String _password;
    for (int8_t i = 0; i < _ssidList.size(); i++)
    {
      _ssid = _ssid + _ssidList[i] + "; ";
    }
    for (int8_t i = 0; i < _passwordList.size(); i++)
    {
      _password = _password + _passwordList[i] + "; ";
    }
    SerialPrint("i", "WIFI", "ssid list: " + _ssid);
    SerialPrint("i", "WIFI", "pass list: " + _password);
  }
  for (size_t i = 0; i < _ssidList.size(); i++)
  {
    triesOne = TRIESONE;
    if (WiFi.status() == WL_CONNECTED)
      break;
    WiFi.begin(_ssidList[i].c_str(), _passwordList[i].c_str());
    SerialPrint("i", "WIFI", "ssid connect: " + _ssidList[i]);
    SerialPrint("i", "WIFI", "pass connect: " + _passwordList[i]);
    while (--triesOne && WiFi.status() != WL_CONNECTED)
    {
//            SerialPrint("i", "WIFI", ": " + String((int)WiFi.status()));
#ifdef ESP8266
      if (WiFi.status() == WL_CONNECT_FAILED || WiFi.status() == WL_WRONG_PASSWORD)
#else
      if (WiFi.status() == WL_CONNECT_FAILED)
#endif
      {
        SerialPrint("E", "WIFI", "password is not correct");
        triesOne = 1;
        jsonWriteInt(errorsHeapJson, "passer", 1);
        break;
      }
#if defined(ESP32)
      //SerialPrint("i", "Task", "Resetting WDT...");
       #if !defined(esp32c6_4mb) && !defined(esp32c6_8mb) //TODO esp32-c6 переписать esp_task_wdt_init
      esp_task_wdt_reset();
      #endif
#endif
      Serial.print(".");
      delay(1000);
    }
    Serial.println("");
  }

  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("");
    startAPMode();
  }
  else
  {
    Serial.println("");
#ifdef LIBRETINY
    SerialPrint("i", "WIFI", "http://" + ipToString(WiFi.localIP()));
    jsonWriteStr(settingsFlashJson, "ip", ipToString(WiFi.localIP()));
#else
    SerialPrint("i", "WIFI", "http://" + WiFi.localIP().toString());
    jsonWriteStr(settingsFlashJson, "ip", WiFi.localIP().toString());
#endif

    mqttInit();
  }
  SerialPrint("i", F("WIFI"), F("Network Init"));
}

bool startAPMode()
{
  SerialPrint("i", "WIFI", "AP Mode");

  WiFi.disconnect();
  WiFi.mode(WIFI_AP);

  String _ssidAP = jsonReadStr(settingsFlashJson, "apssid");
  String _passwordAP = jsonReadStr(settingsFlashJson, "appass");
  if (_passwordAP == "")
    WiFi.softAP(_ssidAP.c_str());
  else
    WiFi.softAP(_ssidAP.c_str(), _passwordAP.c_str());
  IPAddress myIP = WiFi.softAPIP();
#ifdef LIBRETINY
  SerialPrint("i", "WIFI", "AP IP: " + ipToString(myIP));
  jsonWriteStr(settingsFlashJson, "ip", ipToString(myIP));
#else
  SerialPrint("i", "WIFI", "AP IP: " + myIP.toString());
  jsonWriteStr(settingsFlashJson, "ip", myIP.toString());
#endif
  if (jsonReadInt(errorsHeapJson, "passer") != 1)
  {
    ts.add(
        WIFI_SCAN, 30 * 1000,
        [&](void *)
        {
          std::vector<String> jArray;
          jsonReadArray(settingsFlashJson, "routerssid", jArray);
          for (int8_t i = 0; i < jArray.size(); i++)
          {
            SerialPrint("i", "WIFI", "scanning for " + jArray[i]);
          }
          if (RouterFind(jArray))
          {
            ts.remove(WIFI_SCAN);
            WiFi.scanDelete();
            routerConnect();
          }
        },
        nullptr, true);
  }
  return true;
}
#if defined (LIBRETINY)
boolean RouterFind(std::vector<String> jArray)
{
  bool res = false;
  int n = WiFi.scanComplete();
  SerialPrint("i", "WIFI", "scan result: " + String(n, DEC));

  if (n == -2)
  { // Сканирование не было запущено, запускаем
    SerialPrint("i", "WIFI", "start scanning");
   n =  WiFi.scanNetworks(false, false); // async, show_hidden
     SerialPrint("i", "WIFI", "scan result: " + String(n, DEC));
  }

  else if (n == -1)
  { // Сканирование все еще выполняется
    SerialPrint("i", "WIFI", "scanning in progress");
  }

  else if (n == 0)
  { // ни одна сеть не найдена
    SerialPrint("i", "WIFI", "no networks found");
   n =  WiFi.scanNetworks(false, false);
     SerialPrint("i", "WIFI", "scan result: " + String(n, DEC));
  }

  if (n > 0)
  {
    for (int8_t i = 0; i < n; i++)
    {
      for (int8_t k = 0; k < jArray.size(); k++)
      {
        if (WiFi.SSID(i) == jArray[k])
        {
          res = true;
        }
      }
      // SerialPrint("i", "WIFI", (res ? "*" : "") + String(i, DEC) + ") " + WiFi.SSID(i));
      jsonWriteStr_(ssidListHeapJson, String(i), WiFi.SSID(i));

      // String(WiFi.RSSI(i)
    }
  }
  SerialPrint("i", "WIFI", ssidListHeapJson);
  WiFi.scanDelete();
  return res;
}
#elif  defined (ESP8266) || defined (ESP32)
boolean RouterFind(std::vector<String> jArray)
{
  bool res = false;
  int n = WiFi.scanComplete();
  SerialPrint("i", "WIFI", "scan result: " + String(n, DEC));

  if (n == -2)
  { // Сканирование не было запущено, запускаем
    SerialPrint("i", "WIFI", "start scanning");
    WiFi.scanNetworks(true, false); // async, show_hidden
  }

  else if (n == -1)
  { // Сканирование все еще выполняется
    SerialPrint("i", "WIFI", "scanning in progress");
  }

  else if (n == 0)
  { // ни одна сеть не найдена
    SerialPrint("i", "WIFI", "no networks found");
    WiFi.scanNetworks(true, false);
  }
  else if (n > 0)
  {
    for (int8_t i = 0; i < n; i++)
    {
      for (int8_t k = 0; k < jArray.size(); k++)
      {
        if (WiFi.SSID(i) == jArray[k])
        {
          res = true;
        }
      }
      // SerialPrint("i", "WIFI", (res ? "*" : "") + String(i, DEC) + ") " + WiFi.SSID(i));
      jsonWriteStr_(ssidListHeapJson, String(i), WiFi.SSID(i));

      // String(WiFi.RSSI(i)
    }
  }
  SerialPrint("i", "WIFI", ssidListHeapJson);
  WiFi.scanDelete();
  return res;
}
#endif

boolean isNetworkActive() {
    return WiFi.status() == WL_CONNECTED;
}

uint8_t getNumAPClients() {
    return WiFi.softAPgetStationNum();
}

uint8_t RSSIquality() {
    uint8_t res = 0;
    if (isNetworkActive()) {
        int rssi = WiFi.RSSI();
        if (rssi >= -50) {
            res = 6;  //"Excellent";
        } else if (rssi < -50 && rssi >= -60) {
            res = 5;  //"Very good";
        } else if (rssi < -60 && rssi >= -70) {
            res = 4;  //"Good";
        } else if (rssi < -70 && rssi >= -80) {
            res = 3;  //"Low";
        } else if (rssi < -80 && rssi > -100) {
            res = 2;  //"Very low";
        } else if (rssi <= -100) {
            res = 1;  //"No signal";
        }
    }
    return res;
}

#ifdef LIBRETINY
String httpGetString(HTTPClient &http)
{
      String payload = "";
        int len = http.getSize();
        uint8_t buff[128] = { 0 };
        WiFiClient * stream = http.getStreamPtr();

                // read all data from server
                while(http.connected() && (len > 0 || len == -1)) {
                    // get available data size
                    size_t size = stream->available();

                    if(size) {
                        // read up to 128 byte
                        int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));

                        // write it to Serial
                     //   Serial.write(buff,c);
                        
                        //payload += String((char*)buff);
                        char charBuff[c + 1]; // Create a character array with space for null terminator
                        memcpy(charBuff, buff, c); // Copy the data to the character array
                        charBuff[c] = '\0'; // Null-terminate the character array
                        payload += String(charBuff); // Append the character array to the payload

                        if(len > 0) {
                            len -= c;
                        }
                    }
                    delay(1);
                }      
                return payload;
}
#endif