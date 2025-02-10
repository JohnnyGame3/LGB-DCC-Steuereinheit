#include "InputVerarbeitung.h"

bool imAuswahlMenue = false; 

bool geschwLok1Aktiv = false;
bool geschwLok2Aktiv = false;
bool lok1Richtung = true;
bool lok2Richtung = true;
bool tasterBetaetigt = false;
bool tasterLokAktivGedrueckt = false;

int geschwAktuell = 0;
int geschwLok1 = 0;
int geschwLok2 = 0;


#pragma region // Encoder

//instead of changing here, rather change numbers above
AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);

// Funktion zum Einlesen des Encoder-Wertes
int EncoderEinlesen(int minValue, int maxValue, int startWert)
{
  rotaryEncoder.setBoundaries(minValue, maxValue, true); // Grenzen für den Encoder setzen (0 bis maxValue, mit Überlauf/Unterlauf)

  if (!startwertSetzen) 
  {
    rotaryEncoder.setEncoderValue(startWert);  // Startwert nur einmal setzen
    startwertSetzen = true;  // Setzen des Flags, um den Startwert nur einmal zu setzen
  }

  if (rotaryEncoder.encoderChanged()) // Überprüfen, ob sich der Encoder-Wert geändert hat
  {
    return rotaryEncoder.readEncoder(); // Den aktuellen Encoder-Wert zurückgeben
  }

  return startWert;  // Falls keine Änderung, Startwert zurückgeben
}

// Interrupt Service Routine (ISR) für den Encoder
void IRAM_ATTR readEncoderISR()
{
  rotaryEncoder.readEncoder_ISR();  // Den Encoder-Wert in der ISR aktualisieren
}

bool EncoderButtonGedrueckt()
{
  return rotaryEncoder.isEncoderButtonClicked();
}

// Funktion zur Initialisierung des Encoders
void EncoderSetup()
{
  rotaryEncoder.begin();  // Encoder starten
  rotaryEncoder.setup(readEncoderISR);  // Interrupt für den Encoder einrichten
  rotaryEncoder.setAcceleration(150);  // Beschleunigung für den Encoder setzen (Wert bestimmt die Reaktionsgeschwindigkeit)
  //rotaryEncoder.isEncoderButtonClicked();
}

#pragma endregion

struct TasterStatus 
{
  unsigned long lastPressTime;
  bool buttonLock;
};

TasterStatus taster[ANZAHL_TASTER];


// Methoder zum Debouncen der Taster sie gibt einen true Zurück wenn der Taster betätigt wurde
bool DebounceTaster(int pin, int index) 
{
  unsigned long currentTime = millis();
  bool currentButtonState = digitalRead(pin); 

  // Wenn der Taster gedrückt wurde und nicht gesperrt ist
  if (currentButtonState == LOW && !taster[index].buttonLock) 
  {
    if (currentTime - taster[index].lastPressTime > DEBOUNCE_ZEIT) 
    {
      taster[index].lastPressTime = currentTime;
      taster[index].buttonLock = true; // Sperrt erneute Erkennung bis Loslassen
      return true;
    }
  }

  // Wenn der Taster losgelassen wird, Lock zurücksetzen
  if (currentButtonState == HIGH) 
  {
    taster[index].buttonLock = false;
  }

  return false;
}

  int deadZone = 150; // Totzone
  int offset = -320;    // Offset für den Nullpunkt (nach rechts verschoben)
  int nullpunkt = 2048 + offset; // Neuer Nullpunkt mit Offset

// Methoder um den Poti einzulesen und den wert in -28 bis 28 zu mappen
int PotiEinlesen(int pin)
{
  int potiWert = analogRead(pin); // Liest den Wert des Poti ein

  int speed;

  if (potiWert >= (nullpunkt - deadZone) && potiWert <= (nullpunkt + deadZone)) 
  {
    speed = 0;
  } 
  else if (potiWert > (nullpunkt + deadZone)) 
  {
    speed = map(potiWert, nullpunkt + deadZone, 4095, 0, -28);
  }
  else 
  {
    speed = map(potiWert, 0, nullpunkt - deadZone, 28, 0);
  }

  return speed;
}

// Erkennt die tastereingabe und sendet die neuen funktions/Geschw. Befehlle
void EingabeErkennen()
{
  geschwAktuell = PotiEinlesen(POTI); // Poti wert wird eingelesen und in -28 bis 28 umgewandelt
    
  #pragma region // Weichen
    if(DebounceTaster(TASTER_W1, 0))    // Weiche 1
    {
      w1s = !w1s;
      weiche1.weiche = weicheIntArray[w1]; // Ändert die Adresse der 1.Weiche 
      weiche1.zustand = w1s;  // Ändert den Zustand der 1.Weiche
      SerialisierenUndSenden(weiche1);  // Daten der ersten Weiche senden
      tasterBetaetigt = true;
    }
    if(DebounceTaster(TASTER_W2, 1))    // Weiche 2
    {
      w2s = !w2s;
      weiche2.weiche = weicheIntArray[w2]; // Ändert die Adresse der 2.Weiche
      weiche2.zustand = w2s;  // Ändert den Zustand der 2.Weiche
      SerialisierenUndSenden(weiche2);  // Daten der zweiten Weiche senden
      tasterBetaetigt = true;
    }
  #pragma endregion

  #pragma region // Lok 1 Funktion
    if(DebounceTaster(TASTER_LOK1_F1, 2))    // Lok 1 Favorit 1 Schalten
    {
      l1F1s =!l1F1s;
      lok1.lok = intArray[l1][0]; // Legt die Adresse der Lok fest
      lok1.funktion = intArray[l1][1]; 
      lok1.zustand = l1F1s; // Legt fest ob funktion Favorit 1 Ein oder aus ist 
      SerialisierenUndSenden(lok1);  // Daten der ersten Lok senden
      tasterBetaetigt = true;
    }
    if(DebounceTaster(TASTER_LOK1_F2, 3))    // Lok 1 Favorit 2 Schalten
    {
      l1F2s =!l1F2s;
      lok1.lok = intArray[l1][0]; // Legt die Adresse der Lok fest
      lok1.funktion = intArray[l1][2];
      lok1.zustand = l1F2s; // Legt fest ob funktion Favorit 2 Ein oder aus ist 
      SerialisierenUndSenden(lok1);  // Daten der ersten Lok senden
      tasterBetaetigt = true;
    }
    if(DebounceTaster(TASTER_LOK1_Z, 4))     // Lok 1 Zusatz Schalten
    {
      l1Zs =!l1Zs;
      lok1.lok = intArray[l1][0]; // Legt die Adresse der Lok fest
      lok1.funktion = intArray[l1][l1z];
      lok1.zustand = l1Zs;  // Legt fest ob funktion X(Zusatzfunktion) Ein oder aus ist       
      SerialisierenUndSenden(lok1);  // Daten der ersten Lok senden
      tasterBetaetigt = true;
    }
    if(DebounceTaster(TASTER_LOK1_AKTIV, 5)) // Lok 1 Geschwindigkeit Aktivieren/Deaktivieren 
    {
      geschwLok1Aktiv = !geschwLok1Aktiv;

      tasterLokAktivGedrueckt = true;
    }
  #pragma endregion

  #pragma region // Lok 2 Funktion
    if(DebounceTaster(TASTER_LOK2_F1, 6))    // Lok 2 Favorit 1 Schalten
    {
      l2F1s =!l2F1s;      
      lok2.lok = intArray[l2][0]; // Legt die Adresse der Lok fest
      lok2.funktion = intArray[l2][1];
      lok2.zustand = l2F1s;
      SerialisierenUndSenden(lok2);  // Daten der zweiten Lok senden
      tasterBetaetigt = true;
    }
    if(DebounceTaster(TASTER_LOK2_F2, 7))    // Lok 2 Favorit 2 Schalten
    {
      l2F2s =!l2F2s;      
      lok2.lok = intArray[l2][0]; // Legt die Adresse der Lok fest
      lok2.funktion = intArray[l2][2];
      lok2.zustand = l2F2s;
      SerialisierenUndSenden(lok2);  // Daten der zweiten Lok senden
      tasterBetaetigt = true;
    }
    if(DebounceTaster(TASTER_LOK2_Z, 8))     // Lok 2 Zusatz Schalten
    {
      l2Zs =!l2Zs;      
      lok2.lok = intArray[l2][0]; // Legt die Adresse der Lok fest
      lok2.funktion = intArray[l2][l2z];
      lok2.zustand = l2Zs;
      SerialisierenUndSenden(lok2);  // Daten der zweiten Lok senden
      tasterBetaetigt = true;
    }
    if(DebounceTaster(TASTER_LOK2_AKTIV, 9)) // Lok 1 Geschwindigkeit Aktivieren/Deaktivieren 
    {
      geschwLok2Aktiv = !geschwLok2Aktiv;

      tasterLokAktivGedrueckt = true;
    }
  #pragma endregion
    
  #pragma region // Loks Geschwindigkeit
    if((geschwLok1 != geschwAktuell)&&(geschwLok1Aktiv == true)) // Wenn die Eingestellte Geschwindigkeit sich verändert hat und die Lok Aktiv gewählt wurde
    {
      geschwLok1 = geschwAktuell;  // Setzt die Geschwindigkeit der Lok auf die Aktuelle geschwindigkeit
      if(geschwAktuell < 0)   // Wenn geschwindigkeit Kleiner 0 
      {
        geschwLok1 = (geschwLok1 * -1); // Entfernt das Minus der geschwindigkeit
        lok1Richtung = false;   // Setzt die richtung auf false (Rückwärts)
      }
      else    // Wenn geschwindigkeit größer gleich 0
      {
        lok1Richtung = true;    // Setzt die richtung auf true (Vorwärts)
      }
      lok1.lok = intArray[l1][0]; // Legt die Adresse der Lok fest
      lok1.speed = geschwLok1;
      lok1.richtung = lok1Richtung;
      SerialisierenUndSenden(lok1);  // Daten der ersten Lok senden
      geschwLok1 = geschwAktuell; // Setzt die Geschwindigkeit der Lok erneut auf die Aktuelle geschwindigkeit um evtl minus werte erneut zu setzen
    } 
    if((geschwLok2 != geschwAktuell)&&(geschwLok2Aktiv == true))
    {
      geschwLok2 = geschwAktuell;  // Setzt die Geschwindigkeit der Lok auf die Aktuelle geschwindigkeit
      if(geschwAktuell < 0)   // Wenn geschwindigkeit Kleiner 0 
      {
        geschwLok2 = (geschwLok2 * -1); // Entfernt das Minus der geschwindigkeit
        lok2Richtung = false;   // Setzt die richtung auf false (Rückwärts)
      }
      else    // Wenn geschwindigkeit größer gleich 0
      {
        lok2Richtung = true;    // Setzt die richtung auf true (Vorwärts)
      }
      lok2.lok = intArray[l2][0]; // Legt die Adresse der Lok fest
      lok2.speed = geschwLok2;
      lok2.richtung = lok2Richtung;
      SerialisierenUndSenden(lok2);  // Daten der zweiten Lok senden
      geschwLok2 = geschwAktuell; // Setzt die Geschwindigkeit der Lok erneut auf die Aktuelle geschwindigkeit um evtl minus werte erneut zu setzen
    } 
  #pragma endregion
}



#pragma region // Encoder (Menü Rad)

// Pins für den Encoder definieren
#define PIN_CLK  32  // Signal A
#define PIN_DT   33  // Signal B
#define PIN_SW   25  // Taster

// Zustandsvariablen
unsigned long lastTurnTime = 0; // Zeitstempel der letzten Drehung
const unsigned long timeout = 5000; // Zeitlimit für Inaktivität (in ms)


bool startwertSetzen = false;  // Flag zum Setzen des Startwerts
int auswahlHauptAnzeige = -1;
int alterEncoderWert = 0;
bool radInaktivEinzelAktualisierung = false;

void MenueRad() 
{
  unsigned long currentTime = millis();

  if (!imAuswahlMenue) 
  {
    auswahlHauptAnzeige = EncoderEinlesen(0, 5, auswahlHauptAnzeige);

    // Wenn der Encoder-Wert sich geändert hat
  if(auswahlHauptAnzeige != alterEncoderWert)
  {
    lastTurnTime = millis();
    alterEncoderWert = auswahlHauptAnzeige;
  }

    // Wenn der Encoder 5 Sekunden lang nicht gedreht wurde
    if ((auswahlHauptAnzeige != -1) && (currentTime - lastTurnTime > timeout)) 
    {
      auswahlHauptAnzeige = -1; // Setze auswahlHauptAnzeige auf 0
      startwertSetzen = false;
      radInaktivEinzelAktualisierung = true;
    }
  }
  else
  {
    // Logik für den Fall, dass im Auswahlmenü
  }

  // Wenn der Taster gedrückt wird, gehe ins Auswahlmenü
  if ((auswahlHauptAnzeige >= 0 && auswahlHauptAnzeige <= 5) && DebounceTaster(ROTARY_ENCODER_BUTTON_PIN, 10))
  {
    imAuswahlMenue = true;
  }
}
#pragma endregion
