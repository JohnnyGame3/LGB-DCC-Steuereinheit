#include "defines.h"
#include "ESPNowSenden.h"

esp_now_peer_info_t peerInfo;

void SetupESP_NOW()
{
  // Setze den ESP32 als Wi-Fi Station
  WiFi.mode(WIFI_STA);


  // Initialisiere ESP-NOW
  if (esp_now_init() != ESP_OK) 
  {
    //Serial.println("Fehler beim Initialisieren von ESP-NOW");
    return;
  }  

  // Registriere den Callback für das Senden von Daten
  esp_now_register_send_cb(OnDataSent);   
  
  // Füge den Peer (Empfänger) hinzu
  memcpy(peerInfo.peer_addr, receiverAddress, 6);
  peerInfo.channel = 0;  // Kanal 0 verwenden
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) 
  {
    //Serial.println("Fehler beim Hinzufügen des Empfängers");
    return;
  }
}

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
