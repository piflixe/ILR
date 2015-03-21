/*
Arduino Due Sketch for ILC

Author: Felix Piela
Date: 5.3.15

*/

// including libraries
#include <avr/pgmspace.h> // library for accessing Data stored in program memory 
#include "DueTimer.h"

// loading data
#include "Funktionswerte.h" // loading file with values of desired function

// declaring constants
const unsigned int PIN_ADC = A0;
const unsigned int PIN_DAC = DAC1;
const unsigned int alpha =40;
const unsigned int beta = 100;

// declaring variables
unsigned int valADC[Nval]; // array of values read from ADC
unsigned int valDAC[Nval]; // array of values to be written on DAC
unsigned int valDACn[Nval];// new DAC array
int error[Nval];  // array of values with errors (desired value - real value)

unsigned int i = 0;

// flag to be set when new values for DAC are ready
boolean flagReady = false;

// flag to be set when a periode of the signal is completed
boolean flagPeriod = false;

void setup() {
  // setting resoulution of ADC and DAC to 12bit
  analogWriteResolution(12);
  analogReadResolution(12);

  // Using internal Timer interrupt (DueTimer library)
  Timer3.attachInterrupt(changei).start(Tsmic);

  // initialise variables
  for (int j = 0; j < Nval; j++)
  {
    // setting ADC and error values to 0
    valADC[j] = 0;
    error[j] = 0;
    // setting DAC values to data table stored in progmem
    valDAC[j] = pgm_read_word(&table[j]);
  }
}

void loop() {
  // checking of Period is complete
  if (flagPeriod == true)
  {
    // clearing flag
    flagPeriod = false;

    // calculation error
    for (int j = 0; j < Nval; j++)
    {
      error[j] = pgm_read_word(&table[j]) - valADC[j];
    }

    // applying update law
    for (int j = 0; j < Nval; j++)
    {
      if (j == 0) // first value
      {
        valDACn[j]=valDAC[j] + error[j]/alpha + (error[j+1] + error[Nval])/beta;
      }
      else if (j == Nval) // last value
      {
        valDACn[j]=valDAC[j] + error[j]/alpha + (error[j-1] + error[0])/beta;
      }
      else // some values in the middle 
      {
        valDACn[j]=valDAC[j] + error[j]/alpha + (error[j+1] + error[j-1])/beta;
      }
    }
    
    // setting old to new values
    for (int j = 0; j<Nval; j++)
    {
      valDAC[j]=valDACn[j];
    }
  }
}

void changei() {
  // setting DAC value
  analogWrite(PIN_DAC, valDAC[i]);

  // setting ADC value
  valADC[i] = analogRead(PIN_ADC);

  // incrementing i
  i = i + 1;
  if (i >= Nval) { // checking if Period is complete
    i = 0;       // resetting i
    flagPeriod = true; // setting flag
  }
}
