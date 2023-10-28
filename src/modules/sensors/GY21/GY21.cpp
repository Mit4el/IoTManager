/******************************************************************
  Used GY21 Driver (temperature and humidity sensor)
  Support for HTY21 , SHT21 , Si7021
  https://github.com/adafruit/Adafruit_AHTX0

  adapted for version 4 Alecs Alecs
 ******************************************************************/
#include "Global.h"
#include "classes/IoTItem.h"

#include "Wire.h"
#include "GY21.h"

GY21* sensor = nullptr;

class GY21t : public IoTItem {
   public:
    GY21t(String parameters) : IoTItem(parameters) {}

    void doByInterval() {
        // wire->read();
        float valD = sensor->GY21_Temperature();
        if (valD < 300)
            setValue(valD); 
        else
            SerialPrint("E", "Sensor GY21t", "Error", _id);
    }

    ~GY21t(){};
};

class GY21h : public IoTItem {
   public:
    GY21h(String parameters) : IoTItem(parameters) {}

    void doByInterval() {
        // sht->read();
        float valD = sensor->GY21_Humidity();
        if (valD > 0)
            setValue(valD);
        else
            SerialPrint("E", "Sensor GY21h", "Error", _id);
    }

    ~GY21h(){};
};

void* getAPI_GY21(String subtype, String param) {
    if (subtype == F("GY21t") || subtype == F("GY21h")) {
        if (!sensor) {
            sensor = new GY21;
            if (sensor) Wire.begin(SDA, SCL);
        }
    
       if (subtype == F("GY21t")) {
           return new GY21t(param);
       } else if (subtype == F("GY21h")) {
           return new GY21h(param);
       }
    }

    return nullptr;
}