#ifndef BUTTONS_H
#define BUTTONS_H
#include "Arduino.h"
const byte firstPin = 2; // First PinChangeInterrupt on D-bus
const byte lastPin =  5; // Last PinChangeInterrupt on D-bus
/* 
  initButtonsAndButtonInterrupts subroutine is called from Setup() function
  during the initialization of Speden Spelit. This function does the following:
  1) Initializes 4 button pins for the game = Arduino pins 2,3,4,5
  2) Initializes 1 button pin for starting the game = Aruino pin 6
  3) Enables PinChangeInterrupt on D-bus in a way that interrupt
     is generated whenever some of pins 2,3,4,5,6 is connected to LOW state

*/

// Tämä funktio alustaa painikkeet ja mahdollistaa keskeytykset
void initButtonsAndButtonInterrupts(void);

// Tavun prototyyppi, tavu määritetään buttons.cpp-tiedostossa
extern volatile byte buttonPressed;

// Prototyyppi funktiolle, joka pyörii silmukassa ja suorittaa toiminnon, jos nappia on painettu sekä alustaa lipputavun
void buttonPressLooper(int *);

#endif