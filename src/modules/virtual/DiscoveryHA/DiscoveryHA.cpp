#include "Global.h"
#include "classes/IoTDiscovery.h"

class DiscoveryHA : public IoTDiscovery
{
private:
    String _topic = "";
    bool sendOk = false;
    // bool topicOk = false;
    bool HA = false;
    String ChipId = getChipId();

public:
    DiscoveryHA(String parameters) : IoTDiscovery(parameters)
    {
        _topic = jsonReadStr(parameters, "topic");
        if (_topic && _topic != "" && _topic != "null")
        {
            HA = true;
            HATopic = _topic;
        }
        if (mqttIsConnect() && HA)
        {
#if defined ESP32
// пре реконнекте вызывается отправка всех виджетов из файла layout.json в HA
// на ESP8266 мало оперативки и это можно делать только до момента конфигурации
//           mqttReconnect();
#endif
            // sendOk = true;
            // mqttSubscribeExternal(_topic);
        }
    }
    /*
        void onMqttRecive(String &topic, String &msg)
        {
            if (!HA)
                return;

            if (msg.indexOf("HELLO") == -1)
            {
                String dev = selectToMarkerLast(topic, "/");
                dev.toUpperCase();
                dev.replace(":", "");
                if (_topic != topic)
                {
                    //  SerialPrint("i", "ExternalMQTT", _id + " not equal: " + topic + " msg: " + msg);
                    return;
                }
                // обработка топика, на который подписались
            }
        } */

    void doByInterval()
    {
        /*         // периодически проверяем связь с MQTT брокером и если она появилась, то подписываемся на нужный топик
                if (mqttIsConnect() && !sendOk && &&topicOk)
                {
                    sendOk = true;
                    getlayoutHA();
                    publishRetain(mqttRootDevice + "/state", "{\"status\":\"online\"}");
                    //mqttSubscribeExternal(_topic);
                }

                // если нет коннектас брокером, то сбрасываем флаг подписки, что бы при реконекте заново подписаться
                if (!mqttIsConnect())
                    sendOk = false; */
    }
    /*     String getMqttExterSub()
        {
            return _topic;
        } */

    void mqttSubscribeDiscovery()
    {
        if (HA)
        {
            getlayoutHA();
            publishRetain(mqttRootDevice + "/state", "{\"status\":\"online\"}");
        }
    }

    void getlayoutHA()
    {
        if (HA)
        {
            auto file = seekFile("layout.json");
            if (!file)
            {
                SerialPrint("E", F("MQTT"), F("no file layout.json"));
                return;
            }
            size_t size = file.size();
            DynamicJsonDocument doc(size * 2);
            DeserializationError error = deserializeJson(doc, file);
            if (error)
            {
                SerialPrint("E", F("MQTT"), error.f_str());
                jsonWriteInt(errorsHeapJson, F("jse3"), 1); // Ошибка чтения json файла с виджетами при отправки в mqtt
            }
            int i = 0;
            // String HATopic = jsonReadStr(settingsFlashJson, F("HomeAssistant"));
            JsonArray arr = doc.as<JsonArray>();
            for (JsonVariant value : arr)
            {
                String dev = selectToMarkerLast(value["topic"].as<String>(), "/");
                dev.replace(":", "");
                String HAjson = "";
                HAjson = "{\"availability\":[{\"topic\": \"" + mqttRootDevice + "/state\",\"value_template\": \"{{ value_json.status }}\"}],\"availability_mode\": \"any\",";
                HAjson = HAjson + " \"device\": {\"identifiers\": [\"" + mqttRootDevice + value["page"].as<String>() + "\"],";
                HAjson = HAjson + " \"name\": \"  " + value["page"].as<String>() + "\"},";
                HAjson = HAjson + " \"name\": \"" + value["descr"].as<String>() + "\",";
                HAjson = HAjson + " \"state_topic\": \"" + value["topic"].as<String>() + "/status\",";
                HAjson = HAjson + " \"icon\": \"hass:none\",";

                // сенсоры
                if (value["name"].as<String>() == "anydataTmp")
                {
                    HAjson = HAjson + " \"value_template\": \"{{  float( value_json.status, default = 0) | default }}\",";
                    HAjson = HAjson + " \"unique_id\": \"" + mqttRootDevice + dev + "\",";
                    HAjson = HAjson + " \"unit_of_measurement\": \"°C\"";
                }
                else if (value["name"].as<String>() == "anydataHum")
                {
                    HAjson = HAjson + " \"value_template\": \"{{  float( value_json.status, default = 0) | default  }}\",";
                    HAjson = HAjson + " \"unique_id\": \"" + mqttRootDevice + dev + "\",";
                    HAjson = HAjson + " \"unit_of_measurement\": \"%\"";
                }
                else if (value["name"].as<String>() == "anydataMm")
                {
                    HAjson = HAjson + " \"value_template\": \"{{  float( value_json.status, default = 0) | default  }}\",";
                    HAjson = HAjson + " \"unique_id\": \"" + mqttRootDevice + dev + "\",";
                    HAjson = HAjson + " \"unit_of_measurement\": \"mm\"";
                }
                else if (value["name"].as<String>() == "anydataBar")
                {
                    HAjson = HAjson + " \"value_template\": \"{{  float( value_json.status, default = 0) | default  }}\",";
                    HAjson = HAjson + " \"unique_id\": \"" + mqttRootDevice + dev + "\",";
                    HAjson = HAjson + " \"unit_of_measurement\": \"Bar\"";
                }
                else if (value["name"].as<String>() == "anydataPpm")
                {
                    HAjson = HAjson + " \"value_template\": \"{{  float( value_json.status, default = 0) | default  }}\",";
                    HAjson = HAjson + " \"unique_id\": \"" + mqttRootDevice + dev + "\",";
                    HAjson = HAjson + " \"unit_of_measurement\": \"ppm\"";
                }

                // ввод числаФВ
                else if (value["name"].as<String>() == "inputDgt")
                {
                    HAjson = HAjson + " \"value_template\": \"{{  float( value_json.status, default = 0) | default }}\",";
                    HAjson = HAjson + " \"unique_id\": \"" + mqttRootDevice + dev + "\",";
                    HAjson = HAjson + " \"command_topic\": \"" + value["topic"].as<String>() + "/control\",";
                    HAjson = HAjson + " \"mode\": \"box\",";
                    HAjson = HAjson + " \"min\": " + -1000000 + ",";
                    HAjson = HAjson + " \"max\": " + 1000000 + "";
                }
                // ввод текста inputTxt
                else if (value["name"].as<String>() == "inputTxt")
                {
                    HAjson = HAjson + " \"value_template\": \"{{ value_json.status | default  }}\",";
                    HAjson = HAjson + " \"unique_id\": \"" + mqttRootDevice + dev + "\",";
                    HAjson = HAjson + " \"command_topic\": \"" + value["topic"].as<String>() + "/control\"";
                }
                // переключатель
                else if (value["name"].as<String>() == "toggle")
                {
                    HAjson = HAjson + " \"value_template\": \"{{ value_json.status | default  }}\",";
                    HAjson = HAjson + " \"unique_id\": \"" + mqttRootDevice + dev + "\",";
                    HAjson = HAjson + " \"command_topic\": \"" + value["topic"].as<String>() + "/control\",";
                    HAjson = HAjson + " \"device_class\": \"switch\",";
                    HAjson = HAjson + " \"payload_off\": " + 0 + ",";
                    HAjson = HAjson + " \"payload_on\": " + 1 + ",";
                    HAjson = HAjson + " \"state_off\": " + 0 + ",";
                    HAjson = HAjson + " \"state_on\": " + 1 + "";
                }
                else
                {
                    HAjson = HAjson + " \"value_template\": \"{{ value_json.status | default  }}\",";
                    HAjson = HAjson + " \"unique_id\": \"" + mqttRootDevice + dev + "\"";
                }

                HAjson = HAjson + " }";
                //                 "has_entity_name" : false,

                // SerialPrint("E", F("MQTT"), HAjson);
                // текст
                if (value["widget"].as<String>() == "anydata")
                {

                    if (!publishRetain(HATopic + "/sensor/" + ChipId + "/" + dev + "/config", HAjson))
                    {
                        SerialPrint("E", F("MQTT"), F("Failed publish  data to homeassitant"));
                    }
                }

                // ввод числа
                if (value["name"].as<String>() == "inputDgt")
                {

                    if (!publishRetain(HATopic + "/number/" + ChipId + "/" + dev + "/config", HAjson))
                    {
                        SerialPrint("E", F("MQTT"), F("Failed publish  data to homeassitant"));
                    }
                }
                // ввод текста inputTxt
                if (value["name"].as<String>() == "inputTxt")
                {

                    if (!publishRetain(HATopic + "/text/" + ChipId + "/" + dev + "/config", HAjson))
                    {
                        SerialPrint("E", F("MQTT"), F("Failed publish  data to homeassitant"));
                    }
                }
                // переключатель
                if (value["name"].as<String>() == "toggle")
                {

                    if (!publishRetain(HATopic + "/switch/" + ChipId + "/" + dev + "/config", HAjson))
                    {
                        SerialPrint("E", F("MQTT"), F("Failed publish  data to homeassitant"));
                    }
                }

                i++;
            }
            file.close();

            publishRetain(mqttRootDevice + "/state", "{\"status\":\"online\"}");

            for (std::list<IoTItem *>::iterator it = IoTItems.begin(); it != IoTItems.end(); ++it)
            {
                if ((*it)->iAmLocal)
                {
                    publishStatusMqtt((*it)->getID(), (*it)->getValue());
                    (*it)->onMqttWsAppConnectEvent();
                }
            }
        }
    }

    IoTDiscovery *getHADiscovery()
    {
        if (HA)
            return this;
        else
            return nullptr;
    }
    ~DiscoveryHA() {};
};

void *getAPI_DiscoveryHA(String subtype, String param)
{
    if (subtype == F("DiscoveryHA"))
    {
        return new DiscoveryHA(param);
    }
    else
    {
        return nullptr;
    }
}
