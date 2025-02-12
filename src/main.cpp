#include <Arduino.h>
#include "defines.h"
#include "ESPNowSenden.h"
#include "InputVerarbeitung.h"
#include "Anzeige.h"


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
  while(true)
  {
    DisplayAnzeignAuswahl();
    vTaskDelay(1 / portTICK_PERIOD_MS);  // 10ms Pause
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
  disableSD();  // SPD des Kartenllesers deaktivieren
  delay(100); // Warten bis der Kartenleser deaktiviert ist

  DisplaySetup();  // Display initialisieren


  SetupEncoder();

  // Tasks erstellen
  CreateTask(LoopCore0DCC, "TaskCore0", 10000, 1, 0); // Task auf Core 0
  CreateTask(LoopCore1ESPNow, "TaskCore1", 10000, 1, 1); // Task auf Core 1

  SetupESP_NOW();
  delay(1000);  // Warten bis alles bereit ist 
}


void loop() 
{
  // Der normale loop()-Code bleibt leer oder wird nur minimal verwendet

}
