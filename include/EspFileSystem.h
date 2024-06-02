#pragma once
#include "Global.h"
#ifdef ESP32
#include <rom/spi_flash.h>
#if USE_LITTLEFS
#include <LittleFS.h>
#define FileFS LittleFS
#define FS_NAME "LittleFS_32"
#define CONFIG_LITTLEFS_SPIFFS_COMPAT 1
#else
#include <SPIFFS.h>
extern FS* filesystem;
#define FileFS SPIFFS
#define FS_NAME "SPIFFS_32"
#endif
#endif

#if defined(libretiny)
#include <FS.h>
//extern FS* filesystem;
//extern fs::FS SPIFFS;
/*namespace fs {
class FS;
}*/
//using FS = fs::FS;
//#define FS _fs
/*
fs::FS _fs = fs::FS(fs::FSImplPtr(new fs::FSImpl::FSImpl()));
#define FileFS  _fs
#define FS_NAME "FS_32"
*/
#include "LittleFS.h"
//extern FS LittleFS;
//using littlefs_impl::LittleFSConfig;
//extern FS* filesystem;
#define FileFS LittleFS
#define FS_NAME "LittleFS_LT"
#define FILE_READ "r"
#define FILE_WRITE "w"
#define FILE_APPEND "a"
#endif

#ifdef ESP8266
#if USE_LITTLEFS
#include "LittleFS.h"
extern FS LittleFS;
using littlefs_impl::LittleFSConfig;
extern FS* filesystem;
#define FileFS LittleFS
#define FS_NAME "LittleFS_8266"
#define FILE_READ "r"
#define FILE_WRITE "w"
#define FILE_APPEND "a"
#else
extern FS* filesystem;
#define FileFS SPIFFS
#define FS_NAME "SPIFFS_8266"
#endif
#endif

extern bool fileSystemInit();
extern void globalVarsSync();

// extern String getParamsJson();

extern void syncSettingsFlashJson();
extern void syncValuesFlashJson();

extern const String getChipId();
extern void setChipId();
extern const String getUniqueId(const char* name);
extern uint32_t ESP_getChipId(void);
extern uint32_t ESP_getFlashChipId(void);
extern const String getMacAddress();
extern const String getWebVersion();
extern uint32_t getFlashChipIdNew();
