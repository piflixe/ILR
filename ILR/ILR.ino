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
const unsigned int Nsmooth = 20;                                // number of values used as smoothing in update law
const float ILCgain = 0.1;                                      // overall gain of ILC 
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

void loop() {

}






