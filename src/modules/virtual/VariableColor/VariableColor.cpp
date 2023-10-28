#include "Global.h"
#include "classes/IoTItem.h"

// дочь         -        родитель
class VariableColor : public IoTItem
{
private:
public:
    VariableColor(String parameters) : IoTItem(parameters)
    {
    }

    void doByInterval()
    {
    }

    // событие когда пользователь подключается приложением или веб интерфейсом к усройству
    void onMqttWsAppConnectEvent()
    {
        SerialPrint("i", "Connecting", "Dashbord open ");
        setValue((String)getValue(), true);
    }
     
     IoTValue execute(String command, std::vector<IoTValue> &param)
    {
        if (command == "widget" && param.size() == 2)
        {
            String json = "{}";
            jsonWriteStr(json, param[0].val(), param[1].val());
            sendSubWidgetsValues(_id, json);
        }
        return {};
    }
};

void *getAPI_VariableColor(String subtype, String param)
{
    if (subtype == F("VariableColor"))
    {
        return new VariableColor(param);
    }
    else
    {
        return nullptr;
    }
}
