#ifndef STEUERUNG_H
#define STEUERUNG_H

#include <Arduino.h>
#include "defines.h"
#include "ESPNowSenden.h"
#include "AiEsp32RotaryEncoder.h"


int EncoderEinlesen(int minValue, int maxValue, int startWert);  // Funktion zum Einlesen des Encoder-Wertes
void IRAM_ATTR readEncoderISR();    // Interrupt Service Routine (ISR) für den Encoder
void EncoderSetup();    // Funktion zur Initialisierung des Encoders
bool EncoderButtonGedrueckt();


// Funktionsprototypen
bool DebounceTaster(int pin, int index);
int PotiEinlesen(int pin);
void EingabeErkennen();
void MenueRad();

#endif // STEUERUNG_H