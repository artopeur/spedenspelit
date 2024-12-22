#include "display.h"

//alustetaan pinnit muuttumattomiksi
const int reset = 12;
const int shiftClock = 11;
const int latchClock = 10; 
const int outEnable = 9;
const int serialInput = 8; 

void initializeDisplay(void)
{
// määritetään pinnit OUTPUT tilaan.
  pinMode(reset, OUTPUT);
  pinMode(shiftClock, OUTPUT);
  pinMode(latchClock, OUTPUT);
  pinMode(outEnable, OUTPUT);
  pinMode(serialInput, OUTPUT);

// määritetään reset arvoon 1 ja loput arvoon 0 alustavasti. 
  digitalWrite(reset, HIGH);  
  digitalWrite(shiftClock, LOW); 
  digitalWrite(latchClock, LOW); 
  digitalWrite(outEnable, LOW);  
  digitalWrite(serialInput, LOW); 
}


void writeByte(uint8_t bits,bool last)
{
  int bittitaulukko[10][8] = {
   //määritellään mitkä segmentit syttyy niin tulee oikea numero
  // (segmentit) [A][B][C][D][E][F][käytetään läpivientiin latchin kautta seuraavalle.]
  // Nolla arvot sytyttää, koska näyttöpaneeli on common anode.
    {0,0,0,0,0,0,1,1}, //0
    {1,0,0,1,1,1,1,1}, //1
    {0,0,1,0,0,1,0,1}, //2
    {0,0,0,0,1,1,0,1}, //3
    {1,0,0,1,1,0,0,1}, //4
    {0,1,0,0,1,0,0,1}, //5
    {0,1,0,0,0,0,0,1}, //6
    {0,0,0,1,1,1,1,1}, //7
    {0,0,0,0,0,0,0,1}, //8
    {0,0,0,0,1,0,0,1} //9
    };

    // kirjoitetaan viimeisestä bitistä ensimmäiseen bittiin ja lähetetään eteenpäin.
    for (int i = 7; i >=0; i--)
    {
    digitalWrite(serialInput, bittitaulukko[bits][i]);
    digitalWrite(shiftClock, HIGH);
    digitalWrite(shiftClock, LOW);
    }

    // jos last (true), niin latchclockpin käytetään ylhäällä ja alhaalla. Näin bitit siirretään
    // toiselle näytölle.
    if (last){
    digitalWrite(latchClock, HIGH);
    digitalWrite(latchClock, LOW);
    }
    }



void writeHighAndLowNumber(uint8_t tens,uint8_t ones)
{
  writeByte(tens, false); // koska false menee kymmennumeronäytölle
  writeByte(ones, true);  // koska true, menee yksikkönumeronäytölle
}



// kutsutaan esim. showResults(9) pääohjelmasta, jolloin kymmenet on 0 ja yhdet on 9
// tulostetaan 10 näytölle 0 ja ykkös näytölle 9
void showResult(byte number)
{
  int tens = number / 10;  // jako lukema (11/10) = 1 kokonaista ja 1 jää yli
  int ones = number % 10;  // jakojäännös (9 / 10) = 0 kokonaista 9 jää yli
  writeHighAndLowNumber(tens, ones);
}


void kirjoitademobitti(uint8_t bitit,bool v) {
  int demobittitaulukko[12][8] = {
  {1,0,1,1,1,1,1,1},
  {1,1,0,1,1,1,1,1},
  {1,1,1,0,1,1,1,1},
  {1,1,1,1,0,1,1,1},
  {1,1,1,1,1,0,1,1},
  {0,1,1,1,1,1,1,1},
  {1,0,1,1,1,1,1,1},
  {1,1,0,1,1,1,1,1},
  {1,1,1,0,1,1,1,1},
  {1,1,1,1,0,1,1,1},
  {1,1,1,1,1,0,1,1},
  {0,1,1,1,1,1,1,1}
  //{1,1,1,1,1,1,0,1},
  //{0,1,1,1,1,1,1,1}
  };
  
  // kirjoitetaan viimeisestä bitistä ensimmäiseen bittiin ja lähetetään eteenpäin.
    for (int i = 7; i >=0; i--)
    {
    digitalWrite(serialInput, demobittitaulukko[bitit][i]);
    digitalWrite(shiftClock, HIGH);
    digitalWrite(shiftClock, LOW);
    }

    // jos last (true), niin latchclockpin käytetään ylhäällä ja alhaalla. Näin bitit siirretään
    // toiselle näytölle.
    if (v){
    digitalWrite(latchClock, HIGH);
    digitalWrite(latchClock, LOW);
    }
    }
void kirjoitademo(uint8_t demotulos)
{
  kirjoitademobitti(demotulos, false);
  kirjoitademobitti(demotulos, true);
}
void naytademo(byte demotulos)
{
  kirjoitademo(demotulos);
}
