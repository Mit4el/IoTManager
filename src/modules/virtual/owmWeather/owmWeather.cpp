#include "Global.h"
#include "classes/IoTItem.h"
#include <ArduinoJson.h>
#include "NTP.h"
// long prevWeatherMillis = millis() - 60001;
//  TODO Зачем так много???
StaticJsonDocument<JSON_BUFFER_SIZE * 2> Weatherdoc1;

extern IoTGpio IoTgpio;
class owmWeather : public IoTItem
{
private:
    // описание параметров передаваемых из настроек датчика из веба

    String _location;
    String _param;
    //  long interval;
    String _API_key;
    String _city = "";
    String _lat = "";
    String _lon = "";
    String _lang = "";
    bool _debug = false;

public:
    owmWeather(String parameters) : IoTItem(parameters)
    {
        _API_key = jsonReadStr(parameters, "API_key");
        //    _ID_sity = jsonReadStr(parameters, "ID_sity");
        _city = jsonReadStr(parameters, "city");
        _lon = jsonReadStr(parameters, "lon");
        _lat = jsonReadStr(parameters, "lat");
        _lang = jsonReadStr(parameters, "lang");
        _param = jsonReadStr(parameters, "param");
        jsonRead(parameters, "debug", _debug);
        long interval;
        jsonRead(parameters, F("int"), interval); // в минутах
        setInterval(interval * 60);
    }

    void getWeather()
    {
        String ret;

        if (isNetworkActive())
        {
            String urlReq;
            if (_city != "")
            {
                // Для нового API
                //   request = "http://api.openweathermap.org/geo/1.0/direct?q=" _сity + "&limit=1&appid=" + _API_key;
                // Устарело, но пока работает
                urlReq = "http://api.openweathermap.org/data/2.5/weather?q=" + _city + "&units=metric&lang=" + _lang + "&appid=" + _API_key;
            }
            else
            {
                // Получаем название города по координатам. Новый API
                // request = "http://api.openweathermap.org/geo/1.0/reverse?lat=" + _lat + "&lon=" + _lon + "&limit=1&appid=" + _API_key;
                //[0].local_names:{ru:Москва,...}
                urlReq = "http://api.openweathermap.org/data/2.5/weather?lat=" + _lat + "&lon=" + _lon + "&units=metric&lang=" + _lang + "&appid=" + _API_key;
            }

            WiFiClient client;
            HTTPClient http;
            String payload;
            bool find = false;
            http.setTimeout(500);
            http.begin(client, urlReq); // urlCurrent
            // http.begin(client, "http://api.openweathermap.org/data/2.5/weather?id=" + _ID_sity + "&appid=" + _API_key + "&units=metric");
            http.addHeader("Content-Type", "application/x-www-form-urlencoded");
            int httpCode = http.GET();

            if (httpCode > 0)
            {
                ret = httpCode;

                if (httpCode == HTTP_CODE_OK)
                {
                    payload = http.getString();

                    deserializeJson(Weatherdoc1, payload);
                    ret += payload;
                }
            }
            else
            {
                ret = http.errorToString(httpCode).c_str();
            }
            if (_debug)
            {
                SerialPrint("<-", F("getOwmWeather"), urlReq);
                SerialPrint("->", F("getOwmWeather"), "server: " + ret);
            }
            http.end();
        }
    }

    void doByInterval()
    {

        getWeather();
        if (jsonReadStr(Weatherdoc1["main"], "temp", true) != "null")
        {

            publishNew("main", "temp");
            publishNew("main", "pressure");
            publishNew("main", "humidity");
            publishNew("wind", "speed");
            publishNew("wind", "deg");
            publishNew("clouds", "all");
            publishNew("weather", "main");
            publishNew("weather", "description");
            publishNew("weather", "icon");
            publishNew("sys", "sunrise");
            publishNew("sys", "sunset");
            publishNew("", "name");
            String strtmp;
            if (_param == "temp")
            {
               strtmp = jsonReadStr(Weatherdoc1["main"], "temp", true);
            }
            else if (_param == "pressure")
            {
                 strtmp = jsonReadStr(Weatherdoc1["main"], "pressure", true);
            }
            else if (_param == "humidity")
            {
                 strtmp = jsonReadStr(Weatherdoc1["main"], "humidity", true);
            }
            else if (_param == "speed")
            {
                 strtmp = jsonReadStr(Weatherdoc1["wind"], "speed", true);
            }
            else if (_param == "deg")
            {
                 strtmp = jsonReadStr(Weatherdoc1["wind"], "deg", true);
            }
            else if (_param == "all")
            {
                 strtmp = jsonReadStr(Weatherdoc1["clouds"], "all", true);
            }
            else if (_param == "main")
            {
                 strtmp = jsonReadStr(Weatherdoc1["weather"][0], "main", true);
            }
            else if (_param == "description")
            {
                 strtmp = jsonReadStr(Weatherdoc1["weather"][0], "description", true);
            }
            else if (_param == "icon")
            {
                 strtmp = jsonReadStr(Weatherdoc1["weather"][0], "icon", true);
            }
            else if (_param == "sunrise")
            {
                 strtmp = getTimeDotFormatedFromUnix(std::atoll(jsonReadStr(Weatherdoc1["sys"], "sunrise", true).c_str()));
            }
            else if (_param == "sunset")
            {
                 strtmp = getTimeDotFormatedFromUnix(std::atoll(jsonReadStr(Weatherdoc1["sys"], "sunset", true).c_str()));
            }
            else if (_param == "sunset")
            {
                 strtmp = Weatherdoc1["name"].as<String>();
            }
            // value.isDecimal = false;
            setValue(strmp);
           // regEvent(value.valS, "owmWeather");
        }
    }

    IoTValue execute(String command, std::vector<IoTValue> &param)
    {
        if (command == "get")
        {
            // getWeather();
            doByInterval();
        }

        return {};
    }

    // проверяем если пришедшее значение отличается от предыдущего регистрируем событие
    static void publishNew(String root, String param)
    {
        IoTItem *tmp = findIoTItem("wea_" + param);
        if (!tmp)
            return;

        if (root == "weather")
        { // ☔⛅❄🌤🌥🌦🌧🌨🌩🌫🌞
            String icn = Weatherdoc1[root][0]["icon"].as<String>();
            if (icn == "01d" || icn == "01n")
                icn = "🌞";
            else if (icn == "02d" || icn == "02n")
                icn = "🌤";
            else if (icn == "03d" || icn == "03n")
                icn = "🌥";
            else if (icn == "04d" || icn == "04n")
                icn = "🌫";
            else if (icn == "09d" || icn == "09n")
                icn = "🌧";
            else if (icn == "10d" || icn == "10n")
                icn = "🌦";
            else if (icn == "11d" || icn == "11n")
                icn = "🌩";
            else if (icn == "13d" || icn == "13n")
                icn = "❄";
            else
                icn = "";
            if (Weatherdoc1[root][0][param].as<String>() != tmp->getIoTValue().val())
            {
                if (param == "description")
                    tmp->setValue(Weatherdoc1[root][0][param].as<String>() + icn, true);
                else
                    tmp->setValue(Weatherdoc1[root][0][param].as<String>(), true);
            }
        }
        else if (root == "")
        {
            if (Weatherdoc1[param].as<String>() != tmp->getIoTValue().val())
            {
                tmp->setValue(Weatherdoc1[param].as<String>(), true);
            }
        }
        else if (root == "sys")
        {
            if (Weatherdoc1[root][param].as<String>() != tmp->getIoTValue().val())
            {
                tmp->setValue(getTimeDotFormatedFromUnix(std::atoll(jsonReadStr(Weatherdoc1[root], param, true).c_str())), true);
            }
        }
        else
        {
            if (Weatherdoc1[root][param].as<String>() != tmp->getIoTValue().val())
            {
                tmp->setValue(Weatherdoc1[root][param].as<String>(), true);
            }
        }
    }

    ~owmWeather(){};
};

void *getAPI_owmWeather(String subtype, String param)
{
    if (subtype == F("owmWeather"))
    {
        return new owmWeather(param);
    }
    else
    {
        return nullptr;
    }
}
