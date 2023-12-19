#include "IoTBench.h"
#include "utils/JsonUtils.h"
#include "utils/SerialPrint.h"

#ifdef OS_BENCH

IoTBenchmark *IoTBenchmark::mInstance = nullptr;

IoTBenchmark *IoTBenchmark::instance()
{
  if (NULL == mInstance)
  {
    mInstance = new IoTBenchmark();
  }
  return mInstance;
}

IoTBenchmark::IoTBenchmark()
{
  // считать из конфига  loadP
  jsonRead(settingsFlashJson, "loadP", (int &)loadP, false);
  loadTmr = uptime();
}
// получить загруженность процессора в процентах
int IoTBenchmark::getLoad() { return load; }

// включить измерение загруженности процессора за указанный период в мс
void IoTBenchmark::enableLoad(uint32_t lp) { loadP = lp; }

// отключить измерение загруженности процессора
void IoTBenchmark::disableLoad() { loadP = 0; }

// подключить функцию обработчик задачи
void IoTBenchmark::attach(const String &id)
{
  if (loadP > 0)
    banchItems[id] = new ItemBench;
  SerialPrint("i", F("Benchmark"),
              "add id: " + id);
}

void IoTBenchmark::initLoadFunction()
{
  if (loadP > 0)
  { // если время анализа задано
    if (uptime() - loadTmr >= loadP)
    {
      SerialPrint("i", F("Benchmark"),
                  "CPU load in " + String(loadP) + ", realtime :" + String((uptime() - loadTmr)/1ul) + "ms");
      // если пришло время подсичтывать загруженность
      loadTmr = uptime();              //+= loadP;
      load = (loadSum / 10ul) / loadP; // (loadSum / 1000) / loadP * 100
      loadSum = 0;
      SerialPrint("i", F("Benchmark"),
                  "CPU load in " + String(loadP) + "ms :" + String(load) + "%" +
                      " loop/sec: " + String(count / (loadP / 1000)));
      for (auto it = banchItems.begin(); it != banchItems.end(); it++)
      {
        // it->second->updateSerial(_myUARTpzem);
        SerialPrint(
            "i", F("Benchmark"),
            "Max load in " + it->first + " - " +
                String((it->second)->loopTimeMax) +
                "ms");
      }
      count = 0;
    }
    us = micros(); // micros();
    count++;
  }
}
void IoTBenchmark::postLoadFunction()
{
  if (loadP > 0)
    loadSum += micros() - us;
}

void IoTBenchmark::preBenchFunction(const String &id)
{
  if (loadP > 0 && banchItems.find(id) != banchItems.end())
  {
    banchItems[id]->loopTime = millis(); // micros();
  }
}
void IoTBenchmark::postBenchFunction(const String &id)
{
  if (loadP > 0 && banchItems.find(id) != banchItems.end())
  {
    banchItems[id]->loopTime = millis() - banchItems[id]->loopTime;
    if (banchItems[id]->loopTime > banchItems[id]->loopTimeMax)
      banchItems[id]->loopTimeMax = banchItems[id]->loopTime;
  }
}

// получить максимальное время выполнения выбранной ранее задачи в мкс
uint32_t IoTBenchmark::getMaxLoopTime(const String &id)
{
  if (loadP > 0 && banchItems.find(id) != banchItems.end())
    return banchItems[id]->loopTimeMax;
  else
    return 0;
}

void IoTBenchmark::clearBenchConfig()
{
  for (auto it = banchItems.begin(); it != banchItems.end(); it++)
  {
    delete it->second;
  }
  banchItems.clear();
}

IoTBenchmark::~IoTBenchmark()
{
  for (auto it = banchItems.begin(); it != banchItems.end(); it++)
  {
    delete it->second;
  }
  banchItems.clear();
}
#endif
