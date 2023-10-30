#include "Global.h"
#include "classes/IoTItem.h"

extern IoTGpio IoTgpio;


class ButtonIn : public IoTItem {
   private:
    int _pin;
    bool _execLevel, _fixState, _inv, _buttonState, _reading;
    bool _lastButtonState = LOW;
    unsigned long _lastDebounceTime = 0;
    int _debounceDelay = 50;

   public:
    ButtonIn(String parameters): IoTItem(parameters) {
        String _pinMode;

        jsonRead(parameters, F("inv"), _inv);
        jsonRead(parameters, F("pin"), _pin);
        jsonRead(parameters, F("execLevel"), _execLevel);
        jsonRead(parameters, F("pinMode"), _pinMode);
        jsonRead(parameters, F("debounceDelay"), _debounceDelay);
        jsonRead(parameters, F("fixState"), _fixState);
        _round = 0;
        
        if (_pinMode == F("INPUT")) IoTgpio.pinMode(_pin, INPUT);
        else if (_pinMode == F("INPUT_PULLUP")) IoTgpio.pinMode(_pin, INPUT_PULLUP);
        else if (_pinMode == F("INPUT_PULLDOWN")) {IoTgpio.pinMode(_pin, INPUT); IoTgpio.digitalWrite(_pin, LOW);}
        
        _buttonState = IoTgpio.digitalRead(_pin);
        if (_inv) _buttonState = !_buttonState;    // инвертируем, если нужно показания
        // сообщаем всем о стартовом статусе без генерации события
        setValue((float&)_buttonState, false);
    }

    void loop() {
        _reading = IoTgpio.digitalRead(_pin);
        if (_reading != _lastButtonState) {
            // reset the debouncing timer
            _lastDebounceTime = millis();
        }

        if ((millis() - _lastDebounceTime) > _debounceDelay) {
            if (_reading != _buttonState) {
               _buttonState = _reading;

                if (_fixState == 1 && _buttonState == _execLevel) {
                   // value.valD = !value.valD;
                    setValue(!getValueD()); 
                }
                
                if (_fixState == 0) {
                    if (_inv) setValue(!_buttonState);    // инвертируем, если нужно показания
                    else setValue(_buttonState);
                }
            }
        }

        _lastButtonState = _reading;
    }
/*
    void setValue(const IoTValue& Val, bool genEvent = true) {
        regEvent((String)(int)Val.valD, F("ButtonIn"), false, genEvent);
    }
    */
/*
    String getValue() {
        return (String)(int)getValue().valD;
    }
*/
    ~ButtonIn() {};
};

void* getAPI_ButtonIn(String subtype, String param) {
    if (subtype == F("ButtonIn")) {
        return new ButtonIn(param);
    } else {
        return nullptr;
    }
}
