#include "Global.h"
#include "classes/IoTItem.h"

class HttpGet : public IoTItem
{
public:
    HttpGet(String parameters) : IoTItem(parameters)
    {
    }

    void sendHttpPOST(String url, String msg)
    {
        if (isNetworkActive())
        {

            WiFiClient client;
            HTTPClient http;
            http.begin(client, url);
            http.addHeader("Content-Type", "application/x-www-form-urlencoded");
            String httpRequestData = msg;
            int httpResponseCode = http.POST(httpRequestData);
            String payload = http.getString();
            SerialPrint("<-", F("HttpPOST"), "URL: " + url + ", msg: " + msg);
            SerialPrint("->", F("HttpPOST"), "URL: " + url + ", server: " + httpResponseCode);

            if (httpResponseCode > 0)
            {
                SerialPrint("->", F("HttpPOST"), "msg from server: " + (String)payload.c_str());
                setValue(payload);
            }
            http.end();
        }
    }
    void sendHttpGET(String url)
    {
        WiFiClient client;
        HTTPClient http;
#if defined ESP8266
        if (!http.begin(client, url))
        {
#elif defined ESP32
        if (!http.begin(url))
        {
#endif

            SerialPrint("I", F("HttpGet"), "connection failed  ");
        }
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        int httpResponseCode = http.GET();
        String payload = http.getString();
        SerialPrint("<-", F("HttpGET"), "URL: " + url);
        SerialPrint("->", F("HttpGET"), "URL: " + url + ", server: " + httpResponseCode);
        if (httpResponseCode > 0)
        {
            SerialPrint("->", F("HttpGET"), "msg from server: " + (String)payload.c_str());
            setValue(payload);
        }
        http.end();
    }

    IoTValue execute(String command, std::vector<IoTValue> &param)
    {
        if (param.size() > 0)
        {
            if (command == "get")
            {
                if (param.size())
                {
                    sendHttpGET(param[0].val());
                }
            }
            else if (command == "post")
            {
                if (param.size())
                {
                    sendHttpPOST(param[0].val(), param[1].val());
                }
            }
        }
        return {};
    }

    ~HttpGet(){};
};

void *getAPI_HttpGet(String subtype, String param)
{
    if (subtype == F("HttpGet"))
    {
        return new HttpGet(param);
    }
    else
    {
        return nullptr;
    }
}