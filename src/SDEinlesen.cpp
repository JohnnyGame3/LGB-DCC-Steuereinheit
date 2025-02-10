#include "SDEinlesen.h"


int lokZeilenAnzahl = 0;
int weichenAnzahl = 0;
/*
const char* lokCharArray[10][MAX_SPALTEN_LOK/2];
int intArray[10][MAX_SPALTEN_LOK/2];
const char* weicheCharArray[7];
int weicheIntArray[7];
*/

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
  //Serial.println("Initializing SD card...");
  if (!SD.begin(SD_CS)) 
  {
    Serial.println("Card initialization failed!");
    while (true);
  }
  Serial.println("SD OK");
}



void SDLokEinlesen()
{
    File csvFile = SD.open(lokDateiPfad.c_str());
  if (!csvFile) 
  {
    Serial.println("Fehler beim Öffnen der Datei!");
    return;
  }

  Serial.println("Reading Lok Liste...");

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
  lokZeilenAnzahl = j;  // Anzahl der eingelesenen Zeilen speichern
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


const char* lokCharArray[100][MAX_SPALTEN_LOK/2] = 
{
  {"/", "/", "/", "/", "/", "/", "/", "/", "/", "/"},
  {"Mallet", "Licht", "Sound", "Steckdose", "Dampf", "/", "/", "/", "/", "/"},
  {"Diesellok V51", "Licht", "Sound", "Steckdose", "Dampf", "/", "/", "/", "/", "/"},
  {"Kokodil", "Licht", "Sound", "Steckdose", "Dampf", "/", "/", "/", "/", "/"},
  {"Diesellok D10", "Licht", "Rundum-Licht", "Entkuppler H", "Entkuppler V", "/", "/", "/", "/", "/"},
  {"Lok g2", "L 2 / F 1", "L 2 / F 2", "L 2 / Z", "/", "/", "/", "/", "/", "/"},
  {"/", "/", "/", "/", "/", "/", "/", "/", "/", "/"}
};

int intArray[100][MAX_SPALTEN_LOK/2] =
{
  {0,0,0,0,0,0,0,0,0,0},
  {4,0,1,2,3,0,0,0,0,0},
  {5,0,1,2,3,4,5,6,7,8},
  {6,0,1,2,3,4,5,6,7,8},
  {10,0,1,2,3,4,0,0,0,0},
  {3,0,1,2,3,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0}
};

const char* weicheCharArray[50] =
{
  "Weiche 1","Weiche 2","Weiche 3","Signal 1","-------",
};

int weicheIntArray[50] =
{
  129,130,131,132,133,
};
