/*
Arduino Due Sketch for ILC

Author: Felix Piela
Date: 15.11.15

*/

#define DEBUGPRINT(x) if(debug==true) Serial.println( x );    // short call to debug messages

// including libraries
// #include <avr/pgmspace.h> // library for accessing Data stored in program memory 
#include "DueTimer.h"

// loading data
#include "Funktionswerte.h" // loading file with values of desired function

// DECLARING CONSTANTS -----------------------------------------
// for PINs
const unsigned int PIN_ADC = A0;
const unsigned int PIN_DAC = DAC1;
const unsigned int PIN_HARDWAREDEBUG = 53;

// constants for linear phase lead ILC
const unsigned int Nsmooth = 10;                                // number of values used as smoothing in update law
const float ILCgain = 0.2;                                      // overall gain of ILC 
volatile float SmoothingWeight[Nsmooth];                     // average weighting for smoothing used in update law     
const int PhaseLead = 30;                                       // discrete Phase Lead for digital smoothing in update law (must be in the range of [1:Nval-Nsmooth]

// DECLARING VARIABLES -----------------------------------------
// core ILC
// volatile unsigned int inputSignal[Nval]; // array of values read from ADC
float outputSignal[Nval];                  // array of values to be written on DAC
int error[Nval];                           // array of values with errors (desired value - real value)
volatile unsigned int timeIndex = 0;                  // index to be incremented by ISR

// debugging variables
unsigned int badness = 0;                // sum of all quadratic errors for measuring overall control performance (debug only)
const boolean debug = false;             // debugging with Serial Console (works only for very low Tsmic)
const boolean hardwareDebug = false;     // debugging with measuring certain timings via digital i/o PINs

void setup() {
  // setting resoulution of ADC and DAC to 12bit
  analogWriteResolution(12);
  analogReadResolution(12);

  // Using internal Timer interrupt (DueTimer library)
  Timer3.attachInterrupt(changeIndex).start(Tsmic);
  
  // using Serial Interface for debugging
  Serial.begin(115200);

  // configuring PINs
  pinMode(PIN_ADC, INPUT);
  pinMode(PIN_DAC, OUTPUT);
  pinMode(PIN_HARDWAREDEBUG, OUTPUT);
  digitalWrite(PIN_HARDWAREDEBUG, LOW);
  
  DEBUGPRINT("gelesene Werte:")
  // initialise variables

  // initialise error and outputSignal 
  for (int j = 0; j < Nval; j++)
  {
    error[j] = 0;                     // setting ADC and error values to 0
    outputSignal[j] = table[j];       // setting DAC values to data table stored in progmem
    //pgm_read_word(&table[j]);

    // printing outputSignal on Serial Console for debugging
    DEBUGPRINT(outputSignal[j]);
  }

  for (int j = 0; j < Nsmooth; j++)
  {
    SmoothingWeight[j] = ILCgain/Nsmooth;
    if(Nsmooth>6)  // apply linear window
    {
      if ( (j==1) || (j==Nsmooth) ) SmoothingWeight[j] = SmoothingWeight[j] / 2;
    }
  }
  if(debug==true) delay(1000);
}

void loop() {

}

void changeIndex() {
  // setting DAC value
  analogWrite(PIN_DAC, ((int)outputSignal[timeIndex]));

  // measuring current value on ADC
  error[timeIndex] = table[timeIndex] - analogRead(PIN_ADC);
  
  if(debug==true){ // calculating badness and printing error only in debug mode
    badness = badness + error[timeIndex]*error[timeIndex]; // incrementing badness by quadratic error
//    if(timeIndex==0) Serial.print("Fehler: ");
//    char errorBuffer[6];
//    sprintf(errorBuffer, " %4i", error[timeIndex]);
//    Serial.print(errorBuffer);
//    if(timeIndex==(Nval-1)) Serial.print("\n");    // new line at end of period
  }

  // UPDATE LAW
  outputSignal[indexShift(timeIndex)] = updateLaw((timeIndex));
 
  timeIndex = timeIndex + 1;  // incrementing i (time axis)
  if (timeIndex >= Nval) // checking if Period is complete
  {
    timeIndex = 0;       // resetting i for next period

    // debugging code
    DEBUGPRINT("Badness:")
    DEBUGPRINT(badness)
    badness = 0; // resetting badness for next period  
  }
}






