#include "Anzeige.h"

int letzteDisplayGeschw = 0;


int w1 = 1;      // Weiche/Signal Pos 1 (Oben Links)
int w2 = 2;      // Weiche/Signal Pos 2 (Oben Rechts)
int l1 = 1;      // Lok 1 (Mitte, Links/Mitte/Rechts)
int l2 = 2;      // Lok 2 (Unten, Links/Mitte/Rechts)
int l1z = 3;     // Lok 1 Zusatzfunktion (Mitte Rechts)
int l2z = 3;     // Lok 1 Zusatzfunktion (Unten Rechts)

bool w1s = false;
bool w2s = false;   
bool l1F1s = false;
bool l1F2s = false;
bool l1Zs = false;
bool l2F1s = false;
bool l2F2s = false;
bool l2Zs = false;

bool testAktualisierung = false;

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCK, TFT_RST);


void SetupDisplay() 
{
  SPI.begin();  // SPI-Bus neu starten
  tft.init(TFT_HOEHE, TFT_BREITE);
  SPI.beginTransaction(SPISettings(40000000, MSBFIRST, SPI_MODE0)); // 40 MHz
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
}



//Methode zur Berechnung eines zyklischen Indexes innerhalb eines bestimmten Bereichs.
int getWrappedIndex(int aktuellerIndex, int offset, int minIndex, int maxIndex) 
{
    // Berechne den neuen Index durch Addition des Offsets
    int newIndex = aktuellerIndex + offset;

    // Falls der neue Index den maximalen Wert überschreitet, setze ihn auf den minimalen Wert
    if (newIndex > maxIndex) newIndex = minIndex;

    // Falls der neue Index kleiner als der minimale Wert ist, setze ihn auf den maximalen Wert
    if (newIndex < minIndex) newIndex = maxIndex;

    // Gib den berechneten Index zurück
    return newIndex;
}



// Methode um das Auswahl-Menü Vor Zu Zeichnen
void MenueVorZeichnen(uint16_t trennLinienFarbe)
{
  tft.fillRect(0,0,TFT_BREITE,TFT_HOEHE/3,ST77XX_BLACK);
  tft.drawLine(0,TFT_HOEHE/3,TFT_BREITE,TFT_HOEHE/3,trennLinienFarbe);  // Trennlinie 1 (Horizontal)

  tft.fillRect(0,(TFT_HOEHE/3)+1,TFT_BREITE,(TFT_HOEHE/3)-1,ST77XX_BLACK);
  tft.drawLine(0,(TFT_HOEHE/3)*2,TFT_BREITE,(TFT_HOEHE/3)*2,trennLinienFarbe);  // Trennlinie 2 (Horizontal)
  
  tft.fillRect(0,(TFT_HOEHE/3)*2+1,TFT_BREITE,(TFT_HOEHE/3)-1,ST77XX_BLACK);
}


// Methode zur Anzeige eines Textes an einer bestimmten Position mit variabler Farbe.
void DruckeTextMitFarbe(int x, int y, const char* text, bool bedingung) 
{
  // Setze den Cursor an die gewünschte Position
  tft.setCursor(x, y);

  // Wähle die Textfarbe basierend auf der Bedingung
  // Falls "bedingung" true ist → ROT, sonst → WEISS
  if(bedingung) {tft.setTextColor(ST77XX_RED);}

  // Drucke den Text auf das Display
  tft.print(text);

  // Setze die Textfarbe zurück auf Weiß (Standard)
  tft.setTextColor(ST77XX_WHITE);
}



// Methode zur Anzeige eines Status an einer bestimmten Position auf dem Display.
void StatusAnzeigen(int x, int y, const char* text1, const char* text2, bool status)
{
  // Setze den Cursor an die gewünschte Position
  tft.setCursor(x, y);

  // Lösche vorherigen Text, indem beide möglichen Werte in schwarzer Farbe gezeichnet werden
  tft.setTextColor(ST77XX_BLACK);
  tft.print(text1);
  tft.setCursor(x, y);
  tft.print(text2);

  // Setze den Cursor erneut für die finale Anzeige
  // Falls "bedingung" true ist → ROT, sonst → WEISS
  tft.setTextColor(status ? ST77XX_GREEN : ST77XX_RED);
  tft.setCursor(x, y);

  // Zeige den aktuellen Status an
  tft.print(status ? text1 : text2);
  tft.setTextColor(ST77XX_WHITE);
}


void ErsteLokZeile()
{ 
  if(geschwLok1Aktiv){tft.setTextColor(ST77XX_GREEN);}  // Andert die Textfarbe auf Grün wenn die lok angewählt ist
  DruckeTextMitFarbe(0, 120, lokCharArray[l1][0], auswahlHauptAnzeige == 4); 

  DruckeTextMitFarbe((80*3)+3, 120, lokCharArray[l1][l1z], auswahlHauptAnzeige == 1); 
}

void ZweiteLokZeile()
{
  if(geschwLok2Aktiv){tft.setTextColor(ST77XX_GREEN);}  // Andert die Textfarbe auf Grün wenn die lok angewählt ist
  DruckeTextMitFarbe(0, 172, lokCharArray[l2][0], auswahlHauptAnzeige == 3); 

  DruckeTextMitFarbe((80*3)+3, 172, lokCharArray[l2][l2z], auswahlHauptAnzeige == 0); 
}

void GeschwindigkeitAnzeigen()
{
  if(geschwAktuell != letzteDisplayGeschw)
  {
    tft.fillRect(180, 42, 51, 21, ST77XX_BLACK); // Löscht ein 40x25 Pixel großes Rechteck ab (185, 25)
    letzteDisplayGeschw = geschwAktuell;
  }
  tft.setCursor(180,42);
  tft.setTextSize(3);
  tft.print(letzteDisplayGeschw);
  tft.setTextSize(1);
}

void LokFunktionenAnzeigen(int x, int y, bool status1, bool status2, bool statusZ) 
{
  StatusAnzeigen(x, y, "Ein", "Aus", status1);
  StatusAnzeigen(x + 80, y, "Ein", "Aus", status2);
  StatusAnzeigen(x + 160, y, "Ein", "Aus", statusZ);
}

void StandardAnzeigeVorZeichnen()
{

  MenueVorZeichnen(ST77XX_CYAN);

  // Trennlinie 1 (Vertikal (Links))
  tft.drawLine(80,0,80,TFT_HOEHE,ST77XX_CYAN);

  // Trennlinie 2 (Vertikal (Rechts))
  tft.drawLine(240,0,240,TFT_HOEHE,ST77XX_CYAN);

  // Trennlinie 3 (Vertikal (Mitte))
  tft.drawLine(160,80,160,TFT_HOEHE,ST77XX_CYAN);


  // Text "Geschw.: " schreiben
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.setCursor(85, 46);
  tft.print("Geschw.:");


  // Überschriften schreiben
  tft.setCursor(0, 90);
  tft.print("Lok");

  tft.setCursor(80 + 3, 90);
  tft.print("Fav 1");

  tft.setCursor((80 * 2) + 3, 90);
  tft.print("Fav 2");

  tft.setCursor((80 * 3) + 3, 90);
  tft.print("Zusatz");
  tft.setTextSize(1);

  // Lok 1 Funktionen mittig schreiben
  tft.setCursor(80 + 3, 120);
  tft.println(lokCharArray[l1][1]);

  tft.setCursor((80 * 2) + 3, 120);
  tft.println(lokCharArray[l1][2]);


  // Lok 2 Funktionen mittig schreiben
  tft.setCursor(80 + 3, 172);
  tft.println(lokCharArray[l2][1]);

  tft.setCursor((80 * 2) + 3, 172);
  tft.println(lokCharArray[l2][2]);
}

void StandardAnzeige()
{ 
  bool standardAnzeigeErsterZyklus = true;

  StandardAnzeigeVorZeichnen();

  while (imAuswahlMenue == false)
  {

    MenueRad();

    if(tasterLokAktivGedrueckt || standardAnzeigeErsterZyklus || auswahlHauptAnzeige != -1 || radInaktivEinzelAktualisierung)
    {
      if(standardAnzeigeErsterZyklus || auswahlHauptAnzeige != -1 || radInaktivEinzelAktualisierung)
      {
        DruckeTextMitFarbe(15, 40, weicheCharArray[w1], auswahlHauptAnzeige == 5); 

        DruckeTextMitFarbe((80*3)+18, 40, weicheCharArray[w2], auswahlHauptAnzeige == 2); 

        radInaktivEinzelAktualisierung = false;
      }

      //Zeile 2 Lok 1
      ErsteLokZeile();

      //Zeile 3 Lok 2
      ZweiteLokZeile();
      tasterLokAktivGedrueckt = false;
    }

    GeschwindigkeitAnzeigen();


    if(tasterBetaetigt || standardAnzeigeErsterZyklus)
    {
      //tft.setTextSize(2);
      // Zustand Weiche/Signal/... 1
      if(strncmp(weicheCharArray[w1], "Signal",6) == 0 && strlen(weicheCharArray[w1]) >= 6) // Wenn Weiche 1 ein Signal ist
      {
        StatusAnzeigen(29, 60, "Ein", "Aus", w1s);
      }
      else
      {
        StatusAnzeigen(30, 60, "/", "|", w1s);
      }

      
      // Zustand Weiche/Signal/... 2
      if (strncmp(weicheCharArray[w2], "Signal", 6) == 0 && strlen(weicheCharArray[w2]) >= 6) 
      {
        StatusAnzeigen(269, 60, "Ein", "Aus", w2s);
      }
      else
      {
        StatusAnzeigen(270, 60, "/", "|", w2s);
      }

      // LOK 1 Funktionen
      LokFunktionenAnzeigen(80 + 3, 140, l1F1s, l1F2s, l1Zs);

      // LOK 2 Funktionen
      LokFunktionenAnzeigen(80 + 3, 190, l2F1s, l2F2s, l2Zs);

      //tft.setTextSize(1);
      
      tasterBetaetigt = false;
    }

    standardAnzeigeErsterZyklus = false;
  }
}


int WeicheAendern(int arrayNr)
{
  int temp = arrayNr;
  int LetzterTemp = temp;
  startwertSetzen = false;

  MenueVorZeichnen(ST77XX_RED);

  int arrayLaenge = Array1DLaenge(weicheCharArray);
  
  while (imAuswahlMenue)
  {
    if(temp != LetzterTemp)
    {
      
      tft.fillRect(0, 47, 158, 17, ST77XX_BLACK);
      tft.fillRect(0, 110, 245, 24, ST77XX_BLACK);
      tft.fillRect(0, 174, 158, 17, ST77XX_BLACK);
      
      LetzterTemp = temp;
    }


    tft.setCursor(3,48);
    tft.setTextSize(2);
    tft.println(weicheCharArray[getWrappedIndex(temp,-1,0,arrayLaenge)]);

    tft.setCursor(3,110);
    tft.setTextSize(3);
    tft.println(weicheCharArray[temp]);
    tft.setTextSize(2);

    tft.setCursor(3,175);
    tft.println(weicheCharArray[getWrappedIndex(temp,1,0,arrayLaenge)]);
    tft.setTextSize(1);

    temp = EncoderEinlesen(0,arrayLaenge,temp);

    if(DebounceTaster(ROTARY_ENCODER_BUTTON_PIN, 10)) // MultiRad
    {
      imAuswahlMenue = false;

      tft.setTextColor(ST77XX_GREEN);
      tft.setCursor(3,110);
      tft.setTextSize(3);
      tft.println(weicheCharArray[temp]);
    }
  }
  return temp;

  
}


int LokAendern(int arrayNr)
{
  int temp = arrayNr;
  int LetzterTemp = temp;
  startwertSetzen = false;

  MenueVorZeichnen(ST77XX_RED);

  while(imAuswahlMenue)  // Evtl. Aktualisierungs    zeit imAuswahlMenue
  {
    if(temp != LetzterTemp)
    {
      tft.fillRect(0, 47, 158, 17, ST77XX_BLACK);
      tft.fillRect(0, 110, 245, 24, ST77XX_BLACK);
      tft.fillRect(0, 174, 158, 17, ST77XX_BLACK);
 
      LetzterTemp = temp;
    }


    tft.setCursor(3,48);
    tft.setTextSize(2);
    tft.println(lokCharArray[getWrappedIndex(temp,-1,0,lokZeilenAnzahl)][0]);

    tft.setCursor(3,110);
    tft.setTextSize(3);
    tft.println(lokCharArray[temp][0]);
    tft.setTextSize(2);

    tft.setCursor(3,175);
    tft.println(lokCharArray[getWrappedIndex(temp,1,0,lokZeilenAnzahl)][0]);
    tft.setTextSize(1);

    temp = EncoderEinlesen(0,lokZeilenAnzahl,temp);

    if(DebounceTaster(ROTARY_ENCODER_BUTTON_PIN, 10)) // MultiRad
    {
      imAuswahlMenue = false;

      tft.setTextColor(ST77XX_GREEN);
      tft.setCursor(3,110);
      tft.setTextSize(3);
      tft.println(lokCharArray[temp][0]);
    }
  }
  return temp;
}


int ZusatzAendern(int lokNr, int arrayNr)
{
  int temp = arrayNr;
  int LetzterTemp = temp;
  startwertSetzen = false;

  MenueVorZeichnen(ST77XX_RED);

  while(imAuswahlMenue)  // Evtl. Aktualisierungs    zeit imAuswahlMenue
  {
    if(temp != LetzterTemp)
    {
      tft.fillRect(0, 47, 158, 17, ST77XX_BLACK);
      tft.fillRect(0, 110, 245, 24, ST77XX_BLACK);
      tft.fillRect(0, 174, 158, 17, ST77XX_BLACK);
      LetzterTemp = temp;
    }

    // Curser und Feld Hintergrund Schwartz
    tft.setCursor(3,48);
    tft.setTextSize(2);
    tft.println(lokCharArray[lokNr][getWrappedIndex(temp,-1,3,(MAX_SPALTEN_LOK/2)-3)]);

    tft.setCursor(3,110);
    tft.setTextSize(3);
    tft.println(lokCharArray[lokNr][temp]);
    tft.setTextSize(2);

    tft.setCursor(3,175);
    tft.println(lokCharArray[lokNr][getWrappedIndex(temp,1,3,(MAX_SPALTEN_LOK/2)-3)]);
    tft.setTextSize(1);

   
    temp = EncoderEinlesen(3,(MAX_SPALTEN_LOK/2)-3,temp);

    if(DebounceTaster(ROTARY_ENCODER_BUTTON_PIN, 10)) // MultiRad
    {
      imAuswahlMenue = false;

      tft.setTextColor(ST77XX_GREEN);
      tft.setCursor(3,110);
      tft.setTextSize(3);
      tft.println(lokCharArray[lokNr][temp]);
    }
  }
  return temp;
}

// Methode um Destzustellen ob man im Auswahlmenü ist oder nicht 
void DisplayAnzeignAuswahl()
{
  if((auswahlHauptAnzeige != -1) && (imAuswahlMenue == true)) // Hier muss noch etwas für den Encoder Taster rein
  {
    switch (auswahlHauptAnzeige)
    {
      case 5:
        w1 = WeicheAendern(w1);
        break;
      case 4:
        l1F1s = false;
        l1F2s = false;
        l1Zs = false;
        l1 = LokAendern(l1);
        break;
      case 3:
        l2F1s = false;
        l2F2s = false;
        l2Zs = false;
        l2 = LokAendern(l2);;
        break;
      case 2:     
        w2 = WeicheAendern(w2);
        break;
      case 1:
        l1Zs = false;
        l1z = ZusatzAendern(l1,l1z);
        break;
      case 0:
        l2Zs = false;
        l2z = ZusatzAendern(l2,l2z);
        break;
      default:
        break;
   }
  }
  else
  {
    StandardAnzeige();
  }
}