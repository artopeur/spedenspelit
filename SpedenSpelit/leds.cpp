#include <arduino.h>

void initializeLeds() {
	const int leds[]={A2, A3, A4, A5};		// määritetään pinnit taulukkoon leds
  const int buzzerPin=7;
  pinMode(buzzerPin, OUTPUT);

	for(int i=0;i<4;i++) {			// alustetaan pinnit käyttämään OUTPUT moodia
		pinMode(leds[i], OUTPUT);
	}
	
}
void clearAllLeds(void) {			// asetetaan ledit aktiiviseksi
	const int leds[]={A2, A3, A4, A5};
	for(int i=0;i<4;i++) {
		digitalWrite(leds[i], LOW);
	}
}

void setAllLeds(void) {				// asetetaan ledit inaktiiviseksi
	const int leds[]={A2, A3, A4, A5};
	for(int i=0;i<4;i++) {
		digitalWrite(leds[i],HIGH);
	}
}
/* 
voidaan syöttää monenlaisia arvoja
1+2+4+8 sytyttää kaikki ledit, koska arvoksi tulee 0xF = 15
1+2 sytyttää vain ensimmäiset 2 lediä oikealta
1 sytyttää vain ensimmäisen ledin oikealta.
Alla binääreinä lyhyesti
0001		= 1 = oikean puoleinen ledi
0010		= 2 = toinen oikealta
0100		= 4	= toinen vasemmalta
1000		= 8 = ensimmäinen vasemmalta.

Lediohjausta onnistuu myös muillakin menetelmillä. Voidaan kytkeä eri ledit palamaan tarvittaessa.
järjestys jos halutaan esim siisti alkuanimaatio ledeillä, kun peli ei ole käynnissä. 
6 eri lukemaa = 0x00 (00), 0x0c(12), 0x0e(14), 0x0f (15), 0x0e (14), 0x0c(12), 0x00(0)

tai
int demo[] = {0x0, 0x8, 0x4, 0x2, 0x1, 0xc, 0xe, 0xf, 0xe, 0xc, 0x0,};

for(int i=0; i<11;i++) {
	setLed(demo[i]);
	delay(1000);
}
demo[] = {0x1, 0x3, 0x7, 0xF, 0xF, 0x7, 0x3, 0x1, 0, 0, 0};
for(int i=0; i<8;i++) {
	setLed(demo[i]);
	delay(500);

*/
void setLed(byte ledNumber) {			
	const int leds[]={A2, A3, A4, A5};
  Serial.println();
  Serial.print("Ledinarvo leds.cpp:llä on: ");
  Serial.println(ledNumber);
	switch(ledNumber) {			
		case 0x00:
			clearAllLeds();
			break;
		case 0x01:  //0001
			clearAllLeds();
			digitalWrite(leds[3], HIGH);
			break;
		case 0x02:   //0010
			clearAllLeds();
			digitalWrite(leds[2], HIGH);
			break;
		case 0x03: // 0011
			clearAllLeds();
			digitalWrite(leds[2], HIGH);
      digitalWrite(leds[3], HIGH);
			break;
    case 0x04: //0100
      clearAllLeds();
      digitalWrite(leds[1], HIGH);
      break;
    case 0x05: // 0101
      clearAllLeds();
      digitalWrite(leds[1], HIGH);
      digitalWrite(leds[3], HIGH);
      break;
    case 0x06: //0110
      clearAllLeds();
      digitalWrite(leds[1], HIGH);
      digitalWrite(leds[2], HIGH);
      break;
    case 0x07:  //0111
      clearAllLeds();
      digitalWrite(leds[1], HIGH);
      digitalWrite(leds[2], HIGH);
      digitalWrite(leds[3], HIGH);
      break;
    case 0x08: //1000
      clearAllLeds();
      digitalWrite(leds[0], HIGH);
      break;
    case 0x09:  //1001
      clearAllLeds();
      digitalWrite(leds[0], HIGH);
      digitalWrite(leds[3], HIGH);
      break;
    case 0x0A:    // 1010 = 10
      clearAllLeds();
      digitalWrite(leds[0], HIGH);
      digitalWrite(leds[2], HIGH);
      break;
    case 0x0B:    // 1011 = 11
      clearAllLeds();
      digitalWrite(leds[0], HIGH);
      digitalWrite(leds[2], HIGH);
      digitalWrite(leds[3], HIGH);
      break;
    case 0x0C:    // 1100 = 12
      clearAllLeds();
      digitalWrite(leds[0], HIGH);
      digitalWrite(leds[1], HIGH);
      break;

    case 0x0D:    // 1101 = 13
      clearAllLeds();
      digitalWrite(leds[0], HIGH);
      digitalWrite(leds[1], HIGH);
      digitalWrite(leds[3], HIGH);
      break;
    case 0x0E:   // 1110 = 14
      clearAllLeds();
      digitalWrite(leds[0], HIGH);
      digitalWrite(leds[1], HIGH);
      digitalWrite(leds[2], HIGH);
      break;
    case 0x0F:  // 1111 = 15
      setAllLeds();
    break;
		default:
			
			break;
	}
  
}

void buzzerTest(int buzzTone) {
  const int buzzerPin=7;
  tone(buzzerPin,buzzTone,100);

}
