#pragma once
#include "classes/IoTGpio.h"
//#define __cplusplus 201703L
#include <variant>

class IoTValue
{
    public:
    //    float valD = 0;
    //    String valS = "";
    std::variant<String, float> val;
    bool isDecimal() { return val.index() == 0 ? false : true; }
    // вернет значение float без окреглений, map и т.п. преобразований
    // Для работы в IoTItem и наследниках используйте getValue(), вкернте с учётом округленяи и преобразований
    float valD()
    {
        auto ptr = std::get_if<float>(&val); // запросили float
        if (ptr == nullptr)
            return 0.0;
        return (float)*ptr;
    }
    // Вернет строку, если значение во float, то преобразует к строке
    // Для работы в IoTItem и наследниках используйте getValue(), вкернте с учётом округленяи и преобразований
    String valS()
    {
        if (val.index() == 0)
        {                                        // если это float, а хотят строку, то берем float и копируем в строку
            auto ptr = std::get_if<float>(&val); // запросили float
            if (ptr == nullptr)
                return "";
            return String((float)*ptr);
        }
        else // если это строка, то просто возвращаем
        {
            auto ptr = std::get_if<String>(&val);
            return *ptr;
        }
    }
};

class IoTItem
{
public:
    IoTItem(const String &parameters);
    virtual ~IoTItem(){};
    virtual void loop();
    virtual void doByInterval();
    virtual IoTValue execute(String command, std::vector<IoTValue> &param);

    void checkIntFromNet();

    String getSubtype();

    String getID();
    bool isStrInID(const String &str);
    int getIntFromNet();

    long getInterval();
    bool isGlobal();

    void sendSubWidgetsValues(String &id, String &json);

    void setInterval(long interval);
    void setIntFromNet(int interval);

    // unsigned long currentMillis;
    // unsigned long prevMillis;
    // unsigned long difference;
    unsigned long nextMillis = 0; // достаточно 1 переменной, надо экономить память
    // задержка следующего вызова, не изменяет текущий _interval
    void suspendNextDoByInt(unsigned long _delay); // 0 - force

    // bool iAmDead = false;  // признак необходимости удалить объект из базы
    bool iAmLocal = true; // признак того, что айтем был создан локально

    bool enableDoByInt = true;

    virtual IoTGpio *getGpioDriver();
    virtual IoTItem *getRtcDriver();
    // virtual IoTItem* getCAMDriver();
    virtual IoTItem *getTlgrmDriver();
    virtual unsigned long getRtcUnixTime();

    // делаем доступным модулям отправку сообщений в телеграм
    virtual void sendTelegramMsg(bool often, String msg);
    virtual void sendFoto(uint8_t *buf, uint32_t length, const String &name);
    virtual void editFoto(uint8_t *buf, uint32_t length, const String &name);

    //////////////////////////////////////////////////
    //      работа с главным занчением элемента     //
    //////////////////////////////////////////////////
    // узнать изменилось ли значение с последнего запроса isChange
    bool isChange();
    // получтьтип записанных данных 0-float, 1-String
    //  uint8_t getTypeValue(){ val.index();}
    // Верет саму структкру, в основном испоьзуется в ядре //????????????????????????
    virtual IoTValue* getValue() { return &_value; }

    // вернуть строку, даже если там число, то округлит, переведет в строку, а потом вернет (напрмер для вывода в serial или json)
    virtual String getValueS();
    // вернет число float если его нет то вернет 0, для проверки использовать isDecimal()
    virtual float getValueD();
    void value(String &val){val = getValueS();}
    void value(float &val){val = getValueD();}
    // установить данные в формате строки, если в строке содержится число, то оно округлится будет храниться как число
    virtual void setValue( String valStr, bool genEvent = true);
    // установить данные в формате float
    virtual void setValue( float valD, bool genEvent = true);
        // установить данные в формате bool
    virtual void setValue( bool valD, bool genEvent = true);
            // установить данные в формате bool
    virtual void setValue( int valD, bool genEvent = true) {setValue ((float)valD, genEvent);}
    // установить данные в формате IoTValue
    virtual void setValue(const IoTValue &val, bool genEvent = true);

    // Молчаливое присвоение главного значения. При этом не будет выработано события и не будет работать isChange
 //   void setValueSilent(float val) { _value.val = val; }
    // Молчаливое присвоение главного значения. При этом не будет выработано события и не будет работать isChange
 //   void setValueSilent(String &val) { _value.val = val; }

    //    String getRoundValue();
    void getNetEvent(String &event);
    virtual String getMqttExterSub();

    // хуки для системных событий (должны начинаться с "on")
    virtual void onRegEvent(IoTItem *item);
    virtual void onMqttRecive(String &topic, String &msg);
    virtual void onMqttWsAppConnectEvent();
    virtual void onModuleOrder(String &key, String &value);
    virtual void onTrackingValue(IoTItem *item); // момент, когда ядро заметило изменение отслеживаемого значения

    // методы для графиков (будет упрощено)
    virtual void publishValue();
    virtual void clearValue();
    virtual void setPublishDestination(int type, int wsNum = -1);
    virtual void clearHistory();
    virtual void setTodayDate();

    bool isTracking(IoTItem *item); // проверка на отслеживание

protected:
    bool _needSave = false; // признак необходимости сохранять и загружать значение элемента на flash
    String _subtype = "";
    String _id = "errorId"; // если будет попытка создания Item без указания id, то элемент оставит это значение
    long _interval = 0;
    int _intFromNet = -2; // количество секунд доверия, пришедших из сети вместе с данными для текущего ИД
                          // -2 - данные не приходили, скорее всего, элемент локальный, доверие есть, в случае прихода сетевого значения с int=0, будет выключен механизм проверки доверия
                          // -1 - данные приходили и обратный отсчет дошел до нуля, значит доверия нет и элемент будет удален при следующем такте loop

    float _multiply; // умножаем на значение
    float _plus;     // увеличиваем на значение
    int _map1 = 0;
    int _map2 = 0;
    int _map3 = 0;
    int _map4 = 0;
    int _round = 1;     // 1, 10, 100, 1000, 10000
    int _numDigits = 1; // количество целых значений, не значимые позиции заменяются нулем в строковом формате

    bool _global = false; // характеристика айтема, что ему нужно слать и принимать события из внешнего мира

    IoTValue *_trackingValue = nullptr; // указатель на значение родительского элемента изменение которого отслеживается

private:
    IoTValue _value;        // хранение основного значения, которое обновляется из сценария, execute(), loop() или doByInterval()
    bool flChange = false; // флаг иземения основного значения с последнего опроса isChange()
    //       virtual void regEvent(const String& value, const String& consoleInfo, bool error = false, bool genEvent = true);
    //        virtual void regEvent(float value, const String& consoleInfo, bool error = false, bool genEvent = true);
    void regEvent(const IoTValue &value, bool error = false, bool genEvent = true);
};

IoTItem *findIoTItem(const String &name);                 // поиск экземпляра элемента модуля по имени
IoTItem *findIoTItemByPartOfName(const String &partName); // поиск элемента модуля в существующей конфигурации по части имени
String getItemValue(const String &name);                  // поиск плюс получение значения
bool isItemExist(const String &name);                     // существует ли айтем
// StaticJsonDocument<JSON_BUFFER_SIZE>* getLocalItemsAsJSON();  // сбор всех локальных значений Items

IoTItem *createItemFromNet(const String &itemId, const String &value, int interval);
IoTItem *createItemFromNet(const String &msgFromNet);
void analyzeMsgFromNet(const String &msg, String altId = "");

// class externalVariable : IoTItem {  // объект, создаваемый при получении информации о событии на другом контроллере для хранения информации о событии указанное время

//    public:
//     externalVariable(const String& parameters);
//     ~externalVariable();
//     void doByInterval();  // для данного класса doByInterval+int выполняет роль счетчика обратного отсчета до уничтожения
// };