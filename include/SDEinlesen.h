#ifndef SDEINLESEN_H    // Falls SDEINLESEN_H nicht definiert ist...
#define SDEINLESEN_H    // definiere SDEINLESEN_H

#include <Arduino.h>
#include "defines.h"
#include <SPI.h>
#include <SD.h>
#include "FS.h"

int Array1DLaenge(const char* Array[]);
void SetupSD();
void DisableSD(); 
void SDLokEinlesen();
void SDWeicheEinlesen();

#endif  // End-Guard: Endet, falls SDEINLESEN_H bereits definiert ist