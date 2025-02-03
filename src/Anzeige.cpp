#include <Arduino.h>
#include "Anzeige.h"

int letzteDisplayGeschw = 0;


int w1 = 0;      // Weiche/Signal Pos 1 (Oben Links)
int w2 = 1;      // Weiche/Signal Pos 2 (Oben Rechts)
int l1 = 4;      // Lok 1 (Mitte, Links/Mitte/Rechts)
int l2 = 1;      // Lok 2 (Unten, Links/Mitte/Rechts)
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

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCK);

void displaySetup() 
{
  Wire.setPins(10, 8); // redefine first I2C port to be on pins 10/8
  SPI.beginTransaction(SPISettings(40000000, MSBFIRST, SPI_MODE0)); // 40 MHz

  tft.init(TFT_HOEHE, TFT_BREITE);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
}




// Methode zur Berechnung des zyklischen Index
int getWrappedIndex(int aktuellerIndex, int offset, int minIndex, int maxIndex) 
{
  int newIndex = aktuellerIndex + offset;
  if (newIndex > maxIndex) newIndex = minIndex;
  if (newIndex < minIndex) newIndex = maxIndex;
  return newIndex;
}


void GitterZeichnen()
{
    // Trennlinie 1 (Horizontal)
  tft.drawLine(0,80,320,80,ST77XX_CYAN);  

    // Trennlinie 2 (Horizontal)
  tft.drawLine(0,160,320,160,ST77XX_CYAN);

    // Trennlinie 1 (Vertikal (Links))
  tft.drawLine(80,0,80,240,ST77XX_CYAN);

    // Trennlinie 2 (Vertikal (Rechts))
  tft.drawLine(240,0,240,240,ST77XX_CYAN);

    // Trennlinie 3 (Vertikal (Mitte))
  tft.drawLine(160,80,160,240,ST77XX_CYAN);
}

void ErsteLokZeile()
{  
  tft.setCursor(0,120);
  if(geschwLok1Aktiv){tft.setTextColor(ST77XX_GREEN);}  // Andert die Textfarbe auf Grün wenn die lok angewählt ist
  if(auswahlHauptAnzeige == 2) {tft.setTextColor(ST77XX_RED);} // Andert die Anzeige auf Rot Wenn ich mit dem "Cörser" hier drauf bin (Pos 2 = Mitte,Links) 
  tft.print(lokCharArray[l1][0]);
  tft.setTextColor(ST77XX_WHITE);   // ändert die Anzeige Zurück auf Weiß


  tft.setCursor((80*3)+3,120);
  if(auswahlHauptAnzeige == 5) {tft.setTextColor(ST77XX_RED);} // Andert die Anzeige auf Rot Wenn ich mit dem "Cörser" hier drauf bin (Pos 3 = Mitte,Rechts)
  tft.println(lokCharArray[l1][l1z]);
  tft.setTextColor(ST77XX_WHITE);   // ändert die Anzeige Zurück auf Weiß
}

void ZweiteLokZeile()
{
  tft.setCursor(0,172);
  if(geschwLok2Aktiv){tft.setTextColor(ST77XX_GREEN);}  // Andert die Textfarbe auf Grün wenn die lok angewählt ist
  if(auswahlHauptAnzeige == 3) {tft.setTextColor(ST77XX_RED);} // Andert die Anzeige auf Rot Wenn ich mit dem "Cörser" hier drauf bin (Pos 3 = Unten,Links)
  tft.print(lokCharArray[l2][0]);
  tft.setTextColor(ST77XX_WHITE);   // ändert die Anzeige Zurück auf Weiß


  tft.setCursor((80*3)+3,172);
  if(auswahlHauptAnzeige == 6) {tft.setTextColor(ST77XX_RED);} // Andert die Anzeige auf Rot Wenn ich mit dem "Cörser" hier drauf bin (Pos 6 = Unten,Rechts)
  tft.println(lokCharArray[l2][l2z]);
  tft.setTextColor(ST77XX_WHITE);   // ändert die Anzeige Zurück auf Weiß
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

void StandardAnzeige()
{  
  tft.fillScreen(ST77XX_BLACK);
  
  tft.setRotation(1);
  tft.setTextColor(ST77XX_WHITE); // ändert den Anzeige Text auf Weiß

  GitterZeichnen();

    // __________________Überschriften__________________________
  tft.setCursor(0,90);
  tft.setTextSize(2);
  tft.print("Lok");

  tft.setCursor(80+3,90);
  tft.print("Fav 1");
 
  tft.setCursor((80*2)+3,90);
  tft.print("Fav 2");

  tft.setCursor((80*3)+3,90);
  tft.print("Zusatz");

  // ____________Geschwindigkeit_______________________________
  tft.setCursor(85,46);
  tft.print("Geschw.:");
  tft.setTextSize(1);

  // _____________Weichen Geschalten____________________________
  tft.setCursor(30,60);
  tft.print(w1s ? "|":"/"); // Zustand Weiche/Signal/... 1

  tft.setCursor(270,60);
  tft.print(w2s ? "|":"/"); // Zustand Weiche/Signal/... 2

 
  // ____________LOK 1 Funktionen______________________________
  tft.setCursor((80)+3,120);
  tft.println(lokCharArray[l1][1]);

  tft.setCursor((80)+3,140);
  tft.print(l1F1s ? "Ein":"Aus");


  tft.setCursor((80*2)+3,120);
  tft.println(lokCharArray[l1][2]);

  tft.setCursor((80*2)+3,140);
  tft.print(l1F2s ? "Ein":"Aus");


  tft.setCursor((80*3)+3,140);
  tft.print(l1Zs ? "Ein":"Aus"); 

  // ______________LOK 2 Funktionen______________________
  tft.setCursor((80)+3,172);
  tft.println(lokCharArray[l2][1]);

  tft.setCursor((80)+3,190);
  tft.print(l2F1s ? "Ein":"Aus");


  tft.setCursor((80*2)+3,172);
  tft.println(lokCharArray[l2][2]);

  tft.setCursor((80*2)+3,190);
  tft.print(l2F2s ? "Ein":"Aus");


  tft.setCursor((80*3)+3,190);
  tft.print(l2Zs ? "Ein":"Aus");
  


  while (imAuswahlMenue == false)
  {

    MenueRad();

    // Zeile 1 Weiche 1| Geschwindigkeit | Weiche 2
    tft.setCursor(15,40);
    if(auswahlHauptAnzeige == 1) 
    {tft.setTextColor(ST77XX_RED);}  // Andert die Anzeige auf Rot Wenn ich mit dem "Cörser" hier drauf bin (Pos 1 = Oben,Links)
    tft.print(weicheCharArray[w1]);     // Name Weiche/Signal/... 1
    tft.setTextColor(ST77XX_WHITE);   // ändert die Anzeige Zurück auf Weiß

    GeschwindigkeitAnzeigen();

    tft.setCursor((80*3)+18,40);
    if(auswahlHauptAnzeige == 4)
    {tft.setTextColor(ST77XX_RED);} // Andert die Anzeige auf Rot Wenn ich mit dem "Cörser" hier drauf bin (Pos 4 = Oben,Rechts)
    tft.print(weicheCharArray[w2]);     // Name Weiche/Signal/... 2
    tft.setTextColor(ST77XX_WHITE);   // ändert die Anzeige Zurück auf Weiß

    //Zeile 2 Lok 1
    ErsteLokZeile();

    //Zeile 3 Lok 2
    ZweiteLokZeile();


    if(tasterBetaetigt == true)
    {
      // ______________Weichen Aktualisieren_______________________
      //tft.fillRect(0, 56, 78, 16, ST77XX_BLACK); // Löscht ein 40x25 Pixel großes Rechteck ab (185, 25)
      tft.setCursor(30,60);

      tft.setTextColor(ST77XX_BLACK);
      tft.print("|");
      tft.setCursor(30,60);
      tft.print("/");
      tft.setTextColor(ST77XX_WHITE); 
      tft.setCursor(30,60);

      tft.print(w1s ? "|":"/"); // Zustand Weiche/Signal/... 1


      //tft.fillRect((80*3)+1, 56, 78, 16, ST77XX_BLACK); // Löscht ein 40x25 Pixel großes Rechteck ab (185, 25)
      tft.setCursor(270,60);

      tft.setTextColor(ST77XX_BLACK);
      tft.print("|");
      tft.setCursor(270,60);
      tft.print("/");
      tft.setTextColor(ST77XX_WHITE); 
      tft.setCursor(270,60);

      tft.print(w2s ? "|":"/"); // Zustand Weiche/Signal/... 2


      // ____________LOK 1 Funktionen______________________________
      //tft.fillRect((80) +1, 136, 78, 16, ST77XX_BLACK); // Löscht ein 40x25 Pixel großes Rechteck ab (185, 25)
      tft.setCursor((80)+3,140);

      tft.setTextColor(ST77XX_BLACK);
      tft.print("Ein");
            tft.setCursor((80)+3,140);
      tft.print("Aus");
      tft.setTextColor(ST77XX_WHITE);      
      tft.setCursor((80)+3,140);

      tft.print(l1F1s ? "Ein":"Aus");

      //tft.fillRect((80*2) +1, 136, 78, 16, ST77XX_BLACK); // Löscht ein 40x25 Pixel großes Rechteck ab (185, 25)
      tft.setCursor((80*2)+3,140);
      
      tft.setTextColor(ST77XX_BLACK);
      tft.print("Ein");
            tft.setCursor((80*2)+3,140);
      tft.print("Aus");
      tft.setTextColor(ST77XX_WHITE);
            tft.setCursor((80*2)+3,140);

      tft.print(l1F2s ? "Ein":"Aus");

      //tft.fillRect((80*3) +1, 136, 78, 16, ST77XX_BLACK); // Löscht ein 40x25 Pixel großes Rechteck ab (185, 25)
      tft.setCursor((80*3)+3,140);

      tft.setTextColor(ST77XX_BLACK);
      tft.print("Ein");
            tft.setCursor((80*3)+3,140);
      tft.print("Aus");
      tft.setTextColor(ST77XX_WHITE);
            tft.setCursor((80*3)+3,140);

      tft.print(l1Zs ? "Ein":"Aus");


      // ______________LOK 2 Funktionen______________________
      //tft.fillRect((80) +1, 186, 78, 16, ST77XX_BLACK); // Löscht ein 40x25 Pixel großes Rechteck ab (185, 25)
      tft.setCursor((80)+3,190);

      tft.setTextColor(ST77XX_BLACK);
      tft.print("Ein");
            tft.setCursor((80)+3,190);
      tft.print("Aus");
      tft.setTextColor(ST77XX_WHITE);
            tft.setCursor((80)+3,190);

      tft.print(l2F1s ? "Ein":"Aus");

      //tft.fillRect((80*2) +1, 186, 78, 16, ST77XX_BLACK); // Löscht ein 40x25 Pixel großes Rechteck ab (185, 25)
      tft.setCursor((80*2)+3,190);

      tft.setTextColor(ST77XX_BLACK);
      tft.print("Ein");
            tft.setCursor((80*2)+3,190);
      tft.print("Aus");
      tft.setTextColor(ST77XX_WHITE);
            tft.setCursor((80*2)+3,190);      

      tft.print(l2F2s ? "Ein":"Aus");

      //tft.fillRect((80*3) +1, 186, 78, 16, ST77XX_BLACK); // Löscht ein 40x25 Pixel großes Rechteck ab (185, 25)
      tft.setCursor((80*3)+3,190);

      tft.setTextColor(ST77XX_BLACK);
      tft.print("Ein");
            tft.setCursor((80*3)+3,190);
      tft.print("Aus");
      tft.setTextColor(ST77XX_WHITE);
            tft.setCursor((80*3)+3,190);

      tft.print(l2Zs ? "Ein":"Aus");

      tasterBetaetigt = false;
    }

  }
}


int WeicheAendern(int arrayNr)
{
  int temp = arrayNr;
  int LetzterTemp = temp;
  startwertSetzen = false;

  tft.fillScreen(ST77XX_BLACK);
  // Trennlinie 1 (Horizontal)
  tft.drawLine(0,80,320,80,ST77XX_RED);  

  // Trennlinie 2 (Horizontal)
  tft.drawLine(0,160,320,160,ST77XX_RED);

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
    tft.println(weicheCharArray[getWrappedIndex(temp,1,0,arrayLaenge)]);

    tft.setCursor(3,110);
    tft.setTextSize(3);
    tft.println(weicheCharArray[temp]);
    tft.setTextSize(2);

    tft.setCursor(3,175);
    tft.println(weicheCharArray[getWrappedIndex(temp,-1,0,arrayLaenge)]);
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
  tft.fillScreen(ST77XX_BLACK);
  startwertSetzen = false;

  while(imAuswahlMenue)  // Evtl. Aktualisierungs    zeit imAuswahlMenue
  {
    if(temp != LetzterTemp)
    {
      tft.fillRect(0, 47, 158, 17, ST77XX_BLACK);
      tft.fillRect(0, 110, 245, 24, ST77XX_BLACK);
      tft.fillRect(0, 174, 158, 17, ST77XX_BLACK);
 
      LetzterTemp = temp;
    }

    // Trennlinie 1 (Horizontal)
    tft.drawLine(0,80,320,80,ST77XX_RED);  

    // Trennlinie 2 (Horizontal)
    tft.drawLine(0,160,320,160,ST77XX_RED);

    tft.setCursor(3,48);
    tft.setTextSize(2);
    tft.println(lokCharArray[getWrappedIndex(temp,1,0,6)][0]);

    tft.setCursor(3,110);
    tft.setTextSize(3);
    tft.println(lokCharArray[temp][0]);
    tft.setTextSize(2);

    tft.setCursor(3,175);
    tft.println(lokCharArray[getWrappedIndex(temp,-1,0,6)][0]);
    tft.setTextSize(1);

    temp = EncoderEinlesen(0,6,temp);

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
  tft.fillScreen(ST77XX_BLACK);
  startwertSetzen = false;

  while(imAuswahlMenue)  // Evtl. Aktualisierungs    zeit imAuswahlMenue
  {
    if(temp != LetzterTemp)
    {
      tft.fillRect(0, 47, 158, 17, ST77XX_BLACK);
      tft.fillRect(0, 110, 245, 24, ST77XX_BLACK);
      tft.fillRect(0, 174, 158, 17, ST77XX_BLACK);
      LetzterTemp = temp;
    }
    // Trennlinie 1 (Horizontal)
    tft.drawLine(0,80,320,80,ST77XX_RED);  

    // Trennlinie 2 (Horizontal)
    tft.drawLine(0,160,320,160,ST77XX_RED);

    // Curser und Feld Hintergrund Schwartz
    tft.setCursor(3,48);
    tft.setTextSize(2);
    tft.println(lokCharArray[lokNr][getWrappedIndex(temp,1,3,7)]);

    tft.setCursor(3,110);
    tft.setTextSize(3);
    tft.println(lokCharArray[lokNr][temp]);
    tft.setTextSize(2);

    tft.setCursor(3,175);
    tft.println(lokCharArray[lokNr][getWrappedIndex(temp,-1,3,7)]);
    tft.setTextSize(1);

   
    temp = EncoderEinlesen(3,7,temp);

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
  if((auswahlHauptAnzeige != 0) && (imAuswahlMenue == true)) // Hier muss noch etwas für den Encoder Taster rein
  {
    switch (auswahlHauptAnzeige)
    {
      case 1:
        w1 = WeicheAendern(w1);
        break;
      case 2:
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
      case 4:     
        w2 = WeicheAendern(w2);
        break;
      case 5:
        l1Zs = false;
        l1z = ZusatzAendern(l1,l1z);
        break;
      case 6:
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