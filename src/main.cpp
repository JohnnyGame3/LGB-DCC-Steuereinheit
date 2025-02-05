#include <Arduino.h>
#include "defines.h"
#include <WiFi.h>
#include "ESPNowSenden.h"
#include "InputVerarbeitung.h"
#include "Anzeige.h"

esp_now_peer_info_t peerInfo;


// Funktion für die erste Loop (Core 0)
void LoopCore0DCC(void *parameter)
{
  while (true)
  {
    EingabeErkennen();
    vTaskDelay(2 / portTICK_PERIOD_MS);  // 5ms Pause
  }
}
 
// Funktion für die zweite Loop (Core 1)
void LoopCore1ESPNow(void *parameter)
{
  while(true)
  {
    DisplayAnzeignAuswahl();
    //vTaskDelay(2 / portTICK_PERIOD_MS);  // 10ms Pause
  }
}

 
// Hilfsmethode zum Erstellen einer Task
void createTask(void (*taskFunction)(void *), const char *taskName, uint32_t stackSize, uint8_t priority, uint8_t core)
{
  xTaskCreatePinnedToCore(
    taskFunction,   // Zeiger auf die Task-Funktion
    taskName,       // Name der Task
    stackSize,      // Stackgröße in Bytes
    NULL,           // Parameter für die Task (optional)
    priority,       // Priorität der Task
    NULL,           // Task-Handle (nicht verwendet)
    core            // Core, auf dem die Task laufen soll
  );
}

void setup() 
{
  for(int i = 4; i <= 8; i++)
  {
    pinMode(i,INPUT_PULLUP);
  }
  for(int i = 14; i <= 17; i++)
  {
    pinMode(i,INPUT_PULLUP);
  }
  pinMode(TASTER_LOK2_AKTIV,INPUT_PULLUP);
  pinMode(POTI, INPUT_PULLUP);
  
  //Serial.begin(115200);
  
  // Setze den ESP32 als Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Initialisiere SD-Karten Leser
  //SetupSD();

  // Initialisiere Encoder
  EncoderSetup();

  // Initialisiere Display
  displaySetup(); 


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

  //Serial.println("Sender bereit.");

  // Tasks erstellen
  createTask(LoopCore0DCC, "TaskCore0", 10000, 1, 0); // Task auf Core 0
  createTask(LoopCore1ESPNow, "TaskCore1", 10000, 1, 1); // Task auf Core 1

}


void loop() 
{
  //EingabeErkennen();
  // Der normale loop()-Code bleibt leer oder wird nur minimal verwendet

}
