#include "defines.h"
#include "ESPNowSenden.h"



// Callback für das Senden von Daten
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) 
{
  //Serial.print("\r\nLast Packet Send Status:\t");
  //Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

char jsonData[100];

// Funktion, um Daten von einer LOK zu serialisieren und zu senden
void SerialisierenUndSenden(LOK &lok) 
{
  JsonDocument doc;
  lok.toJson(doc); // LOK-Objekt in JSON umwandeln

  serializeJson(doc, jsonData, sizeof(jsonData)); // JSON-Dokument in char[] schreiben
  esp_now_send(receiverAddress, (uint8_t *)jsonData, sizeof(jsonData)); // Daten senden
}

// Funktion, um Daten von einer WEICHE zu serialisieren und zu senden
void SerialisierenUndSenden(WEICHE &weiche) 
{
  JsonDocument doc;
  weiche.toJson(doc); // WEICHE-Objekt in JSON umwandeln

  serializeJson(doc, jsonData, sizeof(jsonData)); // JSON-Dokument in char[] schreiben
  esp_now_send(receiverAddress, (uint8_t *)jsonData, sizeof(jsonData)); // Daten senden
}
