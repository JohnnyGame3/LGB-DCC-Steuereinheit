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
/**
 * Methode zur Berechnung eines zyklischen Indexes innerhalb eines bestimmten Bereichs.
 * 
 * @param aktuellerIndex Der aktuelle Indexwert.
 * @param offset         Die Anzahl der Schritte, um den Index zu verschieben.
 * @param minIndex       Der minimale erlaubte Indexwert.
 * @param maxIndex       Der maximale erlaubte Indexwert.
 * @return               Der neue, berechnete Index innerhalb des gültigen Bereichs.
 */
int getWrappedIndex(int aktuellerIndex, int offset, int minIndex, int maxIndex); 
void MenueVorZeichnen(uint16_t trennLinienFarbe); // Methode um das Auswahl-Menü Vor zu Zeichnen
/**
 * Methode zur Anzeige eines Textes an einer bestimmten Position mit variabler Farbe.
 * 
 * @param x         X-Koordinate für die Anzeige des Textes.
 * @param y         Y-Koordinate für die Anzeige des Textes.
 * @param text      Der anzuzeigende Text.
 * @param bedingung Boolean-Wert: Bestimmt die Textfarbe (rot oder weiß).
 */
void DruckeTextMitFarbe(int x, int y, const char* text, bool bedingung); 
/**
 * Methode zur Anzeige eines Status an einer bestimmten Position auf dem Display.
 * 
 * @param x       X-Koordinate für die Anzeige des Textes.
 * @param y       Y-Koordinate für die Anzeige des Textes.
 * @param text1   Text, der angezeigt wird, wenn die Statusvariable "true" ist.
 * @param text2   Text, der angezeigt wird, wenn die Statusvariable "false" ist.
 * @param status  Boolean-Wert, der bestimmt, welcher Text angezeigt wird.
 */
void StatusAnzeigen(int x, int y, const char* text1, const char* text2, bool status);
void ErsteLokZeile();
void ZweiteLokZeile();
void GeschwindigkeitAnzeigen();
/**
 * Methode zur Anzeige der Funktionen einer Lok an einer bestimmten Position auf dem Display.
 * 
 * @param x        X-Koordinate für die Anzeige der Funktionen.
 * @param y        Y-Koordinate für die Anzeige der Funktionen.
 * @param status1  Boolean-Wert für den Status der ersten Funktion.
 * @param status2  Boolean-Wert für den Status der zweiten Funktion.
 * @param statusZ  Boolean-Wert für den Status der Zusatzfunktion.
 */
void LokFunktionenAnzeigen(int x, int y, bool status1, bool status2, bool statusZ); 
void StandardAnzeige();
void StandardAnzeigeVorZeichnen();

int WeicheAendern(int arrayNr);
int LokAendern(int arrayNr);
int ZusatzAendern(int lokNr, int arrayNr);

void DisplayAnzeignAuswahl();

#endif // STEUERUNG_H