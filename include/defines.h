#ifndef DEFINES_H    // Falls DEFINES_H nicht definiert ist...
#define DEFINES_H    // definiere DEFINES_H

#include <Arduino.h>

#include "LOK.h"
#include "WEICHE.h"
#include <string> // Für std::string

// Ziel-MAC-Adresse
const uint8_t receiverAddress[] = {0x98, 0x3D, 0xAE, 0xEB, 0x04, 0x74};  // Mac Adresse des Empfänger esp´s Muss geändert werden

//const uint8_t receiverAddress[] = {0x93, 0x3D, 0xAE, 0xEC, 0xBE, 0xDC};

// Array Constanten
const int MAX_ZEILEN = 100;
const int MAX_SPALTEN_LOK = 20;
const int MAX_SPALTEN_WEICHEN = 100;
extern int lokZeilenAnzahl;
extern int weichenAnzahl;
const std::string lokDateiPfad = "/Excel Lok Test CSV.csv";
const std::string weichenDateiPfad = "/Excel Weichen Test CSV.csv";

// Arrays 
extern int intArray[][MAX_SPALTEN_LOK/2];
extern const char* lokCharArray[][MAX_SPALTEN_LOK/2];
extern int weicheIntArray[];
extern const char* weicheCharArray[];


// Array Werte Display
extern int w1;      // Weiche/Signal Pos 1 (Oben Links)
extern int w2;      // Weiche/Signal Pos 2 (Oben Rechts)
extern int geschwAktuell;  // Geschwindigkeit (Oben Mitte)
extern int l1;      // Lok 1 (Mitte, Links/Mitte/Rechts)
extern int l2;      // Lok 2 (Unten, Links/Mitte/Rechts)
extern int l1z;     // Lok 1 Zusatzfunktion (Mitte Rechts)
extern int l2z;     // Lok 1 Zusatzfunktion (Unten Rechts)

// Einzelnde Lok Geschwindigkeiten
extern int geschwLok1;  // Geschwindigkeit der 1.Lok     
extern int geschwLok2;  // Geschwindigkeit der 2.Lok 
extern int letzteDisplayGeschw; // Geschwindigkeit die auf dem display angezeigt wird

#pragma region // Pin Belegung

const int ANZAHL_TASTER = 11;  // Anzahl der Taster anpassen
// Taster Pin Belegung
const int TASTER_W1 = 4;     //Taster Weiche/Signal 1
const int TASTER_W2 = 5;     //Taster Weiche/Signal 2
const int TASTER_LOK1_F1 = 6; //Taster Lok 1 Favorit 1
const int TASTER_LOK1_F2 = 7; //Taster Lok 1 Favorit 2
const int TASTER_LOK1_Z = 8;  //Taster Lok 1 Zusatz
const int TASTER_LOK2_F1 = 14; //Taster Lok 2 Favorit 1
const int TASTER_LOK2_F2 = 15; //Taster Lok 2 Favorit 2
const int TASTER_LOK2_Z = 16;  //Taster Lok 2 Zusatz
const int TASTER_LOK1_AKTIV = 17; // Taster zum Aktivieren der 1.Lok 
const int TASTER_LOK2_AKTIV = 47; // Taster zum AKtivieren der 2.Lok
const int POTI = 18;       //Potty input für Geschwindigkeit 

// Multifunktions Rad
const int ROTARY_ENCODER_A_PIN = 10;
const int  ROTARY_ENCODER_B_PIN = 9;
const int ROTARY_ENCODER_BUTTON_PIN = 48; // Taster ders Encoders
const int ROTARY_ENCODER_VCC_PIN = -1;
const int  ROTARY_ENCODER_STEPS = 4;

// TFT Display
const int TFT_DC = 38;
const int TFT_CS = 37;
const int TFT_MOSI = 11;
const int TFT_SCK = 12;

// SD-Kartenleser (SPI)
const int SD_MISO = 13;    // SPI MISO
const int SD_MOSI = 11;    // SPI MOSI (gemeinsam mit TFT)
const int SD_SCK = 12;     // SPI SCK (gemeinsam mit TFT)
const int SD_CS = 3;     // Chip Select für SD-Karte
#pragma endregion

// Variablen Für schalt-Zustände
extern bool w1s;        // Zustand Weiche/Signal/... 1
extern bool w2s;        // Zustand Weiche/Signal/... 2
extern bool l1F1s;      // Zustand Lok 1 Favorit 1
extern bool l1F2s;      // Zustand Lok 1 Favorit 2
extern bool l1Zs;       // Zustand Lok 1 Zusatz
extern bool l2F1s;      // Zustand Lok 2 Favorit 1
extern bool l2F2s;      // Zustand Lok 2 Favorit 2
extern bool l2Zs;       // Zustand Lok 2 Zusatz

extern bool geschwLok1Aktiv;    // Zum Feststellen ob Lok 1 Geschwindigkeitsänderunegen bekommt
extern bool geschwLok2Aktiv;    // Zum Feststellen ob Lok 2 Geschwindigkeitsänderunegen bekommt
extern bool lok1Richtung;   // Zum Festlegen der Aktuellen Fahrtrichtung der 1.Lok
extern bool lok2Richtung;   // Zum Festlegen der Aktuellen Farhtrichtung der 2.Lok

extern bool startwertSetzen;    // Muss Auserhalb der Encoder schleife sitzen um einen Startwert des encoders einzustellen
extern bool imAuswahlMenue; // Zum Feststellen ob Mann sich in einem Auswahl Menü Befindet oder nicht
extern int auswahlHauptAnzeige; // Int Wert der Bestimmt was gerade ausewählt ist zum Ändrn
extern bool tasterBetaetigt;    // true wenn ein Taster Betätigt wurde
extern bool tasterLokAktivGedrueckt;    // true wenn ein Lok Anwahl taster betätigt wurde
extern bool radInaktivEinzelAktualisierung; // bool für die auswahlmöglichkeiten nochmal zu aktualisieren wenn der Encoder "Inaktiv" geworden ist 


const unsigned long DEBOUNCE_ZEIT = 200; // Globale Debounce-Zeit in Millisekunden
const int TFT_BREITE = 320; // Breite des TFT Displays
const int TFT_HOEHE = 240;  // Höhe des TFT Displays

// Globale Instanzen deklarieren (nicht definieren!)
extern LOK lok1;
extern LOK lok2;
extern WEICHE weiche1;
extern WEICHE weiche2;

#endif  // End-Guard: Endet, falls DEFINES_H bereits definiert ist