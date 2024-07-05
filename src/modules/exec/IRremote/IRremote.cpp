#include "Global.h"
#include "classes/IoTItem.h"

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRac.h>
#include <IRutils.h>

IRac* ac;

const uint16_t kIrLed = 4;                                      // The ESP GPIO pin to use that controls the IR LED.

class IRremote : public IoTItem {
   private:

    String _set_id;                                             // заданная температура

    int enable = 1;
    float _tmp;

    int _prot;                                                  // протокол
    int _pinTx;                                                 // Выход модуля передатчика

   public:
    IRremote(String parameters): IoTItem(parameters) {
        jsonRead(parameters, "pinTx", _pinTx);                  //передатчик
        jsonRead(parameters, "prot", _prot);                    // используемый протокол
        jsonRead(parameters, "set_id", _set_id);                // id установленной температуры
    
            if (_pinTx >= 0) {
                IoTgpio.pinMode(_pinTx, OUTPUT);
                IoTgpio.digitalWrite(_pinTx, false); }

  // Set up what we want to send.
  // See state_t, opmode_t, fanspeed_t, swingv_t, & swingh_t in IRsend.h for
  // all the various options.

	ac = new IRac(kIrLed);
  ac->next.protocol = (decode_type_t)_prot;
  ac->next.model = 1;                                   // Некоторые кондиционеры имеют разные модели. Попробуйте только первое.
  ac->next.mode = stdAc::opmode_t::kCool;               // Сначала запустите в прохладном режиме.
  ac->next.celsius = true;                              // Используйте градусы Цельсия в качестве единиц измерения температуры. Ложь = Фаренгейт
  ac->next.degrees = 20;                                // 20 degrees.
  ac->next.fanspeed = stdAc::fanspeed_t::kMedium;       // Запустите вентилятор на средней скорости.
  ac->next.swingv = stdAc::swingv_t::kOff;              // Не поворачивайте вентилятор вверх или вниз.
  ac->next.swingh = stdAc::swingh_t::kOff;              // Не поворачивайте вентилятор влево или вправо.
  ac->next.light = false;                               // Выключите все светодиоды/световые приборы/дисплеи, которые сможем.
  ac->next.beep = false;                                // Если есть возможность, выключите все звуковые сигналы кондиционера.
  ac->next.econo = false;  // Turn off any economy modes if we can.
  ac->next.filter = false;  // Turn off any Ion/Mold/Health filters if we can.
  ac->next.turbo = false;  // Don't use any turbo/powerful/etc modes.
  ac->next.quiet = false;  // Don't use any quiet/silent/etc modes.
  ac->next.sleep = -1;  // Don't set any sleep time or modes.
  ac->next.clean = false;  // Turn off any Cleaning options if we can.
  ac->next.clock = -1;  // Don't set any current time if we can avoid it.
  ac->next.power = false;  // Initially start with the unit off.

  Serial.println("Try to turn on & off every supported A/C type ...");
}   


    void doByInterval() {}

        IoTValue execute(String command, std::vector<IoTValue> &param) {

          if (command == "on") { 

            ac->next.power = true;                                       // Типа команда включить
            ac->sendAc();                                                // Send the message.
                
              SerialPrint("i", F("IRremote"), "Ballu AC on ");              
            }

          if (command == "off") { 

            ac->next.power = false;
            ac->sendAc();
                
              SerialPrint("i", F("IRremote"), "Ballu AC off ");              
            }

          if (command == "cool") { 

            ac->next.mode = stdAc::opmode_t::kCool;
            ac->sendAc();
                
              SerialPrint("i", F("IRremote"), "Ballu AC cool ");              
            }

          if (command == "heat") { 

            ac->next.mode = stdAc::opmode_t::kHeat;
            ac->sendAc();
                
              SerialPrint("i", F("IRremote"), "Ballu AC heat ");              
            }

          if (command == "dry") { 

            ac->next.mode = stdAc::opmode_t::kDry;
            ac->sendAc();
                
              SerialPrint("i", F("IRremote"), "Ballu AC dry ");              
            }
          if (command == "auto") { 

            ac->next.fanspeed = stdAc::fanspeed_t::kAuto;
            ac->sendAc();
                
              SerialPrint("i", F("IRremote"), "Ballu AC speed1 ");              
            }
          if (command == "speedmin") { 

            ac->next.fanspeed = stdAc::fanspeed_t::kMin;
            ac->sendAc();
                
              SerialPrint("i", F("IRremote"), "Ballu AC speed min ");              
            }
          if (command == "speedlow") { 

            ac->next.fanspeed = stdAc::fanspeed_t::kLow;
            ac->sendAc();
                
              SerialPrint("i", F("IRremote"), "Ballu AC speed low ");              
            }
          if (command == "speedmed") { 

            ac->next.fanspeed = stdAc::fanspeed_t::kMedium; // Надо выбрать под конкретный кондиционер из 6-ти вариантов
            ac->sendAc();
                
              SerialPrint("i", F("IRremote"), "Ballu AC speed medium ");              
            }
            if (command == "speedhigh") { 

            ac->next.fanspeed = stdAc::fanspeed_t::kHigh; // Надо выбрать под конкретный кондиционер из 6-ти вариантов
            ac->sendAc();
                
              SerialPrint("i", F("IRremote"), "Ballu AC speed high");              
            }
          if (command == "speedmax") { 

            ac->next.fanspeed = stdAc::fanspeed_t::kMax; // Надо выбрать под конкретный кондиционер из 6-ти вариантов
            ac->sendAc(); 
                
              SerialPrint("i", F("IRremote"), "Ballu AC speed max");              
            }

          if (command == "speedmh") { 

            ac->next.fanspeed = stdAc::fanspeed_t::kMediumHigh;
            ac->sendAc();
                
              SerialPrint("i", F("IRremote"), "Ballu AC speed mh");              
            }

          if (command == "setTemp") { 

			// заданная температура
			IoTItem *tmp = findIoTItem(_set_id);
			if (tmp)
			{
				_tmp = ::atof(tmp->getValue().c_str());
				ac->next.degrees = _tmp;    // set Temp 17 C - 30 C.
				ac->sendAc();               // Send the message.
				SerialPrint("i", F("IRremote"), "Ballu AC set temp ->  " + String(_tmp) );
			}
			else
			{
				// если не заполнены настройки кондиционера
				setValue("ошибка настройки кондиционера");
			}
		
    }
          
          if (command == "swing") { 

            ac->next.swingv = stdAc::swingv_t::kMiddle;;        // Надо выбрать под конкретный кондиционер из 6-ти вариантов
            ac->sendAc();                                       // Send the message.
                
              SerialPrint("i", F("IRremote"), "Ballu AC swing middle");              
            }
       
     return {};  // команда поддерживает возвращаемое значения. Т.е. по итогу выполнения команды или общения с внешней системой, можно вернуть значение в сценарий для дальнейшей обработки
    }

    ~IRremote() {};
};

void* getAPI_IRremote(String subtype, String param) {
    if (subtype == F("IRremote")) {
        return new IRremote(param);
    } else {
        return nullptr;  
    }
}
