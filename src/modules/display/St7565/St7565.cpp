#include "Global.h"
#include "classes/IoTItem.h"

#include <Arduino.h>
#include <U8g2lib.h>


#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif

#ifdef ESP8266

//U8G2_ST7565_ERC12864_F_4W_SW_SPI u8g2(U8G2_R0,/* clk= */ 19 , /* data= */ 23 , /* cs= */ 5 , /* dc|a0= */ 22 , /* reset= */ 18 ); //* clock=*/ D5, /* data=*/ D7, /* cs=*/ D8, /* dc=*/ D2, /* reset=*/ D3);
U8G2_ST7565_ERC12864_F_4W_SW_SPI u8g2(U8G2_R0, 16,  17, 15,  4, 2);  //нужное мне подключение
#endif

#ifdef ESP32
//U8G2_ST7565_ERC12864_F_4W_SW_SPI u8g2(U8G2_R0,/* clk= */ 19 , /* data= */ 23 , /* cs= */ 5 , /* dc|a0= */ 22 , /* reset= */ 18 ); //* clock=*/ D5, /* data=*/ D7, /* cs=*/ D8, /* dc=*/ D2, /* reset=*/ D3);
U8G2_ST7565_ERC12864_F_4W_SW_SPI u8g2(U8G2_R0, 16,  17, 15,  4, 2);  //нужное мне подключение
#endif

#include <map>

class ST7565i : public IoTItem {
   private:

    unsigned int _x;
    unsigned int _y;
    String _id2show;
    String _descr;
    String _descr1;
    int _prevStrSize;

    bool _isShow = true;    // экран показывает

   public:
    ST7565i(String parameters) : IoTItem(parameters) {

        String addr, size, xy;
        _prevStrSize = 0;

        u8g2.begin(); 
       // u8g2.setContrast(10*16);
         u8g2.enableUTF8Print();    // enable UTF8 support for the Arduino print() function
         u8g2.setContrast(25);

        jsonRead(parameters, "coord", xy);
        _x = selectFromMarkerToMarker(xy, ",", 0).toInt();
        _y = selectFromMarkerToMarker(xy, ",", 1).toInt();

        jsonRead(parameters, "descr", _descr);
        jsonRead(parameters, "id2show", _id2show);
        jsonRead(parameters, "descr1", _descr1);

        int clock = jsonReadInt(parameters, "Clock");
        int data = jsonReadInt(parameters, "Data");
        int cs = jsonReadInt(parameters, "Cs");

    }

    void doByInterval() {

       // if (u8g2 != nullptr) { // это не работает
           // if (u8g2 != nullptr) {

           // printBlankStr(_prevStrSize);
            
            String tmpStr = "";
       
            //if (_descr != "none") tmpStr = _descr + " " + getItemValue(_id2show);
            if (_descr != "none") tmpStr = _descr + " " + getItemValue(_id2show) + " " + _descr1; // Добавил вывод после
                else tmpStr = getItemValue(_id2show);


            u8g2.clearBuffer();					// clear the internal memory
            //u8g2.setFont(u8g2_font_victoriabold8_8r);	// choose a suitable font
            u8g2.setFont(u8g2_font_6x12_t_cyrillic);  // первая строка

            u8g2.setCursor(_x, _y);

            u8g2.print(tmpStr);// вывод переменной на экран

            u8g2.sendBuffer(); 



            _prevStrSize = tmpStr.length();


         
    }

    IoTValue execute(String command, std::vector<IoTValue> &param) {  // будет возможным использовать, когда сценарии запустятся
 
        

        if (command == "x") {
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
        } else if (command == "descr1") {
            if (param.size()) {
                _descr1 = param[0].valS;
            }    
        } else if (command == "id2show") {
            if (param.size()) {
                _id2show = param[0].valS;
            }
        }

        doByInterval();
        return {};


    }

    //печать пустой строки нужной длинны для затирания предыдущего значения на экране
    void printBlankStr(int strSize) {
        String tmpStr = "";
        for (int i = 0; i < strSize; i++) tmpStr += " ";
        
           u8g2.clearBuffer();                    // clear the internal memory очистите внутреннюю память
            u8g2.setCursor(_x, _y);
            u8g2.print(tmpStr);
            u8g2.sendBuffer();                    // transfer internal memory to the display перенесите внутреннюю память на дисплей

    }


    ~ST7565i(){};
};

void *getAPI_St7565(String subtype, String param) {
    if (subtype == F("Max7219")) {
        return new ST7565i(param);
    } else {
        return nullptr;
    }
  }