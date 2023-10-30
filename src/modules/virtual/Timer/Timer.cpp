#include "Global.h"
#include "classes/IoTItem.h"

extern IoTGpio IoTgpio;


class Timer : public IoTItem {
   private:
    bool _unfin = false;
    bool _ticker = false;
    bool _repeat = false;
    bool _pause = false; 
    int _initValue;

   public:
    Timer(String parameters): IoTItem(parameters) {
        jsonRead(parameters, "countDown", _initValue);
        _unfin = !_initValue;
        
        if (!_needSave) {
            setValue(_initValue, false);                    
            if (_initValue) setValue(getValueD() + 1, false);        // +1 - компенсируем ранний вычет счетчика, ранний вычет, чтоб после события значение таймера не исказилось.     
        }
        
        jsonRead(parameters, "ticker", _ticker);
        jsonRead(parameters, "repeat", _repeat);
    }

    void doByInterval() {
        if (!_unfin && getValueD() >= 0 && !_pause) {
            if (_repeat && getValueD() == 0) setValue (_initValue, false);
            setValue(getValueD() - 1, false);
            if (getValueD() == 0) {
                setValue(getValueD());
            }
            //if (!_ticker) regEvent(getValue(), "Timer tick", false, false);  // только регистрируем изменения без генерации тиков
        }

        if (_ticker && (getValueD() > 0 || _unfin) && !_pause) setValue(getValueD());
    }

    IoTValue execute(String command, std::vector<IoTValue> &param) {
        if (command == "stop") { 
            _pause = true;
        } else if (command == "reset") {
            _pause = false;
            setValue (_initValue, false);
            if (_initValue) setValue (getValueD() + 1, false);
        } else if (command == "continue") {
            _pause = false;
        } else if (command == "int") {
            if (param.size() == 1) {
                setInterval(param[0].valD());
            }
        } else if (command == "setInitCountDown") {
            if (param.size() == 1) {
                _initValue = param[0].valD();
            }
        }

        return {}; 
    }

    ~Timer() {};
};

void* getAPI_Timer(String subtype, String param) {
    if (subtype == F("Timer")) {
        return new Timer(param);
    } else {
        return nullptr;
    }
}
