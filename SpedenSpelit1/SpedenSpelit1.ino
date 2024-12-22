#include "display.h"
#include "buttons.h"
#include "leds.h"
#include "SpedenSpelit1.h"

//EEPROM.update(address, highScore);
int arvotutLedit[100]; //taulukko arviotuista numeroista pelin aikana
int painetutNapit[100]; // taulukko painetuista napeista pelin aikana
bool peliKaynnissa = false; //pelin tila - muuttaa miten esim. napit käyttäytyvät
volatile int highScore = 0;  //säilyttää parhaan tuloksen vaikka pelattaisiin useampi peli peräkkäin.
volatile int pisteet = 0; //pelin aikana saadut pisteet
volatile int nopeusMuuttuja = 0; //erillinen muuttuja joka nollataan kun päästään 10 pisteeseen. Tätä käytetään nopeutta säätäessä.
bool idleTimer = false;
void setup()
{
  Serial.begin(9600);
  initButtonsAndButtonInterrupts(); //alustaa napit
  initializeDisplay(); //alustaa näytöt
  initializeLeds(); // alustaa pinnit oikeisiin ledeihin
  initializeTimer(); //alustaa nopeuden
  randomSeed(analogRead(0)); //
  /*
    Initialize here all modules
  */
}

void loop()
{
  //buttonPressLooper(); //buttons koodista tuotu funktio napeille
Serial.println(buttonPressed);
  OCR1A = 16365;
  TIMSK1 = (1 << OCIE1A );
if(idleTimer == true){   //mitä tapahtuu kun peli ei ole käynnissä
  //IDLE

  idleDemo();
} // if
if(peliKaynnissa == true){ //peli toimii vain bool muuttujan ollessa true arvossa
  //checkGame();
  
  showResult(pisteet);

}
}
  /*
    This loop function constantly checks if
	player has pushed a button and there is
	need for checking the game status
  */
//}

ISR(TIMER1_COMPA_vect)
{
  int ledArvot[] = {8, 4, 2, 1}; //taulukko jossa on annettu ledien numerot led koodista
  int edellinenArvottuLed = 0;
  int arvottuLed;

do {
  arvottuLed = ledArvot[random(4)];
} while (arvottuLed == edellinenArvottuLed || arvottuLed == arvotutLedit[pisteet]);

if (arvotutLedit[pisteet] == 0){
 arvotutLedit[pisteet] = arvottuLed;
 edellinenArvottuLed = arvottuLed;
  if(nopeusMuuttuja == 10){
  OCR1A *= 0.9;
  nopeusMuuttuja = 0;
  }
  
}
 /*
    Here you generate a random number that is used to
	set certain led.
	
	Each timer interrupt increments "played" numbers,
	and after each 10 numbers you must increment interrupt
	frequency.
	
	Each generated random number must be stored for later
	comparision against player push button presses.
  */
  idleTimer=true;
}



void initializeTimer(void)
{
  TCCR1A = 0;
  TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10); //prescale arvoksi 1024
  OCR1A = 15624; //laskettu nopeudeksi (16hz/(1024*1hz))-1 = 1hz
  TIMSK1 = (1 << OCIE1A );


	// see requirements for the function from SpedenSpelit.h
}

void checkGame(int buttonIndex)
{
  painetutNapit[pisteet] = buttonIndex; //buttonIndexi tuodaan pointterilla buttons koodista
  if(painetutNapit[pisteet] != 0){
    if(painetutNapit[pisteet] == arvotutLedit[pisteet]){
      pisteet++;
      nopeusMuuttuja++;
    }
}
   else{
    stopTheGame();
   }
  }
	// see requirements for the function from SpedenSpelit.h


void initializeGame()
{
	// see requirements for the function from SpedenSpelit.h
   for (int i = 0; i < 100; ++i) { //määritetään kaikki alkiot 0 arvoiksi
    arvotutLedit[i] = 0;
    painetutNapit[i] = 0;
  }

  pisteet = 0; //nollataan pisteet molemmista muuttujista
  nopeusMuuttuja = 0;
}

void startTheGame()
{
if(peliKaynnissa == true){
initializeGame();
clearAllLeds();
showResult(3);
delay(1000);
showResult(2);
delay(1000);
showResult(1);
delay(1000);
setLed(arvotutLedit[pisteet]);


}
  
   // see requirements for the function from SpedenSpelit.h
}

void stopTheGame()
{
  peliKaynnissa = false;

  if(pisteet > highScore){
    highScore = pisteet;
  }
clearAllLeds(); //tyhjentää kaikki ledit
delay(100);
setAllLeds();//sytyttää kaikki ledit
delay(100);
clearAllLeds(); //tyhjentää kaikki ledit
delay(100);
setAllLeds();//sytyttää kaikki ledit
delay(100);
clearAllLeds(); //tyhjentää kaikki ledit
delay(100);
setAllLeds();//sytyttää kaikki ledit

  Serial.print("Peli loppui! Tuloksesi on: "); //nämä nyt ovat vain huvin vuoksi laitettu jos meillä olisi oikea näyttö.
  Serial.println(pisteet);
  Serial.print("Pelisi kesti: ");
  //Serial.println(aika);

}

void idleDemo() {
  static int i=0;
  static int demo2[] = {0x0, 0x1, 0x3, 0x7, 0xF, 0xF, 0x7, 0x3, 0x1};   // eka demo. Voidaan testata vähän toimiiko unossa kunnolla.
  static int demo1[] = {0xbf, 0xdf,0xef,0xf7,0xfb,0x7f};
  

  if(idleTimer == true) {
    showResult(demo1[i]);

    setLed(demo2[i]);   // sytyttää ledin taulukon mukaan.
    i++;
    buzzerTest(650);
        // tämä pitänee tehdä ajastetusti timer arvolla. Vaikkakaan tähän ei mennä silloin kun peli on päällä.
    idleTimer=false;

  } // if
  if (i> 10) {
    i=0;
  }
  
}
