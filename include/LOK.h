#ifndef LOK_H
#define LOK_H

#include <ArduinoJson.h>

class LOK {
public:
    int id, lok, speed, funktion;
    bool zustand, richtung;

    LOK(int id, int lok, int speed, int funktion, bool zustand, bool richtung);

    void toJson(JsonDocument &doc);
};

#endif // LOK_H
