#include "SDEinlesen.h"


int lokZeilenAnzahl = 0;
int weichenAnzahl = 0;

const char* lokCharArray[MAX_ZEILEN][MAX_SPALTEN_LOK/2];
int intArray[MAX_ZEILEN][MAX_SPALTEN_LOK/2];
const char* weicheCharArray[MAX_SPALTEN_WEICHEN];
int weicheIntArray[MAX_SPALTEN_WEICHEN];


// Methode um die Länge eines 1D-Arrays Herauszufinden
int Array1DLaenge(const char* Array[]) 
{
  int anzahlElemente = 0;
  while (Array[anzahlElemente] != nullptr) 
  {
    ++anzahlElemente;
  }
  anzahlElemente = anzahlElemente -1;
  return anzahlElemente;
}

void SetupSD()
{
  //Serial.println("Starte SD-Karten Initialisierung...");
  SPI.begin();  // Sicherstellen, dass der SPI-Bus gestartet wird
  bool sdInit = false;
  for (int i = 0; i < 3; i++) 
  {  // Maximal 3 Versuche
    if (SD.begin(SD_CS)) 
    {
      sdInit = true;
      //Serial.println("SD-Karte erfolgreich initialisiert!");
      break;
    }
    //Serial.println("SD-Karte nicht erkannt, neuer Versuch in 500ms...");
    delay(500);
  }

  if (!sdInit) 
  {
    //Serial.println("FEHLER: SD-Karte konnte nicht initialisiert werden!");
  }
}

void disableSD() 
{
  digitalWrite(SD_CS, HIGH);  // SD-Karte deaktivieren
  SPI.end();  // SPI-Bus freigeben (optional)
}

void SDLokEinlesen()
{
    File csvFile = SD.open(lokDateiPfad.c_str());
  if (!csvFile) 
  {
    //Serial.println("Fehler beim Öffnen der Datei!");
    return;
  }

  //Serial.println("Reading Lok Liste...");

  // Erste Zeile ignorieren
  if (csvFile.available()) {
    csvFile.readStringUntil('\n');
  }

  bool KeineWeiterenZeilen = true;
  int j = 0;

  // Lesen der CSV-Daten
  while (KeineWeiterenZeilen) {
    if (!csvFile.available() || j >= MAX_ZEILEN) {  
      KeineWeiterenZeilen = false;
      break;
    }

    String line = csvFile.readStringUntil('\n');
    line.trim();
    
    int i = 0;
    int start = 0;
    int index = 0;
    bool isTextColumn = true;

    while ((index = line.indexOf(';', start)) != -1 && i < MAX_SPALTEN_LOK) 
    {
      String value = line.substring(start, index);
      value.trim();

      if (isTextColumn) 
      {
        lokCharArray[j][i / 2] = strdup(value.c_str());  // String speichern
      } 
      else 
      {
        intArray[j][i / 2] = value.toInt();  // Integer speichern
      }

      isTextColumn = !isTextColumn;
      start = index + 1;
      i++;
    }

    // Letztes Element der Zeile speichern
    if (i < MAX_SPALTEN_LOK) 
    {
      String value = line.substring(start);
      value.trim();

      if (isTextColumn) 
      {
        lokCharArray[j][i / 2] = strdup(value.c_str());
      } else {
        intArray[j][i / 2] = value.toInt();
      }
    }

    j++;  // Nächste Zeile

  }

  csvFile.close();
  lokZeilenAnzahl = (j -1);  // Anzahl der eingelesenen Zeilen speichern
}



void SDWeicheEinlesen()
{
  File csvFile = SD.open(weichenDateiPfad.c_str());
  if (!csvFile) 
  {
    //Serial.println("Fehler beim Öffnen der Datei!");
    return;
  }

  //Serial.println("Reading Weichen Liste...");

  // Erste Zeile ignorieren
  if (csvFile.available()) 
  {
    csvFile.readStringUntil('\n');
  }
  
    String line = csvFile.readStringUntil('\n');
    line.trim();
    
    int i = 0;
    int start = 0;
    int index = 0;
    bool isTextColumn = true;

  while ((index = line.indexOf(';', start)) != -1 && i < MAX_SPALTEN_WEICHEN) 
  {
    String value = line.substring(start, index);
    value.trim();

    if (isTextColumn) 
    {
      weicheCharArray[i / 2] = strdup(value.c_str());  // String speichern
    } 
    else 
    {
      weicheIntArray[i / 2] = value.toInt();  // Integer speichern
    }

    isTextColumn = !isTextColumn;
    start = index + 1;
    i++;
  }  

  // Letztes Element der Zeile speichern
  if (i < MAX_SPALTEN_WEICHEN) 
  {
    String value = line.substring(start);
    value.trim();

    if (isTextColumn) 
    {
      weicheCharArray[i / 2] = strdup(value.c_str());
    } 
    else 
    {
      weicheIntArray[i / 2] = value.toInt();
    }
  }
  weichenAnzahl = i;
}

