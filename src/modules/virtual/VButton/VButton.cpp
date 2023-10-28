#include "Global.h"
#include "classes/IoTItem.h"


class VButton : public IoTItem {
   public:
    VButton(String parameters): IoTItem(parameters) {
        _round = 0;
    }
/*
    void setValue(const IoTValue& Value, bool genEvent = true) {
       // value = Value;
        regEvent((String)(int)Value.valD, "VButton", false, genEvent);
    }

    String getValue() {
        return (String)(int)getIoTValue().valD;
    }
*/
    void doByInterval() { }
};

void* getAPI_VButton(String subtype, String param) {
    if (subtype == F("VButton")) {
        return new VButton(param);
    } else {
        return nullptr;
    }
}
