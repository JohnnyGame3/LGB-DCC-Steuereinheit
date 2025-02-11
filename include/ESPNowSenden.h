#ifndef ESPNOWSENDEN_H    // Falls DEFINES_H nicht definiert ist...
#define ESPNOWSENDEN_H    // definiere DEFINES_H

#include <esp_now.h>
#include <ArduinoJson.h>
#include "defines.h"
#include <WiFi.h>

void SetupESP_NOW();
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);

void SerialisierenUndSenden(LOK &lok); 
void SerialisierenUndSenden(WEICHE &weiche); 


#endif  // End-Guard: Endet, falls DEFINES_H bereits definiert ist