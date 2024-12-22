// Tässä tiedostossa määritetään nappien käyttöön tarvittavat funktiot ja muuttujat

#include "buttons.h"

// Määritetään alustusfunktio
void initButtonsAndButtonInterrupts(void)
{
  // Asetetaan keskeytykset pois päältä alustuksen ajaksi
  cli();
  // Määritetään pinnit 2-6 siten, että ne ovat HIGH-tilassa, kun nappia ei paineta
  for (int i = 2; i < 7; i++)
  {
    pinMode(i, INPUT_PULLUP);
  }
  // Alustetaan pin change-keskeytykset rekistereiden PCICR ja PCMSK2 avulla
  PCICR |= (1 << PCIE2);
  PCMSK2 |= (1 << PCINT18) | (1 << PCINT19) | (1 << PCINT20) | (1 << PCINT21) | (1 << PCINT22);
  // Mahdollistetaan globaalit keskeytykset
  sei();
}

// Määritetään tavu, josta käännetään bitti keskeytyksessä sen perusteella, mitä nappia on painettu
volatile byte buttonPressed = 0;

// Keskeytyspalvelijaa kutsutaan, kun nappia painaessa pinnin tila muuttuu
// Se katsoo, mitä nappia on painettu, ja vaihtaa sitä vastaavan lipun (eli bitin tavussa) todeksi
ISR(PCINT2_vect) 
{
  if (PIND < 0b01111100)
  {
    static unsigned long last_action_time = 0;
    unsigned long action_time = millis();
    if (action_time - last_action_time > 175)
    {
      buttonPressed = ((-PIND) & 0b01111100) >> 2;
    }
    last_action_time = action_time;
  }
}

// Funktio, joka pyörii silmukassa odottaen napin painallusta ja suorittaa toiminnon, kun nappia on painettu
// Sisältää myös värähtelynestokoodin, joka varmistaa, että joka painalluksella suoritetaan toiminto vain yhden kerran
// Funktio myös nollaa tavun johon merkitään onko nappia painettu siitä huolimatta suoritetaanko napin toimintoa tai ei
void buttonPressLooper()
{
  if (buttonPressed)
  {
    for (int i = 0; i < 5; i++)
    {
      if (buttonPressed == (1 << i))
      {
        static unsigned long last_action_time = 0;
        unsigned long action_time = millis();
        if (action_time - last_action_time > 175)
        {
          /*
          // siirrettävä pääohjelmaan, marko oli tätä jo pohtinut siellä.
          if(i == 4){
            if(peliKaynnissa == false){
            peliKaynnissa = true;
          }else if(peliKaynnissa == true){
            peliKaynnissa = false;
          }}
          */

          Serial.print("Nappia ");
          Serial.print(i+2);
          Serial.println(" on painettu");
        }
        last_action_time = action_time;
        buttonPressed = 0;
      }
    }
  }
}
