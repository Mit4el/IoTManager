/******************************************************************
  Simple library for the WEMOS SHT30 Shield.
  
  https://github.com/wemos/WEMOS_SHT3x_Arduino_Library

  adapted for version 4 @Serghei63
 ******************************************************************/

#include "Global.h"
#include "classes/IoTItem.h"


#include "Wire.h"
#include <WEMOS_SHT3X.h>

SHT3X sht30(0x45);

class Sht30t : public IoTItem {
   public:
    Sht30t(String parameters): IoTItem(parameters) { }
    
    void doByInterval() {
        if(sht30.get()==0){

        SerialPrint("E", "Sensor Sht30t", "OK");

        if (sht30.cTemp < -46.85F) setValue(sht30.cTemp);     // TODO: найти способ понимания ошибки получения данных
            else SerialPrint("E", "Sensor Sht30t", "Error", _id);  
        }
    }
    ~Sht30t() {};
};

class Sht30h : public IoTItem {
   public:
    Sht30h(String parameters): IoTItem(parameters) { }
    
    void doByInterval() {
        if(sht30.get()==0){

        SerialPrint("E", "Sensor Sht30h", "OK");
        if (sht30.humidity != -6) setValue(sht30.humidity);    // TODO: найти способ понимания ошибки получения данных
            else SerialPrint("E", "Sensor Sht30h", "Error", _id);
        }   
    }
    ~Sht30h() {};
};


void* getAPI_Sht30(String subtype, String param) {
    if (subtype == F("Sht30t")){
        return new Sht30t(param);
        }
        if (subtype == F("Sht30h")) {
            return new Sht30h(param);
    } else {
        return nullptr;
    }
}
