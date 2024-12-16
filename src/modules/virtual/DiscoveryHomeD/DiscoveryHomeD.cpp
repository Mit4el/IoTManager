#include "Global.h"
#include "classes/IoTDiscovery.h"
// #include "MqttDiscovery.h"
class DiscoveryHomeD : public IoTDiscovery
{
private:
    String _topic = "";
    bool sendOk = false;
    // bool topicOk = false;
    bool HOMEd = false;
    int _names = 0;
    String ChipId = getChipId();
    String esp_id = ChipId;

public:
    DiscoveryHomeD(String parameters) : IoTDiscovery(parameters)
    {
        _topic = jsonReadStr(parameters, "topic");
        _names = jsonReadInt(parameters, "names");
        if (_topic && _topic != "" && _topic != "null")
        {
            HOMEd = true;
            HOMEdTopic = _topic;
        }
        if (_names)
        {
            esp_id = jsonReadStr(settingsFlashJson, F("name"));
            jsonWriteInt(settingsFlashJson, F("HOMEd_names"), 1);
        }
        else
        {
            jsonWriteInt(settingsFlashJson, F("HOMEd_names"), 0);
        }

        if (mqttIsConnect() && HOMEd)
        {
            mqttReconnect();
            // sendOk = true;
            // mqttSubscribeExternal(_topic);
        }
    }

    void onMqttRecive(String &topic, String &payloadStr)
    {
        if (!HOMEd)
            return;

        if (payloadStr.indexOf("HELLO") == -1)
        {
            /*             String dev = selectToMarkerLast(topic, "/");
                        dev.toUpperCase();
                        dev.replace(":", "");
                        if (_topic != topic)
                        {
                            //  SerialPrint("i", "ExternalMQTT", _id + " not equal: " + topic + " msg: " + msg);
                            return;
                        } */
            // обработка топика, на который подписались
            if (topic.indexOf(F("/td/custom")) != -1)
            {

                // обрабатываем команды из HOMEd
                StaticJsonDocument<200> doc;
                deserializeJson(doc, payloadStr);
                for (JsonPair kvp : doc.as<JsonObject>())
                {

                    String key = kvp.key().c_str();
                    String value = kvp.value().as<const char *>();
                    if (key.indexOf(F("status_")) != -1)
                    {
                        key.replace("status_", "");
                        if (value == "on")
                        {
                            generateOrder(key, "1");
                        }
                        else if (value == "off")
                        {
                            generateOrder(key, "0");
                        }
                        else if (value == "toggle")
                        {
                            String val = (String)(1 - getItemValue(key).toInt());
                            generateOrder(key, val);
                        }
                    }
                    else
                    {
                        if (!value)
                        {
                            float val = kvp.value();
                            generateOrder(key, (String)(val));
                        }
                        else
                        {
                            generateOrder(key, value);
                        }
                    }
                }

                SerialPrint("i", F("=>MQTT"), "Msg from HOMEd: " + payloadStr);
            }
        }
    }

    void doByInterval()
    {
        /*         // периодически проверяем связь с MQTT брокером и если она появилась, то подписываемся на нужный топик
                if (mqttIsConnect() && !sendOk && topicOk)
                {
                    sendOk = true;
                    publishRetain(_topic + "/device/custom/" + esp_id, "{\"status\":\"online\"}");
                    String HOMEdsubscribeTopic = _topic + "/td/custom/" + esp_id;
                    // mqtt.subscribe(HOMEdsubscribeTopic.c_str());
                    mqttSubscribeExternal(HOMEdsubscribeTopic);
                }

                // если нет коннектас брокером, то сбрасываем флаг подписки, что бы при реконекте заново подписаться
                if (!mqttIsConnect())
                    sendOk = false; */
    }

    void publishStatusHOMEd(const String &topic, const String &data)
    {
        String path_h = HOMEdTopic + "/fd/custom/" + esp_id;
        String json_h = "{}";
        if (topic != "onStart")
        {
            if (data.toInt() == 1)
            {
                jsonWriteStr(json_h, "status_" + topic, "on");
            }
            else if (data.toInt() == 0)
            {
                jsonWriteStr(json_h, "status_" + topic, "off");
            }
            if (data.toFloat())
            {
                jsonWriteFloat(json_h, topic, data.toFloat());
            }
            else
            {
                jsonWriteStr(json_h, topic, data);
            }
            mqtt.publish(path_h.c_str(), json_h.c_str(), false);
        }
    }

    void mqttSubscribeDiscovery()
    {
        if (HOMEd)
        {
            deleteFromHOMEd();
            getlayoutHOMEd();
            publishRetain(HOMEdTopic + "/device/custom/" + esp_id, "{\"status\":\"online\"}");
            String HOMEdsubscribeTopic = HOMEdTopic + "/td/custom/" + esp_id;
            mqtt.subscribe(HOMEdsubscribeTopic.c_str());
        }
    }

    void getlayoutHOMEd()
    {
        if (HOMEd)
        {
            String devName = jsonReadStr(settingsFlashJson, F("name"));

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
            // String path = jsonReadStr(settingsFlashJson, F("HOMEd"));
            JsonArray arr = doc.as<JsonArray>();
            String HOMEdJSON = "";
            HOMEdJSON = "{\"action\":\"updateDevice\",";
            HOMEdJSON = HOMEdJSON + "\"device\":\"" + ChipId + "\",";
            HOMEdJSON = HOMEdJSON + "\"data\":{";
            HOMEdJSON = HOMEdJSON + "\"active\": true,";
            HOMEdJSON = HOMEdJSON + "\"cloud\": false,";
            HOMEdJSON = HOMEdJSON + "\"discovery\": false,";
            HOMEdJSON = HOMEdJSON + "\"id\":\"" + ChipId + "\",";
            HOMEdJSON = HOMEdJSON + "\"name\":\"" + devName + "\",";
            HOMEdJSON = HOMEdJSON + "\"real\":true,";
            HOMEdJSON = HOMEdJSON + "\"exposes\": [";
            String options = "";
            for (JsonVariant value : arr)
            {
                String name = value["descr"];
                String device = selectToMarkerLast(value["topic"].as<String>(), "/");
                //            String id = ChipId + "-" + device;
                String expose = value["name"];
                if (value["name"].as<String>() == "toggle")
                {
                    HOMEdJSON = HOMEdJSON + "\"switch_" + device + "\",";
                }
                else
                {
                    HOMEdJSON = HOMEdJSON + "\"" + device + "\",";
                }

                if (value["name"].as<String>() == "anydataTmp")
                {
                    // HOMEdJSON = HOMEdJSON + "\"temperature_" + device + "\",";
                    options = options + "\"" + device + "\":{\"type\": \"sensor\", \"class\": \"temperature\", \"state\": \"measurement\", \"unit\": \"°C\", \"round\": 1},";
                }
                if (value["name"].as<String>() == "anydataHum")
                {
                    // HOMEdJSON = HOMEdJSON + "\"humidity_" + device + "\",";
                    options = options + "\"" + device + "\":{\"type\": \"sensor\", \"class\": \"humidity\", \"state\": \"measurement\", \"unit\": \"%\", \"round\": 1},";
                }
                if (value["name"].as<String>() == "inputDgt")
                {
                    options = options + "\"" + device + "\":{\"type\": \"number\", \"min\": -10000, \"max\": 100000, \"step\": 0.1},";
                }

                i++;
            }
            options = options.substring(0, options.length() - 1);
            HOMEdJSON = HOMEdJSON.substring(0, HOMEdJSON.length() - 1);
            HOMEdJSON = HOMEdJSON + "],";
            HOMEdJSON = HOMEdJSON + " \"options\": {" + options + "}";
            HOMEdJSON = HOMEdJSON + "}}";
            String topic = (HOMEdTopic + "/command/custom").c_str();
            if (!publish(topic, HOMEdJSON))
            {
                SerialPrint("E", F("MQTT"), F("Failed publish  data to HOMEd"));
            }

            file.close();

            publishRetain(HOMEdTopic + "/device/custom/" + esp_id, "{\"status\":\"online\"}");

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
    void deleteFromHOMEd()
    {
        if (HOMEd)
        {
            for (std::list<IoTItem *>::iterator it = IoTItems.begin(); it != IoTItems.end(); ++it)
            {
                if (*it)
                {
                    String id_widget = (*it)->getID().c_str();
                    String HOMEdjson = "";
                    HOMEdjson = "{\"action\":\"removeDevice\",";
                    HOMEdjson = HOMEdjson + "\"device\":\"";
                    HOMEdjson = HOMEdjson + ChipId;
                    HOMEdjson = HOMEdjson + "\"}";
                    String topic = (HOMEdTopic + "/command/custom").c_str();
                    if (!publish(topic, HOMEdjson))
                    {
                        SerialPrint("E", F("MQTT"), F("Failed remove from HOMEd"));
                    }
                }
            }
        }
    }
    IoTDiscovery *getHOMEdDiscovery()
    {
        if (HOMEd)
            return this;
        else
            return nullptr;
    }
    ~DiscoveryHomeD() {};
};

void *getAPI_DiscoveryHomeD(String subtype, String param)
{
    if (subtype == F("DiscoveryHomeD"))
    {
        return new DiscoveryHomeD(param);
    }
    else
    {
        return nullptr;
    }
}
