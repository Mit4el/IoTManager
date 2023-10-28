#include "Global.h"
#include "classes/IoTItem.h"

#include "HX710B.h"


class HX710b : public IoTItem {

   private:	
	HX710B pressure_sensor;

   public:
    HX710b(String parameters) : IoTItem(parameters) {
        int data, clock;
        jsonRead(parameters, "data", data);
        jsonRead(parameters, "clock", clock);

        pressure_sensor.begin(data, clock);
        pressure_sensor.tare();
    }

    void doByInterval() {
        if (!pressure_sensor.is_ready()) return;
		
        setValue(pressure_sensor.mmHg());
    }

    IoTValue execute(String command, std::vector<IoTValue> &param) {
        if (command == "tare") { 
            pressure_sensor.tare();
        } else if (command == "read") { 
            setValueSilent (pressure_sensor.read());
            return getIoTValue();
        } 
        return {};  
    }

    ~HX710b() {};
};

void* getAPI_Hx710(String subtype, String param) {
    if (subtype == F("Hx710")) {
        return new HX710b(param);
    } else {
        return nullptr;
    }
}
