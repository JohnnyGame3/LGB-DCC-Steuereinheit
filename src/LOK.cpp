#include "LOK.h"
#include "defines.h"

// Konstruktor-Definition
LOK::LOK(int id, int lok, int speed, int funktion, bool zustand, bool richtung) 
    : id(id), lok(lok), speed(speed), funktion(funktion), zustand(zustand), richtung(richtung) {}

// JSON-Serialisierungsmethode
void LOK::toJson(JsonDocument &doc) 
{
    doc["id"] = id;
    doc["lok"] = lok;
    doc["speed"] = speed;
    doc["funktion"] = funktion;
    doc["zustand"] = zustand;
    doc["richtung"] = richtung;
}

LOK lok1(1, intArray[l1][0], 10, intArray[l1][1], false, true);
LOK lok2(2, intArray[l2][0], 0, intArray[l2][1], false, true);