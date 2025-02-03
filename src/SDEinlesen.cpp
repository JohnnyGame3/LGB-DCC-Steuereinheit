#include "SDEinlesen.h"

// Methode um die Länge eines 1D-Arrays Herauszufinden
int Array1DLaenge(const char* Array[]) 
{
  int anzahlElemente = 0;
  while (Array[anzahlElemente] != nullptr) 
  {
    ++anzahlElemente;
  }
  anzahlElemente = anzahlElemente -1;
  return anzahlElemente;
}




const char* lokCharArray[][10] = 
{
  {"/", "/", "/", "/", "/", "/", "/", "/", "/", "/"},
  {"Mallet", "Licht", "Sound", "Steckdose", "Dampf", "/", "/", "/", "/", "/"},
  {"Diesellok V51", "Licht", "Sound", "Steckdose", "Dampf", "/", "/", "/", "/", "/"},
  {"Kokodil", "Licht", "Sound", "Steckdose", "Dampf", "/", "/", "/", "/", "/"},
  {"Diesellok D10", "Licht", "Rundum-Licht", "Entkuppler H", "Entkuppler V", "/", "/", "/", "/", "/"},
  {"Lok g2", "L 2 / F 1", "L 2 / F 2", "L 2 / Z", "/", "/", "/", "/", "/", "/"},
  {"/", "/", "/", "/", "/", "/", "/", "/", "/", "/"}
};

int intArray[][10] =
{
  {0,0,0,0,0,0,0,0,0,0},
  {4,0,1,2,3,0,0,0,0,0},
  {5,0,1,2,3,4,5,6,7,8},
  {6,0,1,2,3,4,5,6,7,8},
  {10,0,1,2,3,4,0,0,0,0},
  {3,0,1,2,3,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0}
};

const char* weicheCharArray[] =
{
  "Weiche 1","Weiche 2","Weiche 3","Signal 1","-------",
};

int weicheIntArray[] =
{
  129,130,131,132,133,
};