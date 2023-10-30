#include "Global.h"
#include "classes/IoTItem.h"

extern IoTGpio IoTgpio;

class ButtonOut : public IoTItem {
   private:
    int _pin;
    bool _inv;

   public:
    ButtonOut(String parameters): IoTItem(parameters) {
        jsonRead(parameters, "pin", _pin);
        jsonRead(parameters, "inv", _inv);
        _round = 0;
        IoTgpio.pinMode(_pin, OUTPUT);
        IoTgpio.digitalWrite(_pin, _inv?!(bool)getValue():(bool)getValue());
        enableDoByInt = false;
    }

    void doByInterval() {
        int val = _inv?1:0;
        IoTgpio.digitalWrite(_pin, val);
        // SerialPrint("I", "ButtonOut","single pulse end");
       // value.valD = 0;
        setValue(0);
        enableDoByInt = false;
        //regEvent(value.valD, "ButtonOut");  //обязательный вызов хотяб один
    }

    IoTValue execute(String command, std::vector<IoTValue> &param) {
        // реакция на вызов команды модуля из сценария
        // String command - имя команды после ID. (ID.Команда())
        // param - вектор ("массив") значений параметров переданных вместе с командой: ID.Команда("пар1", 22, 33) -> param[0].ValS = "пар1", param[1].ValD = 22
    
        if (command == "change") { 
            //value.valD = 1 - (int)value.valD;
            setValue(1 - (int)getValue());
            IoTgpio.digitalWrite(_pin, _inv?!(bool)getValue():(bool)getValue());
        }
        else if (command == "pulse") {
            if (param[0].isDecimal() && (param[0].valD() != 0)) {
                //value.valD = !_inv?1:0;
                enableDoByInt = true;
                // SerialPrint("I", "ButtonOut","single pulse start");
                setValue((int)!_inv?1:0);
                suspendNextDoByInt(param[0].valD());
                IoTgpio.digitalWrite(_pin, !_inv?1:0);
            }
        }
        return {};  // команда поддерживает возвращаемое значения. Т.е. по итогу выполнения команды или общения с внешней системой, можно вернуть значение в сценарий для дальнейшей обработки
    }

    void setValue(int Val, bool genEvent = true) {
      //  value = Value;
        if ((Val == !_inv?1:0) && (_interval != 0)) {
            Val = !_inv?1:0;
            enableDoByInt = true;
            // SerialPrint("I", "ButtonOut","single pulse start");
            suspendNextDoByInt(_interval);
        } else {
            enableDoByInt = false;
        }
        setValue((float)Val, genEvent);
        IoTgpio.digitalWrite(_pin, _inv?!Val:Val);
    }
/*
    String getValue() {
        return (String)(int)getValueD();
    }
*/
    ~ButtonOut() {};
};

void* getAPI_ButtonOut(String subtype, String param) {
    if (subtype == F("ButtonOut")) {
        return new ButtonOut(param);
    } else {
        return nullptr;
    }
}
