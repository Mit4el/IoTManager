#pragma once
#include "Global.h"

#if defined (ESP8266) 
// эта библиотека встроена в ядро
#include "ESPAsyncUDP.h"
#elif defined(ESP32) || defined(LIBRETINY)
#include "AsyncUDP.h"
#endif
#ifndef LIBRETINY
extern AsyncUDP asyncUdp;
#endif

extern const String getThisDevice();
extern void addThisDeviceToList();
extern void udpListningInit();
extern void  udpBroadcastInit();
extern String uint8tToString(uint8_t* data, size_t len);
extern void udpPacketParse(String& data);
extern void jsonMergeArrays(String& existJson, String& incJson);