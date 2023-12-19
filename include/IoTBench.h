#pragma once
#include "Global.h"
#include <map>

// #define OS_MICROS
#define OS_BENCH
#ifdef OS_BENCH
struct ItemBench
{
  uint32_t loopTime = 0, loopTimeMax = 0, count = 0;
};

class IoTBenchmark
{
public:
  IoTBenchmark();
  ~IoTBenchmark();
  static IoTBenchmark *instance();
  // получить загруженность процессора в процентах
  int getLoad();
  // включить измерение загруженности процессора за указанный период в мс
  void enableLoad(uint32_t loadp = 0);
  // отключить измерение загруженности процессора
  void disableLoad();
  // подключить функцию обработчик задачи
  void attach(const String& id);

  void initLoadFunction();
  void postLoadFunction();

  void preBenchFunction(const String& id);
  void postBenchFunction(const String& id);
  // получить максимальное время выполнения выбранной ранее задачи в мкс
  uint32_t getMaxLoopTime(const String& id);

  void clearBenchConfig();

private:
  static IoTBenchmark *mInstance;

  uint32_t loadP = 0;   // период подсчета загруженности процессора
  uint32_t us = 0;      // время начало цикла loop
  uint32_t loadTmr = 0; // время начала подсчета benchmark, uptime()
  uint32_t loadSum = 0; // время выполнния всех циклов loop за период  loadP
  int load = 0;         // загруженность процессора в процентах за период loadP
                        // (loadSum / 1000) / loadP * 100
  uint32_t count = 0; //количестов циклов loop в сек в среднем за период loadP

  inline uint32_t uptime() { return usMode ? micros() : millis(); }

#ifdef OS_MICROS
  bool usMode = 1;
#else
  bool usMode = 0;
#endif
  std::map<String, ItemBench *> banchItems;
};
#endif
