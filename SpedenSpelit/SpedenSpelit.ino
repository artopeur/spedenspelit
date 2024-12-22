 #include "display.h"
#include "buttons.h"
#include "leds.h"
#include "SpedenSpelit.h"

/* ===============================================
	Käsittelyssä			Kenellä			Tilanne
	LOOP					Arto			valmis
	CheckGame				Timo			valmis
	Interrupt / random		Marko			valmis
 =============================================== */

//EEPROM.update(address, highScore);
int arvotutLedit[100]; //taulukko arviotuista numeroista pelin aikana
int painetutNapit[100]; // taulukko painetuista napeista pelin aikana
bool timerTicks = false;	// pelin pyöriessä käytössä oleva tiksu, joka kellottaa toiminnot.
volatile int highScore = 0;  //säilyttää parhaan tuloksen vaikka pelattaisiin useampi peli peräkkäin.
volatile int pisteet = 0; //pelin aikana saadut pisteet

bool idleTimer = false;


// alustus h tiedostossa..
bool peliKaynnissa = false; //pelin tila - muuttaa miten esim. napit käyttäytyvät
 

void setup()
{
	/*
		Initialize here all modules
	*/	
  pinMode(A0, INPUT);
  Serial.begin(9600);
  initButtonsAndButtonInterrupts(); //alustaa napit
  initializeDisplay(); //alustaa näytöt
  initializeLeds(); // alustaa pinnit oikeisiin ledeihin
  initializeTimer(); //alustaa nopeuden
  randomSeed(analogRead(0)); //
  
  

} // setup ends

	 /*
		This loop function constantly checks if
		player has pushed a button and there is
		need for checking the game status
	  */


void loop()
{
	// Testauskoodi, jotta pelin saa päälle vaikka ei ole nappeja.
	///*
  char a="";
  if(Serial.available()) {
    a=Serial.read();
  }
  if(a == 'a') {
    if(peliKaynnissa) {
      stopTheGame();
      //peliKaynnissa = false;
    }
    else {
      //peliKaynnissa = true;
	    //initializeGame();
      startTheGame();
    }
  }
  //*/
	
	///*	UUSI KOODI, jos lisää // ennen /* niin voi vaihtaa uuden ja vanhan koodin välillä. 
	// Testaus vielä vähän kesken.
	if(peliKaynnissa) {
    if(buttonPressed!=0) {
      Serial.println("checkGame runs");
      checkGame(arvotutLedit[pisteet]);
    }
    // pitää tehdä checkGame funktiossa, jos käytetään timeria.
    // timerTicks = false;
		
	} // if
  else if(peliKaynnissa == false) {
    OCR1A = 16365;
    TIMSK1 = (1 << OCIE1A );
    //Serial.println("idle should run");
    if(buttonPressed == 16){
      Serial.print("Button ");
      Serial.print(arvotutLedit[pisteet]);
      Serial.println(" was pressed.");
      //peliKaynnissa = true;
      //idleTimer = false;
      //initializeGame();
      startTheGame();
        // voi joutua muuttaan startTheGame();
      buttonPressed=0; // määritetään, jotta vain 1 napin painallus aloittaa pelin ja voidaan tehdä uudelleen.
      timerTicks = false;
    } // if
    if(idleTimer && peliKaynnissa == false) {
      Serial.println("Idle running");
      idleDemo();
      idleTimer=false;
    } //if 
  } // else ! peliKaynnissa
  else {
    Serial.print("Neither working.");
    delay(500);
  }
} // function loop ends
 

ISR(TIMER1_COMPA_vect)
{
  
 /*
    Here you generate a random number that is used to
	set certain led.
	
	Each timer interrupt increments "played" numbers,
	and after each 10 numbers you must increment interrupt
	frequency.
	
	Each generated random number must be stored for later
	comparision against player push button presses.
  */
  static int nopeusMuuttuja = 0; //erillinen muuttuja joka nollataan kun päästään 10 pisteeseen. Tätä käytetään nopeutta säätäessä.
  int ledArvot[] = {8, 4, 2, 1}; //taulukko jossa on annettu ledien numerot led koodista
  static int edellinenArvottuLed = 0;
  static int round=0;
  int arvottuLed;
  int randomSeed=0;

	
  if(peliKaynnissa) {
    timerTicks=true;
    
    if(round == 0) {
      //arvottuLed = ledArvot[random(4)];
      arvottuLed = random(4);
      arvotutLedit[round] = ledArvot[arvottuLed];
      round++;
    }
    else if(round <100){
      // do while loop arvontaan ja syöttöön.
      do {
        //arvottuLed = ledArvot[random(4)];
        arvottuLed = random(4);
        arvotutLedit[round] = ledArvot[arvottuLed];
        if(arvotutLedit[round] == arvotutLedit[round-1]) {
          continue;
        }
        ///*
        else {
          arvotutLedit[round] = ledArvot[arvottuLed]; // syyllinen
          
          break;
        }//*/
       
      } while(ledArvot[arvottuLed] == arvotutLedit[round-1]); // while, jotta arvottuja arvoja tulee aina välillä ainakin kerran
      setLed(arvotutLedit[round-1]);
      Serial.print("syyllinen: ");
          Serial.println(arvottuLed);
          Serial.print("arvotutLedit[round] = ");
          Serial.println(arvotutLedit[round]);
          
      if(round>=1) {
        round++;
      }
      
      //Serial.print("ledin arvo: ");
      //Serial.println(arvottuLed); 
      //Serial.print("napinarvo: ");
      //Serial.print(painetutNapit);
      buzzerTest(650);

    } // else if
    else {
      Serial.println("maxed out, waiting button presses");
    }
  } // pelikaynnissa
  else if(peliKaynnissa == false) {
      Serial.print("arvottuja arvoja oli ");
      Serial.print(round);
      round = 0;
      Serial.print(" ja round arvo nollattu, joten arvo nyt ");
      Serial.println(round);
      idleTimer=true;
  }
  else {
    if(buttonPressed == 16) {
      startTheGame();
    }
    else {
      Serial.print("waiting for button press");
    }
  }
  	

	if (round != 100) {
		nopeusMuuttuja++;
	  if(nopeusMuuttuja == 10) {
		  OCR1A *= 0.9;
		  nopeusMuuttuja = 0;
	  } // if
	} // if
} // Timer



void initializeTimer(void)
{
  TCCR1A = 0;
  TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10); //prescale arvoksi 1024
  OCR1A = 15624; //laskettu nopeudeksi (16hz/(1024*1hz))-1 = 1hz
  TIMSK1 = (1 << OCIE1A );


	// see requirements for the function from SpedenSpelit.h
}

void checkGame(byte buttonIndex)
{
		// see requirements for the function from SpedenSpelit.h
  
    Serial.print("GameCheck");
    // syötetään painonapin taulukkoon buttonindex = arvotutLedit[pisteet], joten arvo on aina oikea..
    if(peliKaynnissa) {
      if(painetutNapit[pisteet] != buttonPressed) {
      painetutNapit[pisteet] = buttonPressed;
      //buttonPressed=0;
      Serial.print("ledin arvo: ");
      Serial.println(arvotutLedit[pisteet]); 
      Serial.print("napinarvo: ");
      Serial.println(painetutNapit[pisteet]);
      Serial.print("pisteet: ");
      Serial.println(pisteet);
      }
      //painetutNapit[pisteet] = buttonIndex; //buttonIndexi tuodaan pointterilla buttons koodista
      // jos painetutNapit[pisteet] != 0 aiheuttaa sen, että mennään vain jos ollaan, muuten lopetetaan peli.
      if(painetutNapit[pisteet] != 0){
        Serial.print("buttonpressed: ");
          Serial.println(buttonPressed);
          Serial.print("painetutnapit: ");
          Serial.println(painetutNapit[pisteet]);
          showResult(pisteet);
        if(painetutNapit[pisteet] == arvotutLedit[pisteet]){
          pisteet++;
          buttonPressed=0;
          //nopeusMuuttuja++;
        }
        else if(painetutNapit[pisteet] != arvotutLedit[pisteet]) {
          stopTheGame();
          buttonPressed=0;
        }
        else {
          // aloita uus peli
          startTheGame();
        }
      }
  } // if
} // checkGame
	


void initializeGame()
{
	// see requirements for the function from SpedenSpelit.h
	
	// aloitetaan 16365 tiksulla timer alustus
	OCR1A = 15624;
    TIMSK1 = (1 << OCIE1A );
	
   for (int i = 0; i < 100; ++i) 
   { //määritetään kaikki alkiot 0 arvoiksi
    arvotutLedit[i] = 0;
    painetutNapit[i] = 0;
   }

  pisteet = 0; //nollataan pisteet molemmista muuttujista
  //nopeusMuuttuja = 0;
}

void startTheGame()
{
//initializeGame();
TIMSK1 = (0 << OCIE1A );
int steps=3;
	//int timerTicks = 0;
for(steps=3;steps>0;steps--) {
    Serial.println(steps); // näytetään 3,2,1 serial portin kautta
    showResult(steps);		// näytetään 3,2,1
    if(steps == 1) {
      initializeGame();
      clearAllLeds();
      if(peliKaynnissa==false) {
        peliKaynnissa=true;
        
      }
    } // if
    
  delay(1000);
  
}
  showResult(0);
  delay(500);// see requirements for the function from SpedenSpelit.h
}


void stopTheGame()
{
	if(peliKaynnissa==true) {
		peliKaynnissa = false;
		OCR1A = 15624;
		TIMSK1 = (0 << OCIE1A );		// timeri lopetetaan, jolloin voidaan käyttää delay funktiota.
	}
	
	///*
	if(pisteet > highScore){
		highScore = pisteet;
	}
    else{
      showResult(pisteet);
    }
	for(int i=0;i<3;i++) {
    clearAllLeds();
		buzzerTest(650);
    Serial.println("Clear all leds");

    delay(500);

    setAllLeds();
    Serial.println("set all leds.");
    buzzerTest(650);

    delay(300);
		
	} // for
  clearAllLeds();
  delay(3000);
  //peliKaynnissa=false;
	
	//*/
	Serial.print("Peli loppui! Tuloksesi on: "); //nämä nyt ovat vain huvin vuoksi laitettu jos meillä olisi oikea näyttö.
  // voidaan tulostaa näytölle arvo vielä tässä.
  //showResult(pisteet);
	Serial.println(pisteet);
	Serial.print("Pelisi kesti: ");
	//Serial.println(aika);

}

void idleDemo() {
  static int i=0;
  static int k=0;
  static int demo2[] = {0x0, 0x1, 0x3, 0x7, 0xF, 0xF, 0x7, 0x3, 0x1};   // eka demo. Voidaan testata vähän toimiiko unossa kunnolla.
    //static int demo1[] = {0xbf, 0xdf,0xef,0xf7,0xfb,0x7f};
  
  if(idleTimer == true) {
    if(peliKaynnissa==false) {
      naytademo(k);
      k++;
      
      //demoResult(demo[i]);
      setLed(demo2[i]);   // sytyttää ledin taulukon mukaan.
      i++;
      buzzerTest(650);
      // tämä pitänee tehdä ajastetusti timer arvolla. Vaikkakaan tähän ei mennä silloin kun peli on päällä.
      idleTimer=false;
    } // if
    if (i== 9) {
      i=0;
    }  
     if (k== 12) {
      k=0;
    }  
  } 
}
