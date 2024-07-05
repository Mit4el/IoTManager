#include "Global.h"          // Это шаблон
#include "classes/IoTItem.h" // Смотри пример аналогового входа

#include <Arduino.h> // Тоже шаблон
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include "6bite_rus.h" // подключение шрифт киррилицы
#include "effect.h"    // эффекты
// Defining size, and output pins

class Max7219 : public IoTItem
{
private:
// #define CLK_PIN   18  // or SCK
// #define  DATA_PIN  23  // or MOSI
#define CS_PIN 5 // номер пина CS SPI                  // Для есп8266. Потом попробуем задать с веба

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4 // У меня 4 модулей. Потом опять с веба попробуем задать

    MD_Parola Display = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES); // Опять с ардуины

    String _id2show;
    int _intensity;        // Яркость будем менять с веба ( с modinfo )
    int _SPEED_IN_TIME;    // скорость  начала анимации с веба
    int _SPEED_OUT_TIME;   // скорость  конца анимации с веба
    int _PAUSE_TIME;       // пауза между анимациями с веба
    int _inFX;             // начальный эффект с веба
    int _outFX;            // конечный эффект с веба
    int _setTextAlignment; // выравнивание текста
    String tmpStr;
    uint32_t currentMillis;
    uint32_t difference;
    uint32_t prevMillis;
    uint32_t difference2;
    uint32_t prevMillis2;

public:
    Max7219(String parameters) : IoTItem(parameters)
    { // Опять шаблон для Сетапа

        Display.begin();                  // Сетап с ардуины.
        Display.setIntensity(_intensity); // яркость меняем с веба
        Display.setFont(_6bite_rus);      // шрифт кириллица
        Display.displayClear();

        jsonRead(parameters, "id2show", _id2show);
        jsonRead(parameters, "intensity", _intensity);
        jsonRead(parameters, "SPEED_IN_TIME", _SPEED_IN_TIME);
        jsonRead(parameters, "SPEED_OUT_TIME", _SPEED_OUT_TIME);
        jsonRead(parameters, "PAUSE_TIME", _PAUSE_TIME);
        jsonRead(parameters, "effect_in", _inFX);
        jsonRead(parameters, "effect_out", _outFX);
        jsonRead(parameters, "setTextAlignment", _setTextAlignment);

        // jsonRead(parameters, "MAX_DEVICES", _MAX_DEVICES);
    }
    // Дальше опять шаблон для Лупа

    void loop() // если вызываем loop то void doByInterval() отключается, снизу добавил таймер для вызова этой функции раз в 1 сек.
    {

        // tmpStr = getItemValue(_id2show);

        difference2 = millis() - prevMillis2;

        if (difference2 >= 50)
        {
            if (Display.displayAnimate()) // animates and returns true when an animation is completed
            {
                Display.setTextBuffer(tmpStr.c_str()); // вставляем в буфер дисплея наше значение
                //  Display.displayText(tmpStr.c_str(), PA_CENTER, _SPEED_IN_TIME, _PAUSE_TIME, effect[_inFX], effect[_outFX]); - другая функция отправление данных
                Display.displayReset(); // сбрасываем данные
            }
            prevMillis2 = millis();
        }

        currentMillis = millis(); // таймер для обновления чтения данных с веба
        difference = currentMillis - prevMillis;
        if (difference >= 1000)
        {
            prevMillis = millis();
            this->doByInterval(); // вызов функции void doByInterval()
        }
    }

    void doByInterval()
    {
        tmpStr = getItemValue(_id2show); // считывание значения в веба
        if (_intensity >= 0 and _intensity <= 15)
            Display.setIntensity(_intensity); // выставляем яркость меняем с веба, проверяем значение от 0 до 15
        else
            _intensity = 0; // иначе сбрасываем на 0.

        if (_setTextAlignment >= 0 and _setTextAlignment <= 2)             // выравнивание текста  проверяем значение от 0 до 2
            Display.setTextAlignment(setTextAlignment[_setTextAlignment]); // выравнивание текста 0-слева, 1-по центру, 2-справа
        else
            _setTextAlignment = 1; // иначе выставляем по центру на 1.

        Display.setPause(_PAUSE_TIME);                          // назначение паузы между анимаций
        Display.setSpeedInOut(_SPEED_IN_TIME, _SPEED_OUT_TIME); // назначаем скорость начала и конца анимации

        if (_inFX >= 0 and _inFX <= 28 and _outFX >= 0 and _outFX <= 28)
            Display.setTextEffect(effect[_inFX], effect[_outFX]); // выбираем эффект начала и конца анимации от 0 до 28
        else
        {
            _inFX = 0;
            _outFX = 0;
        }
    }

    // Дальше для изменения дизайна по сценарию. Опять шаблон

    // IoTValue execute(String command, std::vector<IoTValue> &param)

    //{ // будет возможным использовать, когда сценарии запустятся

    // doByInterval();
    // return {};
    //}

    ~Max7219(){};
};
void *getAPI_Max7219(String subtype, String param)
{
    if (subtype == F("Max7219"))
    {
        return new Max7219(param);
    }
    else
    {
        return nullptr;
    }
}