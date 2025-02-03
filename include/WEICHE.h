#ifndef WEICHE_H
#define WEICHE_H

#include <ArduinoJson.h>

class WEICHE {
public:
    int id, weiche;
    bool zustand;

    WEICHE(int id, int weiche, bool zustand);
    void toJson(JsonDocument &doc);
};

#endif // WEICHE_H
