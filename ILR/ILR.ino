/*
Arduino Due Sketch for ILC

Author: Felix Piela
Date: 15.11.15

*/

#define DEBUGPRINT(x) if(debug==true) Serial.println( x );    // short call to debug messages

// including libraries
// #include <avr/pgmspace.h> // library for accessing Data stored in program memory 
#include "DueTimer.h"
#include <stdio.h>
#include <stdlib.h>

// loading data
#include "Funktionswerte.h" // loading file with values of desired function

// DECLARING CONSTANTS -----------------------------------------
// for PINs
const unsigned int PIN_ADC = A0;
const unsigned int PIN_DAC = DAC1;
const unsigned int PIN_HARDWAREDEBUG = 53;

// DECLARING VARIABLES -----------------------------------------
// core ILC
unsigned int Nsmooth = 10;                 // number of values used as smoothing in update law (must be smaller than NsmoothMax
const unsigned int NsmoothMax = 30;        // maximum number of index smoothing
float ILCgain = 0.005;                      // overall gain of ILC 
volatile float SmoothingWeight[NsmoothMax];// average weighting for smoothing used in update law     
int PhaseLead = 1;                        // discrete Phase Lead for digital smoothing in update law (must be in the range of [1:Nval-Nsmooth]
boolean Stop = true;                       // start and stop the experiment

float outputSignal[Nval];                  // array of values to be written on DAC
int error[Nval];                           // array of values with errors (desired value - real value)
volatile unsigned int timeIndex = 0;       // index to be incremented by ISR

// Variables for Serial Communication
String inputString1 = "";                  // a string to hold incoming data for menu
String inputString2 = "";                  // a string to hold incoming data for menu
boolean stringComplete = false;            // whether the string is complete
int menuState = 0;


// debugging variables
unsigned int badness = 0;                // sum of all quadratic errors for measuring overall control performance (debug only)
const boolean debug = false;             // debugging with Serial Console (works only for very high Tsmic -> low frequency)
const boolean hardwareDebug = false;     // debugging with measuring certain timings via digital i/o PINs

void loop() {
  getParamValuesFromSerial(true);        // use with true for verbose output
}


