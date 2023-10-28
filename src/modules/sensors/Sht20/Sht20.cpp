#include "Global.h"
#include "classes/IoTItem.h"

#include "Wire.h"
#include "SHT2x.h"

SHT2x* sht = nullptr;

class Sht20t : public IoTItem {
   public:
    Sht20t(String parameters) : IoTItem(parameters) {}

    void doByInterval() {
        sht->read();
        float valD = sht->getTemperature();
        if (valD > -46.85F)
            setValue(valD);
        else
            SerialPrint("E", "Sensor Sht20t", "Error", _id);
    }

    ~Sht20t(){};
};

class Sht20h : public IoTItem {
   public:
    Sht20h(String parameters) : IoTItem(parameters) {}

    void doByInterval() {
        sht->read();
        float valD = sht->getHumidity();
        if (valD != -6)
            setValue(valD);
        else
            SerialPrint("E", "Sensor Sht20h", "Error", _id);
    }

    ~Sht20h(){};
};

void* getAPI_Sht20(String subtype, String param) {
    if (subtype == F("Sht20t") || subtype == F("Sht20h")) {
        if (!sht) {
            sht = new SHT2x;
            if (sht) sht->begin();
        }
    
       if (subtype == F("Sht20t")) {
           return new Sht20t(param);
       } else if (subtype == F("Sht20h")) {
           return new Sht20h(param);
       }
    }

    return nullptr;
}
