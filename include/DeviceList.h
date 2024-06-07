#pragma once
#include "Global.h"

#if defined (ESP8266) || defined(libretiny)
// эта библиотека встроена в ядро
#include "ESPAsyncUDP.h"
#elif defined(ESP32)
#include "AsyncUDP.h"
#endif
#ifndef libretiny
extern AsyncUDP asyncUdp;
#endif

extern const String getThisDevice();
extern void addThisDeviceToList();
extern void udpListningInit();
extern void  udpBroadcastInit();
extern String uint8tToString(uint8_t* data, size_t len);
extern void udpPacketParse(String& data);
extern void jsonMergeArrays(String& existJson, String& incJson);