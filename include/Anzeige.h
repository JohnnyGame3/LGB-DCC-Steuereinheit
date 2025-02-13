#ifndef ANZEIGE_H
#define ANZEIGE_H

#include <Arduino.h>
#include "defines.h"

#include "SDEinlesen.h"
#include "InputVerarbeitung.h"


#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

void SetupDisplay();
int getWrappedIndex(int aktuellerIndex, int offset, int minIndex, int maxIndex); 


void GitterZeichnen();
void ErsteLokZeile();
void ZweiteLokZeile();
void GeschwindigkeitAnzeigen();
void StandardAnzeige();
void StandardAnzeigeVorZeichnen();

void AuswahlMenueVorZeichnen(); // Methode um das Auswahl-Menü Vor zu Zeichnen
int WeicheAendern(int arrayNr);
int LokAendern(int arrayNr);
int ZusatzAendern(int lokNr, int arrayNr);

void DisplayAnzeignAuswahl();

#endif // STEUERUNG_H