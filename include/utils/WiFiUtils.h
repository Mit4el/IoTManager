#pragma once

#include "Global.h"
#include "MqttClient.h"

//void addPortMap(String TCP_UDP, String maddr, u16_t mport, String daddr, u16_t dport);

boolean isNetworkActive();
uint8_t getNumAPClients();
void routerConnect();
bool startAPMode();
boolean RouterFind(std::vector<String> jArray);
uint8_t RSSIquality();
extern void wifiSignalInit();
 String httpGetString(HTTPClient &http);