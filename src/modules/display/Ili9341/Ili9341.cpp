/******************************************************************
  Library for Adafruit ILI9341 displays
  Support for ILI9341
  =https://github.com/adafruit/Adafruit_ILI9341
  depends=Adafruit GFX Library, Adafruit STMPE610, Adafruit TouchScreen

  adapted for version 4dev @Serghei63
 ******************************************************************/
#include "Global.h"
#include "classes/IoTItem.h"

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

// Добавь шрифты

//#include <Fonts/FreeSans9pt7b.h>
//#include <Fonts/FreeSansBold9pt7b.h>
//#include <Fonts/FreeSansBold12pt7b.h>
//#include <Fonts/FreeSansBold24pt7b.h>

#ifdef ESP8266
// For the Adafruit shield, these are the default.
#define TFT_CS D0  //for D1 mini or TFT I2C Connector Shield (V1.1.0 or later)
#define TFT_DC D8  //for D1 mini or TFT I2C Connector Shield (V1.1.0 or later)
//#define TFT_RST -1 //for D1 mini or TFT I2C Connector Shield (V1.1.0 or later)
   //#define SD_CS    2
#elif defined(ESP32) && !defined(ARDUINO_ADAFRUIT_FEATHER_ESP32S2)
 //  #define STMPE_CS 32
   #define TFT_CS   15
   #define TFT_DC   33
  // #define SD_CS    14
#endif

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
//Adafruit_ILI9341 *tft;


#include <map>


class Ili9341 : public IoTItem {
   private:
    unsigned int _x;
    unsigned int _y;
    String _id2show;
    String _descr;
    String _descr1;
    int _prevStrSize;

    bool _isShow = true;    // экран показывает

   public:
    Ili9341(String parameters) : IoTItem(parameters) {

        String  xy;
        _prevStrSize = 0;


        tft.begin(); 

          tft.setRotation(1);    //Landscape
          tft.fillScreen(ILI9341_BLUE);



        jsonRead(parameters, "coord", xy);
        _x = selectFromMarkerToMarker(xy, ",", 0).toInt();
        _y = selectFromMarkerToMarker(xy, ",", 1).toInt();

        jsonRead(parameters, "descr", _descr);
        jsonRead(parameters, "id2show", _id2show);
        jsonRead(parameters, "descr1", _descr1);



    }

    void doByInterval() {

       // if (tft != nullptr) {
         
          //  printBlankStr(_prevStrSize);
            
            String tmpStr = "";
       
            //if (_descr != "none") tmpStr = _descr + " " + getItemValue(_id2show);
            if (_descr != "none") tmpStr = _descr + " " + getItemValue(_id2show) + " " + _descr1;
                else tmpStr = getItemValue(_id2show);

          tft.setCursor(_x, _y);

          tft.setTextColor(ILI9341_YELLOW, ILI9341_BLUE);

          tft.setTextSize(3);

          tft.print("               ");// 15 пробелов

          tft.setCursor(_x, _y);

          tft.print(tmpStr);

          _prevStrSize = tmpStr.length();
   
        
    }
 /*
    IoTValue execute(String command, std::vector<IoTValue> &param) {  // будет возможным использовать, когда сценарии запустятся
       
        if (command == "noBacklight")
            display->noBacklight();
        else if (command == "backlight")
            display->backlight();
        else if (command == "noDisplay") {
            display->noDisplay();
            _isShow = false;
        }
         else
          
         if (command == "display") {
           // display.display();
            _isShow = true;
        } else if (command == "toggle") {
            if (_isShow) {
              //  display->noDisplay();
                _isShow = false;
            } else { 
              //  display.display();
                _isShow = true;
            }
        } else if (command == "x") {
            if (param.size()) {
                _x = param[0].valD;
            }
        } else if (command == "y") {
            if (param.size()) {
                _y = param[0].valD;
            }
        } else if (command == "descr") {
            if (param.size()) {
                _descr = param[0].valS;
            }
        } else if (command == "id2show") {
            if (param.size()) {
                _id2show = param[0].valS;
            }
        }

        doByInterval();
        return {};


    }
*/
    //печать пустой строки нужной длинны для затирания предыдущего значения на экране
    void printBlankStr(int strSize) {
        String tmpStr = "";
        for (int i = 0; i < strSize; i++) tmpStr += " ";

            tft.setCursor(_x, _y);
           
            tft.setTextColor(ILI9341_YELLOW, ILI9341_BLUE);
            tft.setTextSize(3);
            tft.print(tmpStr);
        
        }


    ~Ili9341(){};
};

void *getAPI_Ili9341(String subtype, String param) {
    if (subtype == F("Ili9341")) {
        return new Ili9341(param);
    } else {
        return nullptr;
    }
}
