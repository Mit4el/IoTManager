#include "Global.h"
#include "classes/IoTItem.h"

#define EB_FAST 30      // таймаут быстрого поворота энкодера, мс
#define EB_DEB 50       // дебаунс кнопки, мс
#define EB_CLICK 400    // таймаут накликивания кнопки, мс
#include "EncButton2.h"


class Encoder : public IoTItem {
   private:
    float step = 1;
    float stepOnPress = 5;
    EncButton2<EB_ENCBTN> enc1;
    
   public:   
    Encoder(String parameters) : IoTItem(parameters) {
        String pins;
        jsonRead(parameters, "pins", pins);
        int CLK = selectFromMarkerToMarker(pins, ",", 0).toInt();
        int DT  = selectFromMarkerToMarker(pins, ",", 1).toInt();
        int SW  = selectFromMarkerToMarker(pins, ",", 2).toInt();
         
        jsonRead(parameters, "step", step);
        jsonRead(parameters, "stepOnPress", stepOnPress);

        enc1.setPins(INPUT, CLK, DT, SW);
        enc1.setEncReverse(false);             
    }

    void loop() { 
        if (enc1.tick() != 0) {
            if (enc1.left()) {  
                setValue(getValue() - step);
            } else if (enc1.right()) {   
                setValue(getValue() + step");
            } else if (stepOnPress) {
                if (enc1.leftH()) {  
                    setValue(getValue() - stepOnPress);
                } else if (enc1.rightH()) {   
                    setValue(getValue() + stepOnPress);
                }
            }

            // в конце лучше вызвать resetState(), чтобы сбросить необработанные флаги!
            enc1.resetState();
        }   
    }

    ~Encoder() {};  
};

void* getAPI_Encoder(String subtype, String param) {
    if (subtype == F("Encoder")) {
        return new Encoder(param);
    } else {
        return nullptr;
    }
}