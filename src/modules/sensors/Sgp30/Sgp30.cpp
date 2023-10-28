#include "Global.h"
#include "classes/IoTItem.h"


extern IoTGpio IoTgpio;

#include "SparkFun_SGP30_Arduino_Library.h" // Click here to get the library: http://librarymanager/All#SparkFun_SGP30
#include <Wire.h>

SGP30* mySensor = nullptr; //create an object of the SGP30 class


class Sgp30t : public IoTItem {
   public:
    Sgp30t(String parameters): IoTItem(parameters) {
        if (!mySensor) mySensor = new SGP30();
        mySensor->begin();

        Wire.begin();
    //Initialize sensor
        if (mySensor->begin() == false) {
            Serial.println("No SGP30 Detected. Check connections.");
            //while (1);
        }
    //Initializes sensor for air quality readings
    //measureAirQuality should be called in one second increments after a call to initAirQuality
        mySensor->initAirQuality();
    }

    void doByInterval() {
        mySensor->measureAirQuality();

        if (mySensor->CO2 > -46.85F) setValue(mySensor->CO2);  
            else SerialPrint("E", "Sensor Sgp30t", "Error"); 
    }

    ~Sgp30t() {};
};

class Sgp30e : public IoTItem {
   public:
    Sgp30e(String parameters): IoTItem(parameters) { 

        if (!mySensor) mySensor = new SGP30();
        mySensor->begin();


         Wire.begin();
  //Initialize sensor
  if (mySensor->begin() == false) {
    Serial.println("No SGP30 Detected. Check connections.");
    while (1);
  }
  //Initializes sensor for air quality readings
  //measureAirQuality should be called in one second increments after a call to initAirQuality
  mySensor->initAirQuality();

    }
    
    void doByInterval() {
        mySensor->measureAirQuality();
      
        if (mySensor->TVOC != -6) setValue(vmySensor->TVOC);  
            else SerialPrint("E", "Sensor Sgp30e", "Error");

    }   

    ~Sgp30e() {};
};


void* getAPI_Sgp30(String subtype, String param) {
    if (subtype == F("Sgp30t")) {

       //   mySensor.begin() ;
        //  mySensor.initAirQuality();

        return new Sgp30t(param);
    } else if (subtype == F("Sgp30e")) {
       //   mySensor.begin() ;
       //   mySensor.initAirQuality();
        return new Sgp30e(param);
    } else {
        return nullptr;
    }
}