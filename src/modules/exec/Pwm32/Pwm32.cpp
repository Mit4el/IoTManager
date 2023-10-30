#include "Global.h"
#include "classes/IoTItem.h"
#include "Arduino.h"

#include "esp32-hal.h"
#include "esp32-hal-ledc.h"

extern IoTGpio IoTgpio;

class Pwm32 : public IoTItem {
   private:
    int _pin;
    int _freq; 
    int _apin, _oldValue;
    bool _freezVal = true;
    int _ledChannel; 
    int _resolution;

   public:
    Pwm32(String parameters): IoTItem(parameters) {
        _interval = _interval / 1000;   // корректируем величину интервала int, теперь он в миллисекундах   

        jsonRead(parameters, "pin", _pin);
        jsonRead(parameters, "freq", _freq);
        jsonRead(parameters, "ledChannel", _ledChannel);
        jsonRead(parameters, "PWM_resolution", _resolution);
        
        pinMode(_pin, OUTPUT);
        ledcSetup(_ledChannel, _freq, _resolution);
        ledcAttachPin(_pin, _ledChannel);
        ledcWrite(_ledChannel, getValueD());

        _resolution = pow(2, _resolution);  // переводим биты в значение

        jsonRead(parameters, "apin", _apin);
        if (_apin >= 0) IoTgpio.pinMode(_apin, INPUT);
    }

    void doByInterval() {
        if (_apin >= 0) {
            setValue((float)map(IoTgpio.analogRead(_apin), 0, 4095, 0, _resolution), false); //Сохраняем значение без события
            if (getValueD() > _resolution - 6) setValue(_resolution);  // нивелируем погрешность установки мин и макс
                else if (getValueD() < 9) setValue(0);   // из-за смягчения значений
            if (abs(_oldValue - getValueD()) > 20) {
                _oldValue = getValueD();
                ledcWrite(_ledChannel, getValueD());
                _freezVal = false;
            } else {
                if (!_freezVal) {   // отправляем событие только раз после серии изменений, чтоб не спамить события
                    setValue((float)getValueD());
                    _freezVal = true;
                }
            }
        }
    }
 
    void setValue(int Value, bool genEvent = true) {
        ledcWrite(_ledChannel, Value);
        setValue((float)Value, genEvent);
    }
    //=======================================================================================================

    ~Pwm32() {};
};


void* getAPI_Pwm32(String subtype, String param) {
    if (subtype == F("Pwm32")) {
        return new Pwm32(param);
    } else {
        return nullptr;
    }
}
