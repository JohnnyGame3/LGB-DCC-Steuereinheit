#include "WEICHE.h"
#include "defines.h"

// Konstruktor-Definition
WEICHE::WEICHE(int id, int weiche, bool zustand) 
    : id(id), weiche(weiche), zustand(zustand) {}

// JSON-Serialisierungsmethode
void WEICHE::toJson(JsonDocument &doc) 
{
    doc["id"] = id;
    doc["weiche"] = weiche;
    doc["zustand"] = zustand;
}

WEICHE weiche1(3,weicheIntArray[w1],true);
WEICHE weiche2(4,weicheIntArray[w2],true);