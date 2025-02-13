#include <Arduino.h>
#include "defines.h"
#include "ESPNowSenden.h"
#include "InputVerarbeitung.h"
#include "Anzeige.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "time.h"


void startup() 
{
  // NVS Speicher initialisieren
  nvs_flash_init();
  nvs_handle_t nvs_handle;
  nvs_open("storage", NVS_READWRITE, &nvs_handle);

  // Zeitstempel aus NVS lesen
  uint32_t lastBootTime = 0;
  nvs_get_u32(nvs_handle, "last_boot", &lastBootTime);

  // Aktuelle Zeit holen
  uint32_t now = millis() / 1000;  // Zeit in Sekunden

  // Debugging auskommentiert
  // Serial.printf("Last Boot: %d sec ago\n", lastBootTime);
  // Serial.printf("Now: %d sec\n", now);

  if (lastBootTime == 0 || (now - lastBootTime) > RESET_TIMEOUT) {
      // Serial.println("ESP war länger als festgelegte Zeit aus -> Reset...");
      nvs_set_u32(nvs_handle, "last_boot", now);  // Neue Zeit speichern
      nvs_commit(nvs_handle);
      nvs_close(nvs_handle);
      delay(2000);
      esp_restart();  // ESP32-S3 zurücksetzen
  } else {
      // Serial.println("Kein Reset nötig.");
  }

  nvs_close(nvs_handle);
}

// Funktion für die erste Loop (Core 0)
void LoopCore0DCC(void *parameter)
{
  while (true)
  {
    EingabeErkennen();
    vTaskDelay(3 / portTICK_PERIOD_MS);  // 5ms Pause
  }
}
 
// Funktion für die zweite Loop (Core 1)
void LoopCore1ESPNow(void *parameter)
{
  //vTaskDelay(2000 / portTICK_PERIOD_MS);  // 1 Sekunde warten
  while(true)
  {
    DisplayAnzeignAuswahl();
    vTaskDelay(2 / portTICK_PERIOD_MS);  // 10ms Pause
  }
}

 
// Hilfsmethode zum Erstellen einer Task
void CreateTask(void (*taskFunction)(void *), const char *taskName, uint32_t stackSize, uint8_t priority, uint8_t core)
{
  xTaskCreatePinnedToCore
  (
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

  SetupSD();  // SPI für den Kartenleser initialisieren
  delay(100); // Warten, bis der SD-Kartenleser bereit ist

  SDLokEinlesen();  // Einlesen der Lok-Datei
  SDWeicheEinlesen(); // Einlesen der Weichen-Datei

  delay(200); // Warten bis die Dateinen eingelesen sind
  DisableSD();  // SPD des Kartenllesers deaktivieren
  delay(100); // Warten bis der Kartenleser deaktiviert ist

  SetupDisplay();  // Display initialisieren

  SetupEncoder();

  // Tasks erstellen
  CreateTask(LoopCore0DCC, "TaskCore0", 10000, 1, 0); // Task auf Core 0
  CreateTask(LoopCore1ESPNow, "TaskCore1", 10000, 1, 1); // Task auf Core 1

  SetupESP_NOW();
  startup();  // Überprüfen ob ein Reset nötig ist
}


void loop() 
{
  // Der normale loop()-Code bleibt leer oder wird nur minimal verwendet

}
