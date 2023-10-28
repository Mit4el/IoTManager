#include "Global.h"
#include "classes/IoTItem.h"

extern IoTGpio IoTgpio;

class Multitouch : public IoTItem
{
private:
    int _int;
    unsigned long timing;
    long _PWMDelay = 500;
    int _count = 0;
    int duration = 0;

    int _pin;
    bool _execLevel, _fixState, _inv, _buttonState, _reading;
    bool _lastButtonState = LOW;
    unsigned long _lastDebounceTime = 0;
    int _debounceDelay = 50;

public:
    Multitouch(String parameters) : IoTItem(parameters)
    {
        String _pinMode;
        jsonRead(parameters, "pin", _pin);
        jsonRead(parameters, "pinMode", _pinMode);
        jsonRead(parameters, "debounceDelay", _debounceDelay);
        jsonRead(parameters, "PWMDelay", _PWMDelay);
        jsonRead(parameters, "int", _int);
        jsonRead(parameters, "inv", _inv);
        _round = 0;

        if (_pinMode == F("INPUT"))
            IoTgpio.pinMode(_pin, INPUT);
        else if (_pinMode == F("INPUT_PULLUP"))
            IoTgpio.pinMode(_pin, INPUT_PULLUP);
        else if (_pinMode == F("INPUT_PULLDOWN"))
        {
            IoTgpio.pinMode(_pin, INPUT);
            IoTgpio.digitalWrite(_pin, LOW);
        }

        _buttonState = IoTgpio.digitalRead(_pin);
        // сообщаем всем о стартовом статусе без генерации события
        setValue(_buttonState, false);
        SerialPrint("I", F("Multitouch"), "_buttonState " + String(_buttonState));
    }

    void loop()
    {
        _reading = IoTgpio.digitalRead(_pin);
        if (_reading != _lastButtonState)
        {
            _lastDebounceTime = millis();
        }
        if ((millis() - _lastDebounceTime) > _debounceDelay)
        {
            if (_reading != _buttonState)
            {
                _count++;
                duration = 0;
                _buttonState = _reading;
            }
            if (1 < _count && millis() > _lastDebounceTime + _PWMDelay)
            {
                setValue( _count / 2);
                _count = 0;
            }

            if (millis() - timing > _int && _reading == _inv && millis() - _lastDebounceTime > _PWMDelay)
            {
                SerialPrint("I", F("Multitouch"), "Считаем задержку");
                timing = millis();
                duration = millis() - _lastDebounceTime - _PWMDelay;
                setValue(duration / _int + 1);
                _count = -1;
            }
        }

        _lastButtonState = _reading;
    }
    ~Multitouch(){};
};

void *getAPI_Multitouch(String subtype, String param)
{
    if (subtype == F("Multitouch"))
    {
        return new Multitouch(param);
    }
    else
    {
        return nullptr;
    }
}